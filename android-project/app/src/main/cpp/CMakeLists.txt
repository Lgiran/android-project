cmake_minimum_required(VERSION 3.10.2)

project("main")

add_library(main SHARED main.cpp)

find_library(log-lib log)

target_link_libraries(main
    SDL2 SDL2_image SDL2_mixer SDL2_ttf
    GLESv1_CM GLESv2
    ${log-lib})
