#pragma once

typedef unsigned char uc;
typedef long long ll;
map<int, function<Json::Value(string)> > extensions;
void extendWith(int type, function<Json::Value(string)> resolver) { extensions[type] = resolver; }
Json::Value msgpack_decode(string str, int& st) {
    // cout << st << endl;
    // if (st < 0) cout << st << endl, assert(false);
    Json::Value res; 
    // cout << "val: " << st << " " << hex << ll(str[st]) << endl;
    // null
    if ((uc)str[st] == 0xc0) res = Json::Value(), st++;
    // bool
    else if ((uc)str[st] == 0xc2) res = false, st++;
    else if ((uc)str[st] == 0xc3) res = true, st++;
    // ll
    else if ((uc)str[st] >= 0x00 && (uc)str[st] <= 0x7f) res = (uc)str[st], st++;
    else if ((uc)str[st] >= 0xe0 && (uc)str[st] <= 0xff) res = -1 * ((uc)str[st] & 0b00011111), st++;
    else if ((uc)str[st] >= 0xcc && (uc)str[st] <= 0xd3) {
        ll len = 1ll << ((uc)str[st] & 0b00000011), isSigned = (uc)str[st] >= 0xd0; long long num = 0; st++;
        for (ll i = 0; i < len; i++) num = (num << 8) + (uc)str[st + i];
        if (isSigned && num >= (1ll << (len * 8 - 1))) num -= (1ll << (len * 8));
        res = num; st += len;
    }
    // float
    else if ((uc)str[st] >= 0xca && (uc)str[st] <= 0xcb) {
        double num = 0; st++;
        for (ll i = 0; i < ((uc)(str[st - 1]) == 0xca ? 4 : 8); i++) num = (num * 256) + (uc)str[st + i];
        res = num; st += ((uc)(str[st - 1]) == 0xca ? 4 : 8);
    }
    // string
    else if ((uc)str[st] >= 0xa0 && (uc)str[st] <= 0xbf) {
        ll len = (uc)str[st] & 0b00011111; st++; // cout << st - 1 << " " << len << endl;
        string str2 = "";
        for (ll i = 0; i < len; i++) str2.push_back(str[st + i]);
        res = str2; st += len;
    }
    else if ((uc)str[st] >= 0xd9 && (uc)str[st] <= 0xdb) {
        ll sizlen = 1ll << ((uc)str[st] - 0xd9), len = 0; st++;
        for (ll i = 0; i < sizlen; i++) len = (len << 8) + (uc)str[st + i];
        string str2 = ""; st += sizlen;
        for (ll i = 0; i < len; i++) str2.push_back(str[st + i]);
        res = str2; st += len;
    }
    // bin
    else if ((uc)str[st] >= 0xc4 && (uc)str[st] <= 0xc6) {
        ll sizlen = 1ll << ((uc)str[st] - 0xc4), len = 0; st++;
        for (ll i = 0; i < sizlen; i++) len = (len << 8) + (uc)str[st + i];
        string str2 = ""; st += sizlen;
        for (ll i = 0; i < len; i++) str2.push_back(str[st + i]);
        res = str2; st += len;
    }
    // array
    else if ((uc)str[st] >= 0x90 && (uc)str[st] <= 0x9f) {
        // cout << st << endl;
        ll len = (uc)str[st] & 0b00001111; st++;
        Json::Value arr = Json::Value();
        for (ll i = 0; i < len; i++) arr.append(msgpack_decode(str, st));
        res = arr;
    }
    else if ((uc)str[st] >= 0xdc && (uc)str[st] <= 0xdd) {
        ll sizlen = 1ll << ((uc)str[st] - 0xdb), len = 0; st++;
        for (ll i = 0; i < sizlen; i++) len = (len << 8) + (uc)str[st + i];
        Json::Value arr = Json::Value(); st += sizlen; // cout << len << endl;
        for (ll i = 0; i < len; i++) {
            arr.append(msgpack_decode(str, st));
        } // cout << arr << endl;
        res = arr;
    }
    // map
    else if ((uc)str[st] >= 0x80 && (uc)str[st] <= 0x8f) {
        // cout << dec << st << endl;
        ll len = (uc)str[st] & 0b00001111; st++;
        Json::Value map = Json::Value();
        for (ll i = 0; i < len; i++) {
            Json::Value key = msgpack_decode(str, st);
            Json::Value val = msgpack_decode(str, st);
            map[key.asString()] = val;
        } res = map;
    }
    else if ((uc)str[st] >= 0xde && (uc)str[st] <= 0xdf) {
        ll sizlen = 1ll << ((uc)str[st] - 0xdd), len = 0; st++;
        for (ll i = 0; i < sizlen; i++) len = (len << 8) + (uc)str[st + i];
        Json::Value map = Json::Value(); st += sizlen;
        for (ll i = 0; i < len; i++) {
            Json::Value key = msgpack_decode(str, st);
            Json::Value val = msgpack_decode(str, st); 
            map[key.asString()] = val;
        } res = map;
    }
    // ext
    else if ((uc)str[st] >= 0xd4 && (uc)str[st] <= 0xd8) {
        ll len = 1ll << ((uc)str[st] - 0xd4), type = (uc)str[st + 1]; st += 2;
        string str2 = "";
        for (ll i = 0; i < len; i++) str2.push_back(str[st + i]);
        res = str2; st += len;
        if (extensions.find(type) != extensions.end()) res = extensions[type](str2);
    }
    else if ((uc)str[st] >= 0xc7 && (uc)str[st] <= 0xc9) {
        ll sizlen = 1ll << ((uc)str[st] - 0xc7), len = 0; st++;
        for (ll i = 0; i < sizlen; i++) len = (len << 8) + (uc)str[st + i];
        string str2 = ""; st += sizlen;
        ll type = (uc)str[st]; st++;
        for (ll i = 0; i < len; i++) str2.push_back(str[st + i]);
        res = str2; st += len;
        if (extensions.find(type) != extensions.end()) res = extensions[type](str2);
    }
    return res;
}

int TimestampExtType = 0xff;
string getTime(int64_t sec) {
	stringstream ss;
	ss << put_time(localtime(&sec), "%Y-%m-%d %H:%M:%S");
	return ss.str();
}
auto TimestampExtResolver = [](string s){
	unsigned char* ch = new unsigned char[s.size()];
	for (int i = 0; i < s.size(); i++) ch[i] = s[i];
	int64_t sec = 0; int64_t nsec = 0;
	switch (s.size()) {
		case 4: {
			for (int i = 0; i < 4; i++) sec <<= 8, sec |= ch[i];
			return getTime(sec);
		} break;
		case 8: {
			for (int i = 0; i < 8; i++) sec <<= 8, sec |= ch[i];
			nsec = sec >> 34; nsec += nsec < 0 ? 0x40000000 : 0;
			sec &= 0x3ffffffff;
			return getTime(sec);
		} break;
		case 12: {
			for (int i = 0; i < 4; i++) nsec <<= 8, nsec |= ch[i];
			for (int i = 4; i < 12; i++) sec <<= 8, sec |= ch[i];
			return getTime(sec);
		} break;
		default: return string("[Timestamp]"); break;
	} return string("[Timestamp]");
};

int Lz4BlockExtType = 0x63;
auto Lz4BlockExtResolver = [](string s){
    string str = lz4_uncompress(s.substr(5));
    // for (int i = 0; i < str.size(); i++) cout << dec << i << " " << hex << int(uc(str[i])) << " " << str[i] << endl; cout << dec << 114514 << endl;
    int st = 0;
    return msgpack_decode(str, st);
};

int Lz4BlockArrayExtType = 0x62;
auto Lz4BlockArrayExtResolver = [](string s){
    assert(false);
    return "[Ext]";
};

auto msgpackExtLoader = [](){
    extendWith(TimestampExtType, TimestampExtResolver);
    extendWith(Lz4BlockExtType, Lz4BlockExtResolver);
    extendWith(Lz4BlockArrayExtType, Lz4BlockArrayExtResolver);
    return true;
}();