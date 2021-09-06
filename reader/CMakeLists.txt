cmake_minimum_required(VERSION 3.19)
project(reader C)

set(CMAKE_C_STANDARD 11)

add_executable(reader main.c)

target_link_libraries(reader pthread rt)