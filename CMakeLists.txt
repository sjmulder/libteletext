cmake_minimum_required(VERSION 3.18.1)
include(FindPkgConfig)

project(libteletext C)

pkg_search_module(JSON_C REQUIRED json-c)

add_library(teletext SHARED
    libteletext/page.c
    libteletext/page_nos.c
    libteletext/util.c)

target_include_directories(teletext
    PUBLIC libteletext/include
    PUBLIC ${JSON_C_INCLUDE_DIRS})

target_link_libraries(teletext
    ${JSON_C_LINK_LIBRARIES})

add_executable(teletext-convert
    teletext-convert/convert.c)

target_include_directories(teletext-convert
    PUBLIC libteletext/include)

target_link_libraries(teletext-convert
    teletext)
