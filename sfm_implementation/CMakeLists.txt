cmake_minimum_required(VERSION 3.10)

project(sfm_implementation VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_library(sfm_implementation STATIC
    sfm_implementation.cpp
)

target_link_libraries(sfm_implementation PUBLIC api)
target_link_libraries(sfm_implementation PRIVATE sfml-system sfml-window sfml-graphics)
target_include_directories(sfm_implementation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
