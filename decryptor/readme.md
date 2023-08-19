# Sirius Decryptor

## Fill AES key in `main.cpp`

Fill the `info_key` in line 69 and `chart_key` in line 70.

Due to some reasons, we are not able to provide AES key in the code. You can find by yourself accroding to the article below.

If you have any requirements like coding a chart maker, you can email littleyang0531@outlook.com to provide key for you.

## Install Dependencies

```bash
sudo apt install g++ openssl libbrotli-dev -y
```

## Compile Program

```bash
g++ main.cpp -omain -lcrypto -lbrotlidec -g
```

## Usage

```bash
./main info music_config.enc music_config  # Decrypt encoded music_config
./main chart 1.enc 1.txt  # Decrypt encoded chart file
```

## Others

- [Unity 应用解包与资源文件解密(zh-cn)](./conclusion.md)
