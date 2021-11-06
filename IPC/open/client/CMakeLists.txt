cmake_minimum_required(VERSION 3.20)
project(open)

set(CMAKE_CXX_STANDARD 17)
include_directories(../../../include)
aux_source_directory(../../../lib src)
add_executable(open main.cpp open.cpp ${src})

