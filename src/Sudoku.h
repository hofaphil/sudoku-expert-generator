#ifndef GENERATOR_SUDOKU_H
#define GENERATOR_SUDOKU_H

typedef struct sudoku
{
    int blocks[9][3][3];    // the sudoku, 0 = empty field
    int solution[9][3][3];  // the filled out solution
} sudoku;

sudoku *new_sudoku(void);

int create(sudoku *s, int free_fields);

int solve(sudoku *s);

#endif
