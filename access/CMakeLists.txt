cmake_minimum_required(VERSION 3.20)
project(access)
include_directories(../include)
set(CMAKE_CXX_STANDARD 14)
aux_source_directory(../lib src)
add_executable(access main.cpp ${src})
