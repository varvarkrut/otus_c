#!/bin/bash

# Установить требуемую версию Curl
CURL_VERSION="curl-$(curl -s https://api.github.com/repos/curl/curl/releases/latest | grep 'tag_name' | cut -d '"' -f 4)"

# Установить путь для скачивания и распаковки Curl
CURL_DIR="/tmp/curl"

mkdir -p "$CURL_DIR"
cd "$CURL_DIR" || exit 1

curl -L "https://github.com/curl/curl/releases/download/$CURL_VERSION/$CURL_VERSION.tar.gz" -o "$CURL_DIR/$CURL_VERSION.tar.gz"
tar -xf "$CURL_DIR/$CURL_VERSION.tar.gz"

cd "$CURL_DIR/$CURL_VERSION" || exit 1

./configure --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --disable-mqtt --disable-manual --with-openssl

make
sudo make install

rm -rf "$CURL_DIR"

echo "Curl успешно установлен."