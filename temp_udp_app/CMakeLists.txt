cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include directories
target_include_directories(app PRIVATE
    modules/sht3xd_reader
    modules/udp_client
)

target_sources(app PRIVATE
    src/main.cpp
    modules/sht3xd_reader/sht3xd_reader.cpp
    modules/udp_client/udp_client.cpp
)
