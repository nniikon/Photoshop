project(plugin_toolbar VERSION 0.1)

add_library(plugin_toolbar SHARED ps_plugin_toolbar.cpp)

target_link_libraries(plugin_toolbar PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_toolbar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
