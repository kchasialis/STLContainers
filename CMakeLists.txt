cmake_minimum_required(VERSION 3.14)
project(STLContainers)

set(CMAKE_CXX_STANDARD 11)

include_directories(include/containers)
include_directories(include/internal)

add_executable(main main.cpp)
add_executable(benchmarks benchmarks.cpp)
