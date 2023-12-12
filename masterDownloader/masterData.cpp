#include<bits/stdc++.h>
using namespace std;

#include"utils.h"

ifstream fin;

Json::Value getEnvironment(string appVer, string gameVer) {
	string url = "https://api.wds-stellarium.com/api/Environment?applicationVersion=" + appVer + "&gameVersion=" + gameVer;
	map<string, string> response;
	return json_decode(geturl(url, getHeader(), "", true, response));
}
Json::Value accountAuthenticate(string accountToken, string appVersionFull) {
	string url = apiBase + "/api/account/Authenticate";
	Json::Value data; data["LoginToken"] = accountToken;
	data["GameVersion"] = 1; data["ApkHash"] = "";
	data["ApkApplicationSignature"] = "";
	data["ApplicationVersion"] = appVersionFull;
	map<string, string> response;
	return json_decode(geturl(url, getHeader(), json_encode(data), true, response));
}
Json::Value getMasterData() {
	string url = apiBase + "/api/data/master";
	map<string, string> response;
	return json_decode(geturl(url, getHeader({}, false), "", false, response));
}
string getMasterMemory(string url) {
	url = proxyUrl + masterBase + "/" + url;
	map<string, string> response;
	return geturl(url, {}, "", false, response);
}

map<string, Json::Value> master;
map<string, vector<string> > master_structure;
Json::Value solveData(Json::Value originalData, string masterName) {
    // for (int i = 0; i < master_structure[masterName].size(); i++) cout << master_structure[masterName][i] << endl;
    if (originalData.size() != master_structure[masterName].size()) {
        cout << masterName << " " << originalData.size() << " " << master_structure[masterName].size() << endl;
        cout << originalData;
        return Json::Value();
    } Json::Value res;
    for (int i = 0, j = 0; i < originalData.size() && j < master_structure[masterName].size(); i++, j++) {
        string variableDefine = master_structure[masterName][j];
        string variableType = variableDefine.substr(0, variableDefine.find_last_of(" "));
        string variableName = variableDefine.substr(variableDefine.find_last_of(" ") + 1);
        variableName.pop_back();
        if (variableType.find("struct ") == string::npos) res[variableName] = originalData[i];
        else if (variableType.find("vector<struct ") != string::npos) {
            string structName = variableType.substr(variableType.find("vector<struct ") + 14);
            if (structName.find("System") != string::npos) res[variableName] = originalData[i];
            else {
                structName.pop_back(); Json::Value arr;
                for (int j = 0; j < originalData[i].size(); j++)
                    arr.append(solveData(originalData[i][j], structName));
                res[variableName] = arr;
            }
        } else if (variableType.find("struct ") != string::npos) {
        	string structName = variableType.substr(variableType.find("struct ") + 7);
        	if (master_structure.find(structName) == master_structure.end()) res[variableName] = originalData[i];
        	else res[variableName] = solveData(originalData[i], structName);
        }
    } return res;
}

string getApplicationVersion() {
    string url = "https://apps.apple.com/jp/app/id6447166623";
    map<string, string> response;
    string res = geturl(url, {}, "", false, response);
    int pos = res.find("バージョン");
    string VersionNumberTmp = res.substr(pos + 16, 10), version = "";
    for (int i = 0; i < VersionNumberTmp.size() && VersionNumberTmp[i] != '<'; i++) version.push_back(VersionNumberTmp[i]);
    return version;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " [dir]" << endl;
        return 1;
    } string dir = argv[1]; mkdir(dir); mkdir(dir + "/master");

	string currentVersion = readFile(dir + "/version.json");
    auto currentInfo = currentVersion == "" ? Json::Value() : json_decode(currentVersion);
    getCurrentInfo(currentInfo);

	appVersionFull = getApplicationVersion() + ".76";
	appVersion = appVersionFull;
	while (appVersion.size() && appVersion.back() != '.') appVersion.pop_back();
	if (appVersion.size()) appVersion.pop_back();

	// 获取环境信息
	auto env = getEnvironment(appVersion, "1");
	apiBase = env["result"]["apiEndpoint"].asString();
	masterBase = env["result"]["masterDataUrl"].asString();
	assetsBase = env["result"]["assetUrl"].asString();
    assetsVersion = env["result"]["assetVersion"].asString();
    currentInfo["url.apiBase"] = apiBase;
    currentInfo["url.masterBase"] = masterBase;
    currentInfo["url.assetsBase"] = assetsBase;
    currentInfo["assets.version"] = assetsVersion;
    currentInfo["app.version"] = appVersion;
	
	// 远程验证
	auto authenticate = accountAuthenticate(accountToken, appVersionFull);
	authorization = authenticate["result"]["token"].asString();
    currentInfo["account.authorization"] = authorization;
	
	// 获取 Master Data
	auto masterData = getMasterData();
	string version = masterData["result"]["version"].asString();
	cout << "Dumping Master Data Version " << version << "..." << endl;
	if (version == currentInfo["masterData.version"].asString()) {
		cout << "Current Master Memory Version is " << version << ", no need to dump!" << endl;
        saveCurrentInfo(dir + "/version.json", currentInfo);
		return 0;
	} currentInfo["masterData.version"] = version;
	
	// 获取 Master Memory
	string masterMemoryDB = getMasterMemory(masterData["result"]["uri"].asString());
	ofstream fout(dir + "/master/mastermemory.db", ios::binary);
	fout.write(const_cast<char*>(masterMemoryDB.c_str()), masterMemoryDB.size());
	fout.close();

	// 解密 Master Memory
	// 获取 offset 和 len
	int dataOffset = 0, curOffset = 0;
	auto offsets = msgpack_decode(masterMemoryDB, dataOffset);
	
	// 提取数据
	cout << "baseOffset = " << dataOffset << endl;
	Json::Value::Members members = offsets.getMemberNames();
	for (auto it = members.begin(); it != members.end(); it++) {
		cout << (*it) << ": Offset = " << offsets[*it][0] << ", len = " << offsets[*it][1] << endl;
		auto res = msgpack_decode_raw(masterMemoryDB.substr(dataOffset + offsets[*it][0].asInt(), offsets[*it][1].asInt()));
		master[*it] = json_decode(res);
	}

    // 数据转换
    // 读取必要头文件
    fin.open("il2cpp.h");
    bool inStruct = false; string structName = "";
    while (!fin.eof()) {
        string s; getline(fin, s);
        while (s.back() == '\n' || s.back() == '\r') s.pop_back();
        if (!inStruct) if (s.find("struct ") != string::npos) {
            inStruct = true, structName = s.substr(s.find("struct ") + 7);
            structName.pop_back(); structName.pop_back();
            master_structure[structName] = vector<string>(); 
            cout << "Imported Struct " << structName << endl;
            goto end; 
        } if (s == "};") inStruct = false, structName = "";
        if (inStruct) master_structure[structName].push_back(s);
        end: continue;
    } fin.close();

    // 处理数据
    for (auto v : master) {
        string name = v.first;
        Json::Value data = v.second, res; res.resize(0);
        for (int i = 0; i < data.size(); i++) res.append(solveData(data[i], name));
        ofstream fout(dir + "/master/" + name + ".json");
        fout << json_pretty_encode(res);
        fout.close();
        cout << "Solved " << name << endl;
    }

    saveCurrentInfo(dir + "/version.json", currentInfo);

    /*auto res = getCatalog(argv[1]); string idx = "";
    for (int i = 0; i < res["m_InternalIdPrefixes"].size(); i++) {
        if (res["m_InternalIdPrefixes"][i].asString() == "Assets/AddressableAssets/Jacket") idx = to_string(i) + "#/";
    } if (idx == "") {
        cout << "[Error] No correct prefix found" << endl;
        return 1;
    } res = res["m_InternalIds"];
	auto res = masters["MusicMaster"]
    for (int i = 0; i < res.size(); i++) {
        if (res[i][0].asString().size() < idx.size()) continue;
        if (res[i].asString().substr(0, idx.size()) != idx) continue;
        string id = res[i].asString().substr(idx.size(), res[i].asString().find(".jpg") - 4);
        // 忽略无用 id (1017, 1021, 9999)
        if (id == "1017" || id == "1021" || id == "9999") continue;

        mkdir(dir + "/" + id); 
        if (!filesystem::exists(dir + "/" + id + "/music_config.txt") || filesystem::file_size(dir + "/" + id + "/music_config.txt") == 0) {
            // 下载 music_config
            string c = getMusicConfig(id);
            ofstream fout((dir + "/" + id + "/music_config.enc").c_str());
            for (int i = 0; i < c.size(); i++) fout << c[i]; fout.close();
            // 解密 music_config
            string cmd = "./decryptor/main info " + dir + "/" + id + "/music_config.enc " + dir + "/" + id + "/music_config.txt";
            // cout << "Decoding " << dir << "/" << id << "/music_config.enc" << endl;
            cout << cmd << endl;
            system(cmd.c_str());
        }

        for (int i = 1; i <= 5; i++) {
            if (!filesystem::exists(dir + "/" + id + "/" + to_string(i) + ".txt") || filesystem::file_size(dir + "/" + id + "/" + to_string(i) + ".txt") == 0) {
                // 下载 music_chart
                string d = getChart(id, i);
                ofstream fout((dir + "/" + id + "/" + to_string(i) + ".enc").c_str());
                for (int j = 0; j < d.size(); j++) fout << d[j]; fout.close();
                // 解密 music_chart
                string cmd = "./decryptor/main chart " + dir + "/" + id + "/" + to_string(i) + ".enc " + dir + "/" + id + "/" + to_string(i) + ".txt";
                // cout << "Decoding " << dir << "/" << id << "/" << to_string(i) + ".enc" << endl;
                cout << cmd << endl;
                system(cmd.c_str());
            }
        }

        if (!filesystem::exists(dir + "/" + id + "/music.mp3") || filesystem::file_size(dir + "/" + id + "/music.mp3") == 0) {
            // 下载 Music
            string c = getMusic(argv[1], id);
            ofstream fout((dir + "/" + id + "/music.acb").c_str());
            for (int i = 0; i < c.size(); i++) fout << c[i]; fout.close();
            // 解包 Music
            string cmd = "./acb2hca/main " + dir + "/" + id + "/music.acb " + dir + "/" + id + "/music.hca";
            // cout << "Converting " << dir << "/" << id << "/music.acb to " << dir << "/" << id << "/music.hca" << endl;
            cout << cmd << endl;
            system(cmd.c_str());
            cmd = "./hcaDecoder/HCADecoder " + dir + "/" + id + "/music.hca";
            cout << cmd << endl;
            system(cmd.c_str());
            cmd = "ffmpeg -i " + dir + "/" + id + "/music.wav " + dir + "/" + id + "/music.mp3";
            cout << cmd << endl;
            system(cmd.c_str());
        }

        if (!filesystem::exists(dir + "/" + id + "/musicpreview.mp3") || filesystem::file_size(dir + "/" + id + "/musicpreview.mp3") == 0) {
            // 下载 MusicPreview
            string c = getMusicPreview(argv[1], id);
            ofstream fout((dir + "/" + id + "/musicpreview.acb").c_str());
            for (int i = 0; i < c.size(); i++) fout << c[i]; fout.close();
            // 解包 MusicPreview
            string cmd = "./acb2hca/main " + dir + "/" + id + "/musicpreview.acb " + dir + "/" + id + "/musicpreview.hca";
            // cout << "Converting " << dir << "/" << id << "/musicpreview.acb to " << dir << "/" << id << "/musicpreview.hca" << endl;
            cout << cmd << endl;
            system(cmd.c_str());
            cmd = "./hcaDecoder/HCADecoder " + dir + "/" + id + "/musicpreview.hca";
            cout << cmd << endl;
            system(cmd.c_str());
            cmd = "ffmpeg -i " + dir + "/" + id + "/musicpreview.wav " + dir + "/" + id + "/musicpreview.mp3";
            cout << cmd << endl;
            system(cmd.c_str());
        }

        if (!filesystem::exists(dir + "/" + id + "/cover.png") || filesystem::file_size(dir + "/" + id + "/cover.png") == 0) {
            // 下载 Cover
            string c = getCover(argv[1], id);
            ofstream fout((dir + "/" + id + "/cover.bundle").c_str());
            for (int i = 0; i < c.size(); i++) fout << c[i]; fout.close();
            // 解包 Cover
            string cmd = "python3 ./bundleUnpack/unpack.py " + dir + "/" + id + "/cover.bundle " + dir + "/" + id + "/cover.png";
            // cout << "Unpacking " << dir << "/" << id << "/cover.bundle" << endl;
            cout << cmd << endl;
            system(cmd.c_str());
        }

        if (!filesystem::exists(dir + "/" + id + "/background.png") || filesystem::file_size(dir + "/" + id + "/background.png") == 0) {
            // 合成 background.png
            string cmd = "convert -size 1920x1180 xc:none -draw \"image Over 730,168,450,450 " + dir + "/" + id + "/cover.png\" -draw \"image Over 0,0,1920,1180 background.png\" " + dir + "/" + id + "/background.png";
            // cout << "Composing " << dir << "/" << id << "/backgroung.png" << endl;
            cout << cmd << endl;
            system(cmd.c_str());
        }

        // 清除不必要的内容
        string cmd = "rm " + dir + "/" + id + "/*.enc > /dev/null 2>&1";
        cout << cmd << endl;
        system(cmd.c_str());
        cmd = "rm " + dir + "/" + id + "/*.bundle > /dev/null 2>&1";
        cout << cmd << endl;
        system(cmd.c_str());
        cmd = "rm " + dir + "/" + id + "/*.acb > /dev/null 2>&1";
        cout << cmd << endl;
        system(cmd.c_str());
        cmd = "rm " + dir + "/" + id + "/*.hca > /dev/null 2>&1";
        cout << cmd << endl;
        system(cmd.c_str());
        cmd = "rm " + dir + "/" + id + "/*.wav > /dev/null 2>&1";
        cout << cmd << endl;
        system(cmd.c_str());
        
    } */
	cout << "Sync Finished" << endl;
}
