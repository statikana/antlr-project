clang-format -i main.cpp
find ./koda_src -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
