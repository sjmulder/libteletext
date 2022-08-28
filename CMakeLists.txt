cmake_minimum_required(VERSION 3.18.1)
include(FindPkgConfig)

project(libteletext C)

find_package(CURL REQUIRED)
pkg_search_module(JSON_C REQUIRED json-c)

# libteletext

add_library(libteletext SHARED
    libteletext/page.c
    libteletext/page_nos.c
    libteletext/util.c)

set_target_properties(libteletext PROPERTIES
    OUTPUT_NAME teletext)

target_include_directories(libteletext
    PUBLIC libteletext/include
    PUBLIC ${JSON_C_INCLUDE_DIRS}
    PUBLIC ${CURL_INCLUDE_DIRS})

target_link_libraries(libteletext
    ${JSON_C_LINK_LIBRARIES}
    ${CURL_LIBRARIES})

# teletext-convert

add_executable(teletext
    tools/teletext.c)

target_include_directories(teletext
    PUBLIC libteletext/include)

target_link_libraries(teletext
    libteletext)

# teletext-convert

add_executable(teletext-convert
    tools/teletext-convert.c)

target_include_directories(teletext-convert
    PUBLIC libteletext/include)

target_link_libraries(teletext-convert
    libteletext)
