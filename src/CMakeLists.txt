add_library(
        generator
        Sudoku.c Block.c Number.c
        Sudoku.h Block.h Number.h
)

add_executable(generator-exe main.c
        Sudoku.c Block.c Number.c
        Sudoku.h Block.h Number.h)