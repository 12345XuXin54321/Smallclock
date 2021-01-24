cmake_minimum_required(VERSION 3.5)

project(Smallclock LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "-Wl,--dynamic-linker=./ld-linux-x86-64.so.2 -Wl,-rpath=$ORIGIN -Wl,-rpath=./")   #生成可执行文件

configure_file(
    "config.in"
    "config.h"
)

add_subdirectory(Smallclock_Src)
