cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(webserv ${SOURCES} main.cpp TCP_layer/tcp_connection.hpp TCP_layer/tcp_connection.cpp)