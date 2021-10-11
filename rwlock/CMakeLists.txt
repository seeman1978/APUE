cmake_minimum_required(VERSION 3.20)
project(rwlock)

set(CMAKE_CXX_STANDARD 17)

add_executable(rwlock main.cpp)
target_link_libraries(rwlock pthread)
