cmake_minimum_required(VERSION 3.13)
project(APICulture C)

set(CMAKE_C_STANDARD 99)

link_directories(curl/curl.h jsmn/jsmn.h)
add_executable(APICulture main.c main.h jsmn/jsmn.h)
target_link_libraries(APICulture curl jsmn)

