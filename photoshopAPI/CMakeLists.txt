cmake_minimum_required(VERSION 3.10)

project(api VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_library(api STATIC
    api_system.cpp
)

target_include_directories(api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
