# CMakeList.txt: server2 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("server2")
include_directories(../../include)
set(CMAKE_CXX_STANDARD 20)
# 将源代码添加到此项目的可执行文件。
aux_source_directory(../../lib src)
add_executable (server2 server2.cpp ${src})

# TODO: 如有需要，请添加测试并安装目标。
