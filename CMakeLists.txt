cmake_minimum_required(VERSION 3.5)

project(Smallclock LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(
    "config.in"
    "config.h"
)

add_subdirectory(Smallclock_Src)
