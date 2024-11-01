cmake_minimum_required(VERSION 3.10)

project(plugin_toolbar VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

project(plugin_brush VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

project(plugin_circle VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

project(plugin_rectangle VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

project(plugin_line VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

project(plugin_canvas VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

project(plugin_eraser VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_library(plugin_circle SHARED
    ps_plugin_circle.cpp
)

add_library(plugin_toolbar SHARED
    ps_plugin_toolbar.cpp
)

add_library(plugin_rectangle SHARED
    ps_plugin_rectangle.cpp
)

add_library(plugin_line SHARED
    ps_plugin_line.cpp
)

add_library(plugin_brush SHARED
    ps_plugin_brush.cpp
)

add_library(plugin_canvas SHARED
    ps_plugin_canvas.cpp
)

add_library(plugin_eraser SHARED
    ps_plugin_eraser.cpp
)

target_link_libraries(plugin_toolbar PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_toolbar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plugin_circle PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_circle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plugin_rectangle PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_rectangle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plugin_line PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_line PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plugin_brush PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_brush PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plugin_canvas PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(plugin_eraser PRIVATE logger api api_implementation sfm_implementation)
target_include_directories(plugin_eraser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
