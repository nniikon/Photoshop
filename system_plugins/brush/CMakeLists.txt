project(plugin_brush VERSION 0.1)

add_library(plugin_brush SHARED ps_plugin_brush.cpp)

target_link_libraries(plugin_brush PRIVATE logger api api_implementation sfm_implementation plugin_secondary_panel)
target_include_directories(plugin_brush PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
