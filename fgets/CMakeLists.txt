﻿# CMakeList.txt: fgets 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
include_directories(../include)
project ("fgets")
aux_source_directory(../lib src)
# 将源代码添加到此项目的可执行文件。
add_executable (fgets "fgets.cpp" "fgets.h" ${src})

# TODO: 如有需要，请添加测试并安装目标。
