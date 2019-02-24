cmake_minimum_required(VERSION 3.13)
project(APICulture C)
include(FindGTK2)
if (GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
    link_directories(${GTK2_LIBRARY_DIRS})
    link_libraries(${GTK2_LIBRARIES})
endif (GTK2_FOUND)
set(CMAKE_C_STANDARD 99)

link_directories(curl/curl.h)
add_executable(APICulture main.c main.h cJSON/cJSON.h)
target_link_libraries(APICulture curl)


