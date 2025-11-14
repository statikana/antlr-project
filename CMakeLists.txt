cmake_minimum_required(VERSION 3.10)
project(antlr-project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the ANTLR runtime as a subdirectory
# this will build the runtime (antlr4_static)
add_subdirectory(antlr4-cpp-runtime)

# create executable
add_executable(antlr-program main.cpp)

# find all the generated .cpp files
file(GLOB gen_sources "gen_cpp/*.cpp")
target_sources(antlr-program PRIVATE ${gen_sources})

# link program against the ANTLR runtime and UUID
target_link_libraries(antlr-program PRIVATE 
    antlr4_static
    ${UUID_LIBRARIES}
)

# tell program where to find generated headers
# and ANTLR runtime headers
target_include_directories(
    antlr-program PRIVATE
    ./gen_cpp
    ./antlr4-cpp-runtime/runtime/src
)