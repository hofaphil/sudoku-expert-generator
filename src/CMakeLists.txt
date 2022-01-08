include_directories(../include)

add_library(generator Sudoku.c Block.c Number.c)
add_executable(generator-exec main.c)

target_include_directories(generator PUBLIC ../include)