#include <stdlib.h>

#include "Block.h"

int block_conflict(block *block, int number);

int line_conflict(block *block, int number, int row, int column);

int has_in_row(block *block, int number, int row);

int has_in_column(block *block, int number, int column);

block new_block()
{
    block b;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            b.numbers[i][j] = 0;

    set_numbers(&b, b.numbers);

    b.latest_del_index_x = b.latest_del_index_z = -1;

    for (int i = 0; i < 2; i++) {
        b.row_partner[i] = NULL;
        b.column_partner[i] = NULL;
    }
    return b;
}

block new_block_copy(block copy_block)
{
    block b = new_block();
    for (int i = 0; i < 2; i++) {
        b.row_partner[i] = copy_block.row_partner[i];
        b.column_partner[i] = copy_block.column_partner[i];
    }
    set_numbers(&b, copy_block.numbers);
    return b;
}

void set_numbers(block *block, int numbers[3][3])
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 3; j++) {
            block->row[j][i] = 0;
            block->column[j][i] = 0;
        }
        block->contains_number[i] = 0;
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            block->numbers[i][j] = numbers[i][j];
            block->contains_number[numbers[i][j]] = 1;
            block->row[i][numbers[i][j]] = 1;
            block->column[j][numbers[i][j]] = 1;
        }
}

void set_row_partner(block *p1, block *p2, block *block)
{
    block->row_partner[0] = p1;
    block->row_partner[1] = p2;
}

void set_column_partner(block *p1, block *p2, block *block)
{
    block->column_partner[0] = p1;
    block->column_partner[1] = p2;
}

int insert(block *block, int number, int row, int column)
{
    if (block->numbers[row][column] == 0 && !block_conflict(block, number) &&
        !line_conflict(block, number, row, column)) {
        block->numbers[row][column] = number;
        block->contains_number[number] = 1;
        block->row[row][number] = 1;
        block->column[column][number] = 1;
        return 1;
    }
    return 0;
}

int insert_without_block_conflict(block *block, int number, int row, int column)
{
    if (block->numbers[row][column] == 0 && !line_conflict(block, number, row, column)) {
        block->numbers[row][column] = number;
        block->contains_number[number] = 1;
        block->row[row][number] = 1;
        block->column[column][number] = 1;
        return 1;
    }
    return 0;
}

void generate_random(block *block)
{
    int row, column;

    for (int i = 1; i < 10; i++) {
        if (block->numbers[row = rand() % 3][column = rand() % 3] == 0) {
            block->numbers[row][column] = i;
            block->contains_number[i] = 1;
            block->row[row][i] = 1;
            block->column[column][i] = 1;
        }
        else
            i--;
    }
}

int block_conflict(block *block, int number)
{
    return block->contains_number[number];
}

int line_conflict(block *block, int number, int row, int column)
{
    return !(!has_in_row(block->row_partner[0], number, row) &&
        !has_in_row(block->row_partner[1], number, row) &&
        !has_in_column(block->column_partner[0], number, column) &&
        !has_in_column(block->column_partner[1], number, column));
}

int has_in_row(block *block, int number, int row)
{
    return block->row[row][number];
}

int has_in_column(block *block, int number, int column)
{
    return block->column[column][number];
}

int delete_number(block *block, int number)
{
    if (block->contains_number[number]) {
        block->contains_number[number] = 0;
        for (int i = 0; i < 3; i++)
            if (block->row[i][number])
                for (int j = 0; j < 3; j++) {
                    if (block->numbers[i][j] == number) {
                        block->numbers[i][j] = 0;
                        block->row[i][number] = 0;
                        block->column[j][number] = 0;
                        block->latest_del_index_x = i;
                        block->latest_del_index_z = j;
                        return 1;
                    }
                }
    }
    return 0;
}

int delete_with_position(block *block, int number, int row, int column)
{
    if (block->contains_number[number]) {
        block->contains_number[number] = 0;
        block->numbers[row][column] = 0;
        block->row[row][number] = 0;
        block->column[column][number] = 0;
        block->latest_del_index_x = row;
        block->latest_del_index_z = column;
        return 1;
    }
    return 0;
}

int contains(block *block, int number)
{
    return block->contains_number[number];
}