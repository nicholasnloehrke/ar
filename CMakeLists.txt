cmake_minimum_required(VERSION 3.18.0)

project(ar)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(ar
    src/main.c
)
