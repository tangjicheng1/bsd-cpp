cmake_minimum_required(VERSION 3.20)

project(bsd-cpp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

add_executable(main ${SRC_DIR}/main.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_executable(client ${SRC_DIR}/client.cpp)
    add_executable(server ${SRC_DIR}/server.cpp)
    add_executable(client1 ${SRC_DIR}/client1.cpp)
    add_executable(select1 ${SRC_DIR}/select1.cpp)

    add_executable(client2 ${SRC_DIR}/client2.cpp)
    target_link_libraries(client2 pthread)

    add_executable(p1 ${SRC_DIR}/p1.cpp)
    add_executable(p3 ${SRC_DIR}/p3.cpp)
    add_executable(worker ${SRC_DIR}/worker.cpp)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_executable(kqueue1 ${SRC_DIR}/kqueue1.cpp)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_executable(main2 ${SRC_DIR}/main.cpp)
endif()
