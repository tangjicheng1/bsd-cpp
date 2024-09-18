cmake_minimum_required(VERSION 3.20)

project(nanobind_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc ")

find_package(Python 3.8...<4.0 COMPONENTS Interpreter Development.Module REQUIRED)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE 
  OUTPUT_VARIABLE nanobind_ROOT
)

find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(demo1 demo1.cpp)
nanobind_add_module(demo2 demo2.cpp)
nanobind_add_module(demo3 demo3.cpp)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -c "import site; print(site.getsitepackages()[0])"
  OUTPUT_STRIP_TRAILING_WHITESPACE 
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
)

install(TARGETS demo3
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
