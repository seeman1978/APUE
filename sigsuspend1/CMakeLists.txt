cmake_minimum_required(VERSION 3.20)
project(sigsuspend1)
include_directories(../include)
set(CMAKE_CXX_STANDARD 20)
aux_source_directory(../lib src)
add_executable(sigsuspend1 main.cpp ${src})
