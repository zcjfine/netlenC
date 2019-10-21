cmake_minimum_required(VERSION 3.10)
project(netlenC C)

set(CMAKE_C_STANDARD 99)

add_executable(netlenC lib/common.h model-1/main.c model-1/tcp_server.c)