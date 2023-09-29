#include<curl/curl.h>
#include<jsoncpp/json/json.h>
#include<msgpack.hpp>
#include<lz4.h>
#include<openssl/aes.h>
#include<brotli/decode.h>

// curl
size_t writeFunction(void* ptr, size_t size, size_t nmemb, string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}
size_t headerWriteFunction(void* ptr, size_t size, size_t nmemb, map<string, string>* data) {
    string header = (char*)ptr;
    int pos = header.find(": ");
    if (pos != string::npos) {
        string key = header.substr(0, pos);
        string value = header.substr(pos + 2, header.size() - pos - 4);
        (*data)[key] = value;
    } return size * nmemb;
}
string geturl(string url, vector<string> header, string data, bool post, map<string, string>& response, bool skipFailed = false, int retryTimes = 5) {
	string method = post ? "POST" : "GET";
    CURL* curl = curl_easy_init();
    if (curl) {
        cout << "Visiting " << method << " " << url << endl;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        if (header.size()) {
            struct curl_slist* headers = NULL;
            for (auto i: header) headers = curl_slist_append(headers, i.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
        if (post) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        }
        string result = "";
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerWriteFunction);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            if (retryTimes) {
                cout << curl_easy_strerror(res) << ", " << retryTimes << " times left." << endl;
                return geturl(url, header, data, post, response, skipFailed, retryTimes - 1);
            } curl_easy_cleanup(curl);
            throw runtime_error(curl_easy_strerror(res));
        } int code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
		if (code != 200 && !skipFailed) {
            if (retryTimes) {
                cout << "Visit " << method << " " << url << ": Failed with code " << code << ", " << retryTimes << " times left." << endl;
                return geturl(url, header, data, post, response, skipFailed, retryTimes - 1);
            } stringstream buffer;
			curl_easy_cleanup(curl);
			buffer << "Visit " << method << " " << url << ": Failed with code " << code << endl;
			buffer << "Info: " << result << endl;
			for (auto v : response) buffer << v.first << ": " << v.second << endl;
			throw runtime_error(buffer.str());
		}
        curl_easy_cleanup(curl);
        return result;
    }
    else throw runtime_error("curl init failed");
}

// json
string json_encode(Json::Value value) {
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["emitUTF8"] = true;
    unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    ostringstream os;
    writer->write(value, &os);
    return os.str();
}
string json_pretty_encode(Json::Value value) {
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    builder["emitUTF8"] = true;
    unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    ostringstream os;
    writer->write(value, &os);
    return os.str();
}
Json::Value json_decode(string source) {
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    JSONCPP_STRING errs;
    Json::Value root;
    unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(source.c_str(), source.c_str() + source.size(), &root, &errs)) throw runtime_error(errs);
    return root;
}

// lz4
string lz4_uncompress(string source) {
	const int len = 16 * 1024 * 1024;
	char* result = new char[len]; 
	auto res = LZ4_decompress_safe(source.c_str(), result, source.size(), len);
	if (res < 1) return cout << "Failed to decompress lz4 package!" << res << endl, "";
	string ret = "";
	for (int i = 0; i < res; i++) ret += result[i];
	return ret;
}

// msgpack
string str_replace(string from, string to, string source, bool supportTranfer = false) {
	string result = source;
	int st = 0, wh = result.find(from.c_str(), st);
	while (wh != string::npos) {
		if (supportTranfer && wh >= 1 && result[wh - 1] == '\\') {
			st = wh + 1;
			wh = result.find(from.c_str(), st);
			continue;
		} result.replace(wh, from.size(), to.c_str());
		st = wh + to.size();
		wh = result.find(from.c_str(), st);
	} return result;
}
int TimestampExtType = -1;
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
class visitor {
	public:
	
	string& m_s;
	visitor(string& m_s): m_s(m_s){}
	map<int, function<string(string)> > extensions;
	
	void extendWidth(int type, function<string(string)> resolver) { extensions[type] = resolver; }
	bool visit_nil() { m_s += "null"; return true; }
	bool visit_boolean(bool v) { m_s += v ? "true" : "false"; return true; }
	bool visit_positive_integer(uint64_t v) { stringstream ss; ss << v; m_s += ss.str(); return true; }
	bool visit_negative_integer(int64_t v) { stringstream ss; ss << v; m_s += ss.str(); return true; }
	bool visit_float32(float v) { stringstream ss; ss << v; m_s += ss.str(); return true; }
	bool visit_float64(double v) { stringstream ss; ss << v; m_s += ss.str(); return true; }
	bool visit_str(const char* v, uint32_t size) { m_s += '"' + str_replace("\"", "\\\"", string(v, size), false) + '"'; return true; }
	bool visit_ext(const char* v, uint32_t size) { 
		int type = v[0];
		if (extensions.find(type) != extensions.end()) {
			string s = "";
			for (int i = 1; i < size; i++) s.push_back(v[i]);
			m_s += '"' + str_replace("\"", "\\\"", extensions[type](s), false) + '"';
			return true;
		} stringstream ss; 
		for (int i = 1; i < size; i++) ss << hex << setw(2) << setfill('0') << int((unsigned char)v[i]);
		m_s += '"' + str_replace("\"", "\\\"", ss.str(), false) + '"';
		return true;
	}
	bool visit_bin(const char* v, uint32_t size) { 
		stringstream ss;
		for (int i = 0; i < size; i++) ss << hex << setw(2) << setfill('0') << int((unsigned char)v[i]);
		m_s += '"' + str_replace("\"", "\\\"", ss.str(), false) + '"';
		return true;
	}
	bool start_array(uint32_t num_elements) { m_s += "["; return true; }
	bool start_array_item() { return true; }
	bool end_array_item() { m_s += ","; return true; }
	bool end_array() { while(m_s.back() == ',') m_s.pop_back(); m_s += "]"; return true; }
	bool start_map(uint32_t num_kv_pairs) { m_s += "{"; return true; }
	bool start_map_key() { m_s += '"'; return true; }
	bool end_map_key() { m_s += "\":"; return true; }
	bool start_map_value() { return true; }
	bool end_map_value() { m_s += ","; return true; }
	bool end_map() { m_s.pop_back(); m_s += "}"; return true; }
	void parse_error(size_t parsed_offset, size_t error_offset) {}
	void insufficient_bytes(size_t parsed_offset, size_t error_offset) {}
};
Json::Value msgpack_decode(string data, int& off) {
	stringstream buffer; size_t offset = 0;
	buffer << msgpack::unpack(data.data(), data.size(), offset).get();
	off = offset;
	return json_decode(buffer.str());
}
string msgpack_decode_raw(string data) {
	string str = "";
	switch ((unsigned char)data[0]) {
		case 0xc7: str = data.substr(3); break;
		case 0xc8: str = data.substr(4); break;
		case 0xc9: str = data.substr(6); break;
		default: {
			string buffer = ""; visitor vis(buffer);
			vis.extendWidth(TimestampExtType, TimestampExtResolver);
			auto res = msgpack::parse(data.data(), data.size(), vis);
			return buffer;
		}
	} str = lz4_uncompress(str.substr(5));
	string buffer = ""; visitor vis(buffer);
	vis.extendWidth(TimestampExtType, TimestampExtResolver);
	if (str != "") auto res = msgpack::parse(str.data(), str.size(), vis);
	return buffer;
}

// aes
void hexout(string source) {
    cout << "{"; for (int i = 0; i < source.size(); i++) {
        if (i) cout << ", ";
        printf("0x%02x", (unsigned char)source[i]);
    } cout << "}" << endl;
    cout << endl;
}
string aes_256_cbc_decryption(string source, string key, string iv) {
    AES_KEY aes_key;
    AES_set_decrypt_key((const unsigned char*)key.c_str(), 256, &aes_key);
    unsigned char* out = new unsigned char[source.size()];
    memset(out, '\0', sizeof out);
    AES_cbc_encrypt((const unsigned char*)source.c_str(), out, source.size(), &aes_key, (unsigned char*)iv.c_str(), AES_DECRYPT);
    string result = "";
    for (int i = 0; i < source.size(); i++) result += out[i];
    delete[] out;
    // 舍弃填充数据
    int len = result.back(); bool lost = true;
    if (len > result.size()) return result;
    for (int i = result.size() - 1; i >= result.size() - len; i--) if (result[i] != len) lost = false;
    if (lost) for (int i = 1; i <= len; i++) result.pop_back();
    return result;
}

// brotli
class BrotliDecodeException : public std::runtime_error {
    using runtime_error::runtime_error;
};
string brotli_decode(string in) {
    unique_ptr<BrotliDecoderState, decltype(&BrotliDecoderDestroyInstance)> state(
        BrotliDecoderCreateInstance(nullptr, nullptr, nullptr), BrotliDecoderDestroyInstance);
    BrotliDecoderResult result = BROTLI_DECODER_RESULT_ERROR;
    string out = "";
    const size_t chunk_size = 1 << 16;
    vector<uint8_t> buffer(chunk_size, 0);
    size_t available_in = in.size();
    const uint8_t * next_in = reinterpret_cast<const uint8_t *>(in.data());
    size_t available_out = buffer.size();
    uint8_t * next_out = buffer.data();
    while (true) {
        result = BrotliDecoderDecompressStream(state.get(), &available_in, &next_in, &available_out, &next_out, 0);
        if (result == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT || result == BROTLI_DECODER_RESULT_SUCCESS) {
            out.append(buffer.data(), buffer.data() + distance(buffer.data(), next_out));
            if (result == BROTLI_DECODER_RESULT_SUCCESS) return out;
            available_out = buffer.size();
            next_out = buffer.data();
        }
        else if(result == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT) throw BrotliDecodeException("input corrupted");
        else {
            if (result == BROTLI_DECODER_RESULT_ERROR) {
                string detail = BrotliDecoderErrorString(BrotliDecoderGetErrorCode(state.get()));
                throw BrotliDecodeException("decoding failed (" + detail + ")");
            } throw BrotliDecodeException("decoding failed");
        }
    }
}

vector<string> default_header = {
    "Content-Type: application/json",
    "Accept: application/json",
    "X-Platform: google-play",
    "X-FM: 0",
    "Accept-Encoding: br, gzip, identity",
    "Connection: Keep-Alive, TE",
    "Keep-Alive: timeout=21",
    "TE: identity",
    "User-Agent: BestHTTP/2 v2.8.3",
};
string accountToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1aWQiOiI0MTM3MzU1IiwibmJmIjoxNjk0NTQ0OTUzLCJleHAiOjE2OTQ1NDg1NTMsImlhdCI6MTY5NDU0NDk1MywiaXNzIjoic2lyaXVzLmttczMuY29tIiwiYXVkIjoic2lyaXVzIn0.uU_Ive-dHSPz-fUoQ3df7w0S_ZlSdDCeksdUovei5wM";
string proxyUrl = "https://corsproxy.org/?";
map<string, string> cookie;
string authorization = "";
string apiBase = "";
string masterBase = "";
string assetsBase = "";
string appVersionFull = "";
string appVersion = "";
string assetsVersion = "";

vector<string> getHeader(vector<string> header = {}, bool useDefault = true) {
    vector<string> result = useDefault ? default_header : vector<string>();
    for (auto i: header) result.push_back(i);
    if (authorization != "") result.push_back("Authorization: Bearer " + authorization);
    string cookie_str = "";
    for (auto i: cookie) cookie_str += i.first + "=" + i.second + "; ";
    if (cookie_str != "") result.push_back("Cookie: " + cookie_str);
    return result;
}
map<string, string> getCookie(string cookie_str) {
    map<string, string> result;
    int pos = 0;
    while (pos < cookie_str.size()) {
        int pos2 = cookie_str.find("=", pos);
        if (pos2 == string::npos) break;
        string key = cookie_str.substr(pos, pos2 - pos);
        pos = pos2 + 1;
        pos2 = cookie_str.find("; ", pos);
        if (pos2 == string::npos) pos2 = cookie_str.size();
        string value = cookie_str.substr(pos, pos2 - pos);
        pos = pos2 + 2;
        result[key] = value;
    } return result;
}

void mkdir(string path) {
    filesystem::create_directories(path);
}

void saveCurrentInfo(string path, Json::Value info) {
    ofstream fout(path);
    fout << json_encode(info);
    fout.close();
}

void getCurrentInfo(Json::Value info) {
    authorization = info["account.authorization"].asString();
    apiBase = info["url.apiBase"].asString();
    masterBase = info["url.masterBase"].asString();
    assetsBase = info["url.assetsBase"].asString();
    appVersion = info["app.version"].asString(); appVersionFull = appVersion + ".76";
    assetsVersion = info["assets.version"].asString();
}

string readFile(string path) {
    ifstream fin(path);
    fin.seekg(0, ios::end);
    int size = fin.tellg();
    fin.seekg(0, ios::beg);
    if (size == -1) return "";
    char* buffer = new char[size];
    fin.read(buffer, size);
    fin.close();
    string result = "";
    for (int i = 0; i < size; i++) result += buffer[i];
    return result;
}

void writeFile(string path, string content) {
    ofstream fout(path);
    fout << content;
    fout.close();
}

bool fileExists(string path) {
    return filesystem::exists(path) && filesystem::file_size(path) > 0;
}