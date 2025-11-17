clang-format -i main.cpp
find ./src -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
