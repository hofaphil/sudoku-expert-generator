#ifndef GENERATOR_SUDOKU_H
#define GENERATOR_SUDOKU_H

#include "Block.h"
#include "Number.h"

typedef struct sudoku
{
    block blocks[9];
    block solution[9];
} sudoku;

sudoku *new_sudoku(void);

int create(sudoku *sudoku, int difficulty);

int solve(sudoku *sudoku);

void set_solution(sudoku *sudoku, block blocks[]);

void set_sudoku(sudoku *sudoku, block blocks[]);

void print_blocks(block *blocks);

#endif
