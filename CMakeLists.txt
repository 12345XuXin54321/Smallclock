cmake_minimum_required(VERSION 3.5)

project(Smallclock LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-no-pie")   #生成可执行文件
set(MAKEFLAGS "-j4")             #多核编译

configure_file(
    "Smallclock_version.h.in"
    "Smallclock_version.h"
)

add_subdirectory(Smallclock_Src)
