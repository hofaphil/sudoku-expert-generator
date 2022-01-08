#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Sudoku.h"

void shuffle(number *array, int n);

int delete_numbers(int difficulty, block blocks[9]);

void init_block(block block[]);

int check_solutions(int index, int x, int z, int number, block holder_num[9]);

int solve(block blocks[9], int number, int block);

int generate(sudoku *sudoku, int number, int block);

sudoku *new_sudoku()
{
    sudoku *s = malloc(sizeof(sudoku));
    init_block(s->blocks);
    srand(time(NULL));
    return s;
}

int create(sudoku *sudoku, int difficulty)
{
    generate_random(&sudoku->blocks[0]);
    generate_random(&sudoku->blocks[4]);
    generate_random(&sudoku->blocks[8]);

    generate(sudoku, 1, 1);

    for (int i = 0; i < 9; i++)
        sudoku->solution[i] = new_block_copy(sudoku->blocks[i]);

    return delete_numbers(difficulty, sudoku->blocks);
}

int delete_numbers(int difficulty, block blocks[9])
{
    int b = 0;
    int diff = difficulty * 7 + 40;

    number numbers[81];
    int array_index = 0;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                numbers[array_index++] = new_number(i, blocks[i].numbers[j][k]);
    shuffle(numbers, 81);

    array_index = 0;

    while (b < diff && array_index < 81) {
        int index = numbers[array_index].block;
        int number = numbers[array_index++].number;

        block holder = new_block_copy(blocks[index]);
        if (delete(&blocks[index], number)) {
            b++;

            if (!check_solutions(index, blocks[index].latest_del_index_x, blocks[index].latest_del_index_z, number,
                                 blocks)) {
                set_numbers(&blocks[index], holder.numbers);
                b--;
            }
        }
    }
    return b;
}

void init_block(block block[9])
{
    for (int i = 0; i < 9; i++)
        block[i] = new_block();

    set_row_partner(&block[1], &block[2], &block[0]);
    set_column_partner(&block[3], &block[6], &block[0]);

    set_row_partner(&block[0], &block[2], &block[1]);
    set_column_partner(&block[4], &block[7], &block[1]);

    set_row_partner(&block[0], &block[1], &block[2]);
    set_column_partner(&block[5], &block[8], &block[2]);

    set_row_partner(&block[4], &block[5], &block[3]);
    set_column_partner(&block[0], &block[6], &block[3]);

    set_row_partner(&block[3], &block[5], &block[4]);
    set_column_partner(&block[1], &block[7], &block[4]);

    set_row_partner(&block[3], &block[4], &block[5]);
    set_column_partner(&block[2], &block[8], &block[5]);

    set_row_partner(&block[7], &block[8], &block[6]);
    set_column_partner(&block[0], &block[3], &block[6]);

    set_row_partner(&block[6], &block[8], &block[7]);
    set_column_partner(&block[1], &block[4], &block[7]);

    set_row_partner(&block[6], &block[7], &block[8]);
    set_column_partner(&block[2], &block[5], &block[8]);
}

int check_solutions(int index, int x, int z, int number, block holder_num[9])
{
    block holder[9];
    for (int i = 0; i < 9; i++) {
        holder[i] = new_block_copy(holder_num[i]);
    }

    for (int j = 1; j < 10; j++) {
        if (j == number)
            continue;

        if (insert(&holder_num[index], j, x, z)) {
            if (solve(holder_num, 1, 0)) {
                for (int i = 0; i < 9; i++)
                    set_numbers(&holder_num[i], holder[i].numbers);
                return 0;
            }
            set_numbers(&holder_num[index], holder[index].numbers);
        }
    }
    return 1;
}

int solve(block blocks[9], int number, int block)
{
    if (number == 10)
        return 1;

    if (contains(&blocks[block], number))
        return solve(blocks, block == 8 ? number + 1 : number, block == 8 ? 0 : block + 1);

    int counter = -1;
    int ready = 0;
    int r = 0, c = 0;

    do {
        delete_with_position(&blocks[block], number, r, c);
        counter++;
        c = (c + 1) % 3;
        if (c == 0)
            r = (r + 1) % 3;

        while (counter < 9 && !insert_without_block_conflict(&blocks[block], number, r, c)) {
            counter++;
            c = (c + 1) % 3;
            if (c == 0)
                r = (r + 1) % 3;
        }

        if (counter >= 9) {
            delete_with_position(&blocks[block], number, r, c);
            return 0;
        }

        ready = solve(blocks, block == 8 ? number + 1 : number, block == 8 ? 0 : block + 1);
    } while (!ready);
    return 1;
}

int generate(sudoku *sudoku, int number, int block)
{
    if (number == 10)
        return 1;
    int counter = -1;
    int ready = 0;
    int r = rand() % 3, c = rand() % 3;

    do {
        delete_with_position(&sudoku->blocks[block], number, r, c);
        counter++;
        c = (c + 1) % 3;
        if (c == 0)
            r = (r + 1) % 3;

        while (counter < 9 && !insert(&sudoku->blocks[block], number, r, c)) {
            counter++;
            c = (c + 1) % 3;
            if (c == 0)
                r = (r + 1) % 3;
        }

        if (counter >= 9)
            return 0;

        if (block == 7)
            ready = generate(sudoku, number + 1, 1);
        else if (block == 3)
            ready = generate(sudoku, number, block + 2);
        else
            ready = generate(sudoku, number, block + 1);

    } while (!ready);
    return 1;
}

void solve_sudoku(sudoku *sudoku)
{
	block solution[9];
	init_block(solution);
	for (int i = 0; i < 9; i++)
	  	set_numbers(&solution[i], sudoku->blocks[i].numbers);

	if (!solve(solution, 1, 0))
	  	exit(0);

	set_solution(sudoku, solution);

  set_solution(sudoku, solution);
}

void set_solution(sudoku *sudoku, block blocks[])
{
    for (int i = 0; i < 9; i++) {
        sudoku->solution[i] = blocks[i];
    }
}

void set_sudoku(sudoku *sudoku, block blocks[])
{
    for (int i = 0; i < 9; i++) {
        sudoku->blocks[i] = blocks[i];
    }
}

void print_blocks(block *blocks)
{
    for (int i = 0; i < 9; i = i + 3) {
        for (int k = 0; k < 3; k++) {
            for (int j = i; j < i + 3; j++) {
                for (int a = 0; a < 3; a++)
                    printf("%i", blocks[j].numbers[k][a]);
                printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    }
}

void shuffle(number *array, int n)
{
    if (n > 1)
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            number t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
}

