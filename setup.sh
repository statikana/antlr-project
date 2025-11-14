#!/bin/bash

# basically just everything i did to get it up and running
# do NOT do `apt install antlr4`, it actually installs antlr3 ???

# apt packages
sudo apt install build-essential cmake uuid-dev

# install antlr tool (is a java binary)
curl -L -O https://www.antlr.org/download/antlr-4.13.2-complete.jar
mv ./antlr-4.13.2-complete.jar /usr/local/lib/

# install cpp runtime
curl -L -O https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip
unzip antlr4-cpp-runtime-4.13.2-source.zip -d antlr4-cpp-runtime
rm antlr4-cpp-runtime-4.13.2-source.zip

mkdir gen_cpp/          # for actually running the grammar
mkdir gen_java/         # for testrig debugging

touch Koda.g4           # antlr grammar file
touch KodaTokens.g4
touch main.cpp          # tree walkers
touch input.txt     # input file

# run.sh:   generate lexer & parser grammar files in ./gen_cpp
# alias.sh: make an alias grun='java -cp .:./gen_java:/usr/local/lib/antlr-4.13.2-complete.jar org.antlr.v4.gui.TestRig]'
# debug.sh: same as run.sh, but generates & compiles java instead of cpp so testrig ("grun") can target it for debugging
# gen_java is only for debug.sh / testrig / debugging