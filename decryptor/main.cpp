#include<bits/stdc++.h>
#include<openssl/aes.h>
#include<brotli/decode.h>
using namespace std;
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
class file_not_found: public exception {
    virtual const char* what() const throw() {
        return "No such file or directory.";
    }
};
int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <info/chart> [input] [output]" << endl;
        return 1;
    }
    string info_key = {};
    string chart_key = {};
    ifstream fin(argv[2], ios::binary); fin.seekg(0, ios::end);
    int len = fin.tellg(); fin.seekg(0, ios::beg);
    if (len == -1) throw file_not_found();
    char* buffer = new char[len]; fin.read(buffer, len); fin.close();
    string source = "";
    for (int i = 0; i < len; i++) source.push_back(buffer[i]); delete[] buffer;
    string result = aes_256_cbc_decryption(source, (argv[1][0] == 'i' ? info_key : chart_key), source.substr(0, 16));
    result = result.substr(16);
    if (argv[1][0] != 'i') result = brotli_decode(result);
    ofstream fout(argv[3]);
    char* out = new char[result.size()];
    for (int i = 0; i < result.size(); i++) out[i] = result[i];
    fout.write(out, result.size());
    fout.close();
}
