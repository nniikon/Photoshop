project(plugin_line VERSION 0.1)

add_library(plugin_line SHARED ps_plugin_line.cpp)

target_link_libraries(plugin_line PRIVATE logger api api_implementation sfm_implementation parse_sprite)
target_include_directories(plugin_line PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
