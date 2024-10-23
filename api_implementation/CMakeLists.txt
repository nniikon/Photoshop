cmake_minimum_required(VERSION 3.10)

project(api_implementation VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_library(api_implementation STATIC
    ps_windows.cpp
    ps_canvas.cpp
)

target_include_directories(api_implementation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
