# Master Data Downloader

## Install Dependencies

```bash
sudo apt install g++ libjsoncpp-dev openssl libbrotli-dev libcurl4-openssl-dev libzip-dev libmysqlclient-dev libsqlite3-dev imagemagick ffmpeg libmsgpack-dev liblz4-dev -y
```

## Compile Program

```bash
g++ masterData.cpp -o masterData -ljsoncpp -lcurl -lmsgpackc -lm -llz4 -lcrypto -lbrotlidec --std=c++17 -w -g
```

## Usage

```bash
./masterData [dir]
```
