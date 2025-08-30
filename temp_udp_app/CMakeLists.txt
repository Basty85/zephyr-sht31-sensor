cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(app PRIVATE modules/sht3xd_reader)

target_sources(app PRIVATE
    src/main.cpp
    modules/sht3xd_reader/sht3xd_reader.cpp
)
