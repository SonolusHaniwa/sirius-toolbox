#include<bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input> <output>" << endl;
        return 1;
    }
    ifstream fin(argv[1], ios::binary); fin.seekg(0, ios::end);
    int len = fin.tellg(); fin.seekg(0, ios::beg);
    char* buf = new char[len]; fin.read(buf, len); fin.close();
    ofstream fout(argv[2], ios::binary);
    for (int i = 4096; i < len; i++) fout << buf[i];
    fout.close();
}