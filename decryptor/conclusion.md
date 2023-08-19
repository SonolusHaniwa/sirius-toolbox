# Unity 应用解包与资源文件解密

**本文仅供学习参考，请勿将其用于非法用途**

## 前言

近日，“世界大明星”动画的衍生音游“世界大明星 梦想的群星(App 名 ユメステ)”上线于 Google Play Store。作为世界计划的玩家之一，看到出了一款同样包含大小键的新音游，于是毫不犹豫地下载了下来。

由于最近才完成了 [sonolus-hwpl-engine](https://github.com/SonolusHaniwa/sonolus-hwpl-engine) 的重构，于是我在想能不能将这个游戏也搬到 Sonolus 上去。

由于是新出的一款音游，在网上找不到任何的资源。没办法，我只好参考世界计划的解密方法，自行解密其谱面文件来分析。这是笔者第一次进行资源文件的解密，走了不少的弯路，所以写个文章记录一下，万一以后还有要用的呢。

## 目录

本文章将分为以下几个板块进行记录:

- [apktool 解包与回编]()
- [il2cppdumper 逆向]()
- [frida 初探]()
- [ida 逆向分析]()

## apktool 解包与回编

由于我们想要获取到加密后的谱面文件，对应用网络资源的访问是必不可少的，这里我们采用 anyproxy 来进行抓包。

抓包有一个前提，需要将抓包软件提供的证书安装在 Android 用户 CA 证书目录下。但是由于高版本 Android 的限制，用户 CA 证书同样不受信任，只有系统 CA 证书才受信任，就要求手机必须 root。但是如果应用主动信任用户 CA 证书，那么该应用就能被顺利抓包。

### apktool 解包

首先把应用安装包下载到电脑上后，利用 apktool 这个工具，我们就可以将安装包解开，看到部分的配置文件。

```bash
java -jar ./apktool.jar d yumesute.apk
```

执行以上命令后，将在同目录下新建一个 `yumesute` 的文件夹，里面就是安装包内的所有内容了。

进入该文件夹，新增一个 `res/xml/network_security_config.xml` 文件，内容如下:

```xml
<?xml version="1.0" encoding="utf-8"?>
<network-security-config>
    <base-config cleartextTrafficPermitted="true">
        <trust-anchors>
            <certificates src="user" />
            <certificates src="system" />
        </trust-anchors>
    </base-config>
</network-security-config>
```

接下来，打开根目录下的 `AndroidManifest.xml` 文件，在 `application` 元素里加一个 `android:networkSecurityConfig="@xml/network_security_config"` 属性，这个软件发出的所有网络请求都可以被我们的 anyproxy 给捕捉到了。

为了方便，我们先把应用的包名也改一下，避免出现各种冲突问题导致需要卸载原应用才能安装的上去。

同样在 `AndroidManifest.xml` 中，搜索所有的 `com.kms.worlddaistar`(这是原应用的包名)，然后将除了出现在 `activity` 元素里的全部改为 `com.kms.worlddaistar2`(这是新包名)，这样一来这个应用的包名就是 `com.kms.worlddaistar2` 了。

### apktool 回编与应用安装

退出 `yumesute` 目录，使用如下指令编译新应用安装包:

```bash
java -jar ./apktool.jar b yumesute -o yumesute.apk
```

要想安装在实机上还需要对安装包进行签名才能安装:

```bash
keytool -genkey -v -keystore abc.keystore -alias abc.keystore -keyalg RSA -validity 365
jarsigner -verbose -keystore abc.keystore -signedjar yumesute_signed.apk yumesute.apk abc.keystore
```

生成的 `yumesute_signed.apk` 通过 `adb` 安装到手机上就行了。

如果不换包名的话，会导致我们的签名与原应用的签名不同，从而导致无法安装。所以在上一步我们才需要换掉包名，目的就是为了避免签名冲突。

接下来，只需要安装 anyproxy 的证书到用户 CA 证书目录中，启动 anyproxy 后，设置好网络代理，再打开我们才安装好的新应用。进入 anyproxy 的管理页面，我们就会发现，所有的网络请求信息都被我们的 anyproxy 捕捉到了，至此这一步就完成了。

(好像 anyproxy 给的链接有点问题，使用脚本修正一下就好了。)

![anyproxy 管理页面](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/d9330c2e-1d91-4000-8dd4-adadaa873c2a)

## il2cppdumper 逆向

注意到有两个链接(上面的图上没有):

```url
https://assets.wds-stellarium.com/production/Notations/4/music_config.enc
https://assets.wds-stellarium.com/production/Notations/4/5.enc
```

一个是音乐配置文件的下载链接，另一个是谱面文件的下载链接。

下载下来后你会发现全都是些乱码，而且后缀名还是 `.enc`，肯定是加了密的。要想知道怎么解密，肯定需要知道源代码才行。

在此，我们使用 il2cppdumper 来逆向，将程序使用的函数全部挖出来，再使用 dnSpy 来查找他可能用到的函数。

于是，打开 dnSpy，你惊奇地发现:

![dnSpy 界面](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/9bce3e64-7d30-468a-b7ad-ef501564e6fe)

好家伙，这么多程序集，程序集下面又有一堆名字空间，名字空间下面又有一堆类，类下面又有一堆方法，这无疑是大海捞针啊。

找了半天，发现了下面这个东西:

![dnSpy 界面](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/e85f236b-c982-45f0-8d4c-dbdbdfdfacdd)

哦原来用的是 AES 来加密吗，参考世界计划对谱面的处理，好像是这么一回事，那接下来只需要获取到 AES 的密钥和 iv 不就可以解出来了吗。

## frida 初探

程序在 `CreateWithInitialize` 这个方法里面传入了加密所需要的 iv 和 password，因此我们只需要获取到这个函数的参数就行了。

这个时候就要请上我们 Hook 的工具 frida 了。

### frida 安装

官方建议是把 `frida-server` 传到手机上启动，但是很不幸的是，没有 root 权限，意味着没法改变可执行文件的运行权限，这条路算是彻底寄了。

但是你会发现还有个东西 `frida-gadget`，这东西只要包到应用里就可以了，根本不需要 root。正巧的是，在第一个板块，我们就使用了 `apktool` 来拆包与回编。

于是下载 `frida-gadget.so` 后，重命名为 `libfrida-gadget.so`，扔到 `yumesute/lib/arm64-v8a/` 下面去。

要想程序能够调用这个库文件，我们需要修改一下应用，打开 `AndroidManifest.xml`，我们不是还有个没改的 `com.kms.worlddaistar.UnityPlayerActivityOverride` 吗，说明这就是程序的入口文件。

打开 `yumesute/smali/com/kms/worlddaistar/UnityPlayerActivityOverride.smali`，在构造函数 `.method public constructor <init>()V` 中新增以下内容:

```smali
const-string v0, "frida-gadget"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

为了能够尽早调用 frida，我们才需要将这句加在构造函数中。

而且这里有很多的坑点，如果你把这句话放在正常函数中，`v0` 后面的那个数字不能超过 `15`，否则即使能够回编成功，应用也无法启动；而如果放在构造函数中，这个数字甚至不能超过 `1`，否则也不能正常启动。

按照第一个板块中的方法回编并安装后，打开应用，只要应用能够暂停在黑色界面不闪退，那么说明 frida 就注入成功了。

接下来的就是在电脑端上安装 `frida` 和 `frida-tools` 就行了。

```bash
python -m pip install frida frida-tools
```

### frida 使用

frida 其实挺好用的，网上的教程一抓一大把，我就说一些需要注意的点:

1. 由于我们使用的是 `frida-gadget`，官网也说了，包名并不是我们打包的那个包名，而是固定的 `Gadget`，因此我们需要使用 `session = frida.get_usb_device().attach("Gadget")` 才能正常连接到应用。
2. 网上有很多关于函数在内存中的地址的争论，有说直接 `Module.findBaseAddress("libil2cpp.so").add(address)` 的，还有需要再加 `1` 的，甚至还有有理有据的分析说什么应该要再加 `1`。但实际上这个跟设备的架构相关，`arm` 架构的设备不需要加 `1` 就能获取到真实地址。

### frida-il2cpp-bridge

经历了六七个小时的 hook 后，我放弃了。由于 Unity 中 Byte 结构的特殊性，我获取到的参数地址总是会与真实的地址出现偏差。

那就没办法了吗？这时我无意中发现了一个工具 `frida-il2cpp-bridge`，它可以自动解析出 Unity 中的 Byte 结构。

我心想，我不就是我需要的东西吗？于是开始光速开始配置 `frida-il2cpp-bridge` 的环境。

新建一个 `package.json` 文件，写入以下内容

```json
{
    "name": "playground",
    "main": "index.ts",
    "version": "1.0.0",
    "private": true,
    "type": "module",
    "scripts": {
        "build": "frida-compile -o _.js index.ts",
        "attach": "frida -U \"Gadget\" -l _.js --runtime=v8",
        "spawn": "frida -U -f \"Gadget\" -l _.js --no-pause --runtime=v8"
    },
    "devDependencies": {
        "@types/frida-gum": "^18.3.1",
        "frida-compile": "^16.2.2",
        "frida-il2cpp-bridge": "*"
    }
}
```

新建 `tsconfig.json` 文件，写入以下内容

```json
{
    "compilerOptions": {
        "target": "esnext",
        "lib": [ "es2022" ],
        "experimentalDecorators": true,
        "module": "esnext",
        "allowJs": false,
        "noEmit": false,
        "esModuleInterop": false,
        "moduleResolution": "nodenext",
        "strict": true,
        "sourceMap": true
    },
    "files": [ "index.ts" ]
}
```

使用以下指令开始配置 `frida-il2cpp-bridge` 的环境:

```bash
npm install
```

环境安装好后，新建 `index.ts` 文件，写入以下内容:

```typescript
import "frida-il2cpp-bridge";

Il2Cpp.perform(() => {
    const e = Il2Cpp.domain.assembly("Sirius.Security").image.class("Plugins.Sirius.Security.Cryptography.CustomAesEncoder");
    Il2Cpp.trace(true).classes(e).and().attach();
});
```

手机先打开应用，然后连接电脑，输入以下指令开始 Hook

```bash
npm run build & npm run attach
```

这时候，应用成功启动，随便点击一个谱面来玩。在加载谱面的时候，AES 加密的 password 和 iv 就打印在我们控制台上了。

![raw data](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/219f87b8-6a95-4d77-874f-df449ff652ab)

兴冲冲地拿着 `password` 和 `iv` 去控制台里尝试，但是报错 `bad magic number`。

我不知道是我电脑的问题还是什么，反正我就是解不了。

没办法，再挖一下，发现 `Plugins.Sirius.Security.Cryptography.CustomAesEncoder` 还调用了个 `System.Security.Cryptography`。

经过一段时间的寻找，又发现了这个类:

![dnSpy 界面](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/f108331c-0c8b-4af0-880b-b884ae875721)

里面的 `createDecryptor` 函数不是正好就给了我 key 和 iv 吗。于是修改 `index.ts`，追踪一下这个类:

```typescript
import "frida-il2cpp-bridge";

Il2Cpp.perform(() => {
    const e = Il2Cpp.domain.assembly("System.Core").image.class("System.Security.Cryptography.AesCryptoServiceProvider");
    Il2Cpp.trace(true).classes(e).and().attach();
});
```

再次打开游戏，加载谱面的时候，这次不仅将 key 和 iv 吐出来了，还告诉我们使用的 `AES-256-CBC` 进行加密的，这一点和世界计划是不一样的。

![raw data2](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/39e4323e-8dcf-4caa-953b-d078d323cc62)

这次再拿着 key 和 iv 去控制台里尝试，成功解密。

但是如果你多开几个谱面，你会发现，`music_config.enc` 的 key 和 iv 与 `5.enc` 都不同，而不同歌曲间的 key 虽然相同，但 iv 也不同。仔细观察，你会发现，其实 iv 就是 `*.enc` 文件的前 16 位。

还有个问题，解出来的文件与你控制台里打出来的对比一下，你会发现你解出来的会多了前 16 位。显然这是没有意义的，在程序里甚至是直接舍掉了。因此这前 16 位我们直接舍掉就好了。

至此，所有解包解密过程结束。本文完。

## ida 逆向分析

你不会真的以为就完了吧。

`music_config.enc` 解出来是没有问题的，但是 `5.enc` 解出来还是个二进制文件，看来还用了其他的加密手段。没办法，只有看看用 ida 来看看了。

用 ida 打开 `libil2cpp.so`，使用 `il2cppdumper` 里的 `ida_py3.py` 将函数导入进去。

先利用 frida 找到函数 `Plugins.Sirius.Security.Cryptography.CustomAesEncoder.Decrypt` 的调用栈。新建 `script.py`，写入如下内容:

```python
import frida
import sys

rdev = frida.get_usb_device()
session = rdev.attach("Gadget")

def on_message(message, data):
    if message['type'] == 'send':
        print("[*] {0}".format(message['payload']))
    else:
        print(message)

scr = """
Java.perform(function(){
    // Hook Constructor
    var initPointer = Module.findBaseAddress("libil2cpp.so").add(0x5F4A074); // 函数地址，在 dnSpy 里面去看
    console.log("initPointer: " + initPointer);
    Interceptor.attach(initPointer, {
        onEnter: function(args) {
            console.log(Thread.backtrace(this.context, Backtracer.ACCURATE).map(DebugSymbol.fromAddress).join("\\n"));
            console.log(Thread.backtrace(this.context, Backtracer.FUZZY).map(DebugSymbol.fromAddress).join("\\n"));
        },
    });
});
"""

script = session.create_script(scr)
script.on('message', on_message)
print('[*] Running CTF')
script.load()
sys.stdin.read()
```

手机打开应用，连接电脑，`python script.py` 就可以启动了。

同样在打开谱面的时候，会输出当前的调用栈中所有函数的地址值。

已知当前函数的地址为 `0x5F4A074`，那调用这个函数的地址就理应是下一行的 `0x445DEAC`。

在 ida 中按下 G 键，输入 `0x445DEAC` 后就会跳转到一个地方。

![ida](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/4780e264-a3f4-41b1-8dd4-940842a67e31)

看不懂？直接按下 F5 反编译，你会发现给了我们一连串的 C 代码。

![ida](https://github.com/SonolusHaniwa/sirius-decryptor/assets/63852815/09a4ef95-a5b8-435d-b871-d1726a4206d1)

说实话我其实不怎么能看得懂，毕竟 `v1`, `v2`, `v3` 这样的神仙命名看得懂就有鬼了。

但是你会发现，我们目前的光标位置是在调用 `Sirius_Compress_BrotliCompressHelper__DecompressToString` 这个函数，而上面一行就是我们刚刚的 `Plugins_Sirius_Security_Cryptography_CustomAesEncoder__Decrypt` 函数。

那么结果就出来了，应用先通过 AES 将谱面文件解出来后，再利用这个叫 `brotli` 的方法来解压缩的。

于是网上随便搜了个解 `brotli` 的工具，一解，你惊奇地发现，出来的正是我们想要的文本文件了。

至此，所有解包解密过程正式宣告结束。本文完。
