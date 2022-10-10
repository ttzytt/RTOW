cmake_minimum_required(VERSION 3.20)
# set(CMAKE_CXX_COMPILER "/usr/bin/g++-12")
# set(CMKAE_C_COMPILER "/usr/bin/gcc-12")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_compile_options(-Wall -O3 -march=native)
add_definitions(-D OUT -D AVX2)

project(ttzytt_RT)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} src)