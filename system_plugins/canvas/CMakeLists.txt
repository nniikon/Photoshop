project(plugin_canvas VERSION 0.1)

add_library(plugin_canvas SHARED ps_plugin_canvas.cpp)

target_link_libraries(plugin_canvas PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
