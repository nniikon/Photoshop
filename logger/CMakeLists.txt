cmake_minimum_required(VERSION 3.10)

project(logger VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_library(logger STATIC
    loguru.cpp
)

target_link_libraries(logger PRIVATE pthread dl)
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
