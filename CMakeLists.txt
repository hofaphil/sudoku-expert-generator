cmake_minimum_required(VERSION 3.15)
project(Generator C)

set(CMAKE_C_STANDARD 99)

add_library(Generator Sudoku.c Block.c Number.c Block.h Number.h Sudoku.h)
