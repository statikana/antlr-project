#!/bin/bash

# clear current generated files
# rm -rf ./gen_cpp/*

# generate cpp target code
java -jar /usr/local/lib/antlr-4.13.2-complete.jar -Dlanguage=Cpp -o ./gen_cpp/ -visitor ./Koda.g4
echo "C++ Generated"

# only need to cmake once
cmake -S . -B ./build

make --directory=./build -j$(nproc)