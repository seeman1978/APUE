cmake_minimum_required(VERSION 3.20)
project(atexit)
include_directories(../include)
set(CMAKE_CXX_STANDARD 14)
aux_source_directory(../lib src)
add_executable(atexit main.cpp ${src})
