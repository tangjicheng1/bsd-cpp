cmake_minimum_required(VERSION 3.20)

project(fadan LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -std=c++20 ")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra ")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror ")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

add_executable(main ${SRC_DIR}/main.cpp)

add_executable(client ${SRC_DIR}/client.cpp)
add_executable(server ${SRC_DIR}/server.cpp)
