cmake_minimum_required(VERSION 3.18.1)

project(libteletext C)

add_library(teletext
        libteletext/teletext.c
        libteletext/util.c
        libteletext/nos.c)

target_include_directories(teletext
        PUBLIC libteletext/include)

add_executable(teletext-convert
        teletext-convert/convert.c)

target_link_libraries(teletext-convert
        teletext)

target_include_directories(teletext-convert
        PUBLIC libteletext/include)
