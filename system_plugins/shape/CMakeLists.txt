project(plugin_shape VERSION 0.1)

add_library(plugin_shape SHARED ps_plugin_shape.cpp)

target_link_libraries(plugin_shape PRIVATE logger api api_implementation sfm_implementation parse_sprite)
target_include_directories(plugin_shape PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
