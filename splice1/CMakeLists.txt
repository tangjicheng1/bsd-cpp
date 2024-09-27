cmake_minimum_required(VERSION 3.20)

project(bsd-cpp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

add_executable(server server.cpp)
