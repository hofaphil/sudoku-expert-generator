cmake_minimum_required(VERSION 3.15)

project(Generator C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

enable_testing()

add_subdirectory(test)
