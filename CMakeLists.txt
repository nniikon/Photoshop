cmake_minimum_required(VERSION 3.10)

project(ps VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -Wall -O2 \
    -Wmissing-declarations \
    -Wcast-align \
    -Wcast-qual \
    -Wchar-subscripts \
    -Wconversion \
    -Wempty-body \
    -Wfloat-equal \
    -Wformat-nonliteral \
    -Wformat-security \
    -Wformat=2 \
    -Winline \
    -Wpacked \
    -Wpointer-arith \
    -Winit-self \
    -Wredundant-decls \
    -Wshadow \
    -Wstrict-overflow=2 \
    -Wswitch-default \
    -Wswitch-enum \
    -Wundef \
    -Wunreachable-code \
    -Wunused \
    -Wvariadic-macros \
    -Wno-nonnull \
    -Wno-missing-field-initializers \
    -Wno-narrowing \
    -Wno-varargs \
    -Wstack-protector \
    -fcheck-new \
    -fstack-protector \
    -fstrict-overflow \
    -fno-omit-frame-pointer -fPIC"
)

include_directories(${CMAKE_SOURCE_DIR}/sfm_implementation)
include_directories(${CMAKE_SOURCE_DIR}/api_implementation)
include_directories(${CMAKE_SOURCE_DIR}/photoshopAPI)
include_directories(${CMAKE_SOURCE_DIR}/logger)
include_directories(${CMAKE_SOURCE_DIR}/system_plugins)

add_subdirectory(logger)
add_subdirectory(sfm_implementation)
add_subdirectory(api_implementation)
add_subdirectory(photoshopAPI)
add_subdirectory(system_plugins)

add_executable(ps
    photoshop/source/main.cpp
)

target_link_libraries(ps PUBLIC plugin_toolbar api_implementation sfm_implementation api logger)
