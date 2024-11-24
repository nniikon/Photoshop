project(plugin_filters VERSION 0.1)

add_library(plugin_filters SHARED ps_plugin_filters.cpp)

target_link_libraries(plugin_filters PRIVATE logger api api_implementation sfm_implementation parse_sprite plugin_secondary_panel)
target_include_directories(plugin_filters PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
