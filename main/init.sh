#!/bin/bash

# Author:       Noah Ostle
# Description:  A bash script that will auomatically make the executables, 
#               convert them to shellcode using donut, start the node C2
#               server, and present you with the stager executable.



cleanup() {
echo "Cleaning up..."

make -C src clean

rm loader.bin
#rm /shellcode/loaders/* -f
}
trap cleanup SIGINT


echo -e "Checking Dependencies...\n"
pip install -r requirements.txt

make -C src "test"

python3 shellcode/exetoshellcode.py

node C2server/server.js


