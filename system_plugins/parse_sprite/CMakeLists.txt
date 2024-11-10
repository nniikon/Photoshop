project(parse_sprite VERSION 0.1)

add_library(parse_sprite STATIC ps_parse_sprite.cpp)

target_link_libraries(parse_sprite PRIVATE logger sfm_implementation)
target_include_directories(parse_sprite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
