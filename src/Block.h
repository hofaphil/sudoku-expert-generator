#ifndef GENERATOR_BLOCK_H
#define GENERATOR_BLOCK_H

typedef struct block block;

struct block
{
    int numbers[3][3];
    block *row_partner[2];
    block *column_partner[2];
    int contains_number[10];
    int row[3][10];
    int column[3][10];
    int latest_del_index_x, latest_del_index_z;
};

block new_block(void);

block new_block_copy(block copy_block);

void generate_random(block *block);

void set_numbers(block *block, int numbers[3][3]);

void set_row_partner(block *p1, block *p2, block *block);

void set_column_partner(block *p1, block *p2, block *block);

int insert(block *block, int number, int row, int column);

int insert_without_block_conflict(block *block, int number, int row, int column);

int delete_number(block *block, int number);

void delete_with_position(block *block, int number, int row, int column);

int contains(block *block, int number);

#endif
