cmake_minimum_required(VERSION 3.17)
project(rbxl2scprp)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/external/lz4.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    include/external
)