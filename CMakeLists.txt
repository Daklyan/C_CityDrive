cmake_minimum_required(VERSION 3.13)
project(APICulture C)

set(CMAKE_C_STANDARD 99)

link_directories(curl/curl.h)
add_executable(APICulture main.c main.h cJSON/cJSON.h)
target_link_libraries(APICulture curl)

