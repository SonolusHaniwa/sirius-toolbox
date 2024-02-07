#include<bits/stdc++.h>
using namespace std;
#include"utils.h"

bool keyword_search(string s, string k) {
    int pt = -k.size();
    while (string::npos != (pt = s.find(k, pt + k.size()))) {
        if (pt && s[pt - 1] != ' ') continue;
        if (pt + k.size() < s.size() && s[pt + k.size()] != ' ') continue;
        return true;
    } return false;
}
int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " [dump.cs] [table.json]";
        return 1;
    } 

    string content = readFile(argv[1]);

    int percent = 0;
    bool inClass = false, inEnum = false, inInterface = false;
    bool inClassField = false; string currentKey = "";
    int classNum = 0, enumNum = 0, interfaceNum = 0, bracket = 0;
    string lineNow = "", nameSpace = "", name = "";
    Json::Value result, single;
    for (int i = 0; i < content.size(); i++) {
        // 进度条
        int newPercent = (long long)i * 100 / content.size();
        if (newPercent != percent) {
            percent = newPercent;
            cout << "Processed: " << percent << "% (" << i << "/" << content.size() << ")" << endl;
        } 
        if (content[i] != '\n') { lineNow.push_back(content[i]); continue; }
        while (lineNow.size() && (lineNow.front() == '\t' || lineNow.front() == ' ')) lineNow = lineNow.substr(1);

        // 判断 NameSpace 名
        if (lineNow.substr(0, 14) == "// Namespace: ") {
            nameSpace = lineNow.substr(14);
            while (nameSpace.size() && (nameSpace.back() == '\t' || nameSpace.back() == ' ')) nameSpace.pop_back();
        }
        
        // 判断是否进入 Class
        bool start = lineNow.find("// TypeDefIndex: ") != string::npos;
        if (start) {
            inClass = keyword_search(lineNow, "class") || keyword_search(lineNow, "struct"); classNum += inClass;
            inEnum = keyword_search(lineNow, "enum"); enumNum += inEnum;
            inInterface = keyword_search(lineNow, "interface"); interfaceNum += inInterface;
            if (keyword_search(lineNow, "class")) name = lineNow.substr(lineNow.find("class") + 6);
            if (keyword_search(lineNow, "struct")) name = lineNow.substr(lineNow.find("struct") + 7);
            if (keyword_search(lineNow, "enum")) name = lineNow.substr(lineNow.find("enum") + 5);
            if (keyword_search(lineNow, "interface")) name = lineNow.substr(lineNow.find("interface") + 10);
            if (name.find("//") != string::npos) name = name.substr(0, name.find("//"));
            while (name.size() && (name.back() == '\t' || name.back() == ' ')) name.pop_back();
            single = Json::Value();
        }

        if (nameSpace.substr(0, 16) == "SiriusApi.Shared") {
            // 如果是 Enum 类型
            if (inEnum && lineNow.size() > 14 + name.size() && lineNow.substr(0, 14 + name.size()) == "public const " + name + " ") {
                string info = lineNow.substr(14 + name.size()); 
                if (info.size()) {
                    info.pop_back();
                    auto tmp = explode(" = ", info.c_str());
                    Json::Value tmp2;
                    tmp2["name"] = tmp[0];
                    tmp2["value"] = tmp[1];
                    single["value"].append(tmp2);
                    single["type"] = "enum";
                }
            }
            // 如果是 Class 类型
            if (inClass) {
                if (lineNow.substr(0, 2) == "//") {
                    if (lineNow.substr(3) == "Properties") inClassField = true;
                    else inClassField = false;
                } if (inClassField) {
                    if (lineNow.substr(0, 5) == "[Key(") currentKey = lineNow.substr(0, lineNow.size() - 2).substr(5);
                    else if (lineNow.find("public ") != string::npos) {
                        string info = lineNow.substr(7);
                        info.pop_back();
                        auto tmp = explode(" ", info.c_str());
                        Json::Value tmp2;
                        tmp2["type"] = tmp[0];
                        tmp2["name"] = tmp[1];
                        tmp2["key"] = currentKey; currentKey = "";
                        single["value"].append(tmp2);
                        single["type"] = "class";
                    }
                }
            }
        }

        // 计算括号层数
        int pre = 0, suf = 0;
        for (int j = 0; j < lineNow.size(); j++) {
            if (lineNow[j] == '{') pre++;
            if (lineNow[j] == '}') suf++;
        } bracket += pre - suf;

        if (!start && bracket == 0) {
            name = explode(" : ", name.c_str())[0];
            if (nameSpace.substr(0, 16) == "SiriusApi.Shared") result[name] = single;
            inClass = inEnum = inInterface = false;
        }
        lineNow = "";
    }

    cout << "Class Number: " << classNum << endl;
    cout << "Enum Number: " << enumNum << endl;
    cout << "Interface Number: " << interfaceNum << endl;
    ofstream fout(argv[2]);
    fout << json_pretty_encode(result);
}