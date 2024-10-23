cmake_minimum_required(VERSION 3.10)

project(plugin_toolbar VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_library(plugin_toolbar STATIC 
    ps_plugin_toolbar.cpp
)

target_link_libraries(plugin_toolbar PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_toolbar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
