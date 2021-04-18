//
// Created by Philipp Hofer on 26.03.21.
//

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

int delete_numbers(int difficulty, block blocks[9]);

void init_block(block block[]);

int check_solutions(int index, int x, int z, int number, block holder_num[9]);

int solve(block blocks[9], int number, int block);

int generate(sudoku *sudoku, int number, int block);

block *get_solution(block sudoku[]);

void set_solution(sudoku *sudoku, block blocks[]);

void print_sudoku(sudoku *sudoku);

void print_blocks(block *blocks);

void shuffle(number *array, int n);

#endif
