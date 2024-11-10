project(plugin_eraser VERSION 0.1)

add_library(plugin_eraser SHARED ps_plugin_eraser.cpp)

target_link_libraries(plugin_eraser PRIVATE logger api api_implementation sfm_implementation parse_sprite)
target_include_directories(plugin_eraser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
