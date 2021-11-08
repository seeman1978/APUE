cmake_minimum_required(VERSION 3.20)
project(server2)
include_directories(../../../include)
set(CMAKE_CXX_STANDARD 20)
aux_source_directory(../../../lib src)
add_executable(server2 main.cpp client.cpp request.cpp cliargs.cpp ../../send_fd2.cpp ../../send_err.cpp
        ../../../UnixDomainSocket/serv_listen.cpp ../../../UnixDomainSocket/cli_conn.cpp
        ../../../UnixDomainSocket/serv_accept.cpp ${src})
