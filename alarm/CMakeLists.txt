
cmake_minimum_required(VERSION 3.20)
project(alarm)
include_directories(../include)
set(CMAKE_CXX_STANDARD 17)
aux_source_directory(../lib src)
add_executable(alarm main.cpp ${src})
