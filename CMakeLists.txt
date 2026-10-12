cmake_minimum_required(VERSION 3.16)

project(Generator C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

enable_testing()

add_subdirectory(test)
