cmake_minimum_required(VERSION 3.10)

project(ps VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -Wall \
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
    -fno-omit-frame-pointer \
    -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr"
)

include_directories(${CMAKE_SOURCE_DIR}/sfm_implementation)
include_directories(${CMAKE_SOURCE_DIR}/photoshopAPI)
include_directories(${CMAKE_SOURCE_DIR}/logger)

add_subdirectory(logger)
add_subdirectory(sfm_implementation)
add_subdirectory(photoshopAPI)

add_executable(ps
    photoshop/source/main.cpp
)

target_link_libraries(ps PUBLIC sfm_implementation api logger)
