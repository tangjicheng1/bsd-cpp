cmake_minimum_required(VERSION 3.20)

project(bsd-cpp LANGUAGES CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -Wextra -std=c++20 ")
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

message(STATUS "System Name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "System Version: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "System Processor: ${CMAKE_SYSTEM_PROCESSOR}")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

add_executable(main ${SRC_DIR}/main.cpp)

add_executable(client ${SRC_DIR}/client.cpp)
add_executable(server ${SRC_DIR}/server.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    execute_process(COMMAND uname -r
                    OUTPUT_VARIABLE FreeBSD_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "FreeBSD version: ${FreeBSD_VERSION}")
endif()

add_executable(client1 ${SRC_DIR}/client1.cpp)
add_executable(select1 ${SRC_DIR}/select1.cpp)
