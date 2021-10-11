cmake_minimum_required(VERSION 3.20)
project(timedmutex)
include_directories(../include)
set(CMAKE_CXX_STANDARD 17)

add_executable(timedmutex main.cpp)
target_link_libraries(timedmutex pthread)