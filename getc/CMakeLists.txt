# CMakeList.txt: getc 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.18)

project ("getc")
include_directories(../include)

aux_source_directory(../lib src)
# 将源代码添加到此项目的可执行文件。
add_executable (getc "getc.cpp" "getc.h" ${src})

# TODO: 如有需要，请添加测试并安装目标。
