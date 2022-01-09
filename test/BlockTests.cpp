#include "gtest/gtest.h"

extern "C" {
#include "../src/Block.h"
}

void check_contains_number(block *block, int number, int row, int col, bool contains)
{
    if (contains)
        EXPECT_EQ(block->numbers[row][col], number);
    else
        EXPECT_NE(block->numbers[row][col], number);

    EXPECT_EQ(block->contains_number[number], contains ? 1 : 0);
    EXPECT_EQ(block->row[row][number], contains ? 1 : 0);
    EXPECT_EQ(block->column[col][number], contains ? 1 : 0);
}

TEST(BlockTests, new_block)
{
    block block = new_block();

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_EQ(block.numbers[i][j], 0);

    for (int j = 0; j < 3; j++)
        for (int i = 1; i < 10; i++) {
            EXPECT_EQ(block.contains_number[i], 0);
            EXPECT_EQ(block.row[j][i], 0);
            EXPECT_EQ(block.column[j][i], 0);
        }

    EXPECT_EQ(block.latest_del_index_x, -1);
    EXPECT_EQ(block.latest_del_index_x, block.latest_del_index_z);

    for (int i = 0; i < 2; i++) {
        EXPECT_EQ(block.row_partner[i], nullptr);
        EXPECT_EQ(block.column_partner[i], nullptr);
    }
}

TEST(BlockTests, new_block_copy)
{
    block block_orig = new_block();
    block block_copy = new_block_copy(block_orig);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_EQ(block_orig.numbers[i][j], block_copy.numbers[i][j]);

    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 10; i++) {
            EXPECT_EQ(block_orig.contains_number[i], block_copy.contains_number[i]);
            EXPECT_EQ(block_orig.row[j][i], block_copy.row[j][i]);
            EXPECT_EQ(block_orig.column[j][i], block_copy.column[j][i]);
        }

    EXPECT_EQ(block_orig.latest_del_index_x, block_copy.latest_del_index_x);
    EXPECT_EQ(block_orig.latest_del_index_z, block_copy.latest_del_index_z);

    for (int i = 0; i < 2; i++) {
        EXPECT_EQ(block_orig.row_partner[i], block_copy.row_partner[i]);
        EXPECT_EQ(block_orig.column_partner[i], block_copy.column_partner[i]);
    }
}

TEST(BlockTests, generate_random)
{
    block block = new_block();
    generate_random(&block);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_GT(block.numbers[i][j], 0);
}

TEST(BlockTests, set_numbers)
{
    block block = new_block();
    int numbers[3][3];

    // TODO should it be allowed to insert here without checks?
    int k = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            numbers[i][j] = k++;

    set_numbers(&block, numbers);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            check_contains_number(&block, numbers[i][j], i, j, true);
}

TEST(BlockTests, set_row_partner)
{
    block b = new_block(), p1 = new_block(), p2 = new_block();
    set_row_partner(&p1, &p2, &b);

    EXPECT_EQ(b.row_partner[0], &p1);
    EXPECT_EQ(b.row_partner[1], &p2);
}


TEST(BlockTests, set_column_partner)
{
    block b = new_block(), p1 = new_block(), p2 = new_block();
    set_column_partner(&p1, &p2, &b);

    EXPECT_EQ(b.column_partner[0], &p1);
    EXPECT_EQ(b.column_partner[1], &p2);
}

TEST(BlockTests, insert)
{
    block b = new_block();
    block row_p1 = new_block(), row_p2 = new_block();
    block col_p1 = new_block(), col_p2 = new_block();

    set_row_partner(&row_p1, &row_p2, &b);
    set_column_partner(&col_p1, &col_p2, &b);
    int number = 2, row = 1, col = 1;

    // should work
    int ret = insert(&b, number, row, col);

    EXPECT_EQ(ret, 1);
    check_contains_number(&b, number, row, col, true);

    // block-conflict
    row += 1;
    ret = insert(&b, number, row, col);

    EXPECT_EQ(ret, 0);

    // row-conflict
    number += 1;
    row_p1.numbers[row][col] = number;
    row_p1.contains_number[number] = 1;
    row_p1.row[row][number] = 1;
    row_p1.column[col][number] = 1;
    ret = insert(&b, number, row, col);

    EXPECT_EQ(ret, 0);
    check_contains_number(&b, number, row, col, false);

    // column-conflict
    row -= 1;
    col += 1;
    col_p1.numbers[row][col] = number;
    col_p1.contains_number[number] = 1;
    col_p1.row[row][number] = 1;
    col_p1.column[col][number] = 1;
    ret = insert(&b, number, row, col);

    EXPECT_EQ(ret, 0);
    check_contains_number(&b, number, row, col, false);
}

TEST(BlockTests, insert_without_block_conflict)
{
    block b = new_block();
    block row_p1 = new_block(), row_p2 = new_block();
    block col_p1 = new_block(), col_p2 = new_block();

    set_row_partner(&row_p1, &row_p2, &b);
    set_column_partner(&col_p1, &col_p2, &b);
    int number = 2, row = 1, col = 1;

    // should work
    int ret = insert_without_block_conflict(&b, number, row, col);

    EXPECT_EQ(ret, 1);
    check_contains_number(&b, number, row, col, true);

    // block-conflict
    row += 1;
    ret = insert_without_block_conflict(&b, number, row, col);

    EXPECT_EQ(ret, 1);
    check_contains_number(&b, number, row, col, true);

    // row-conflict
    number += 1;
    row_p1.numbers[row][col] = number;
    row_p1.contains_number[number] = 1;
    row_p1.row[row][number] = 1;
    row_p1.column[col][number] = 1;
    ret = insert_without_block_conflict(&b, number, row, col);

    EXPECT_EQ(ret, 0);
    check_contains_number(&b, number, row, col, false);

    // column-conflict
    row -= 1;
    col += 1;
    col_p1.numbers[row][col] = number;
    col_p1.contains_number[number] = 1;
    col_p1.row[row][number] = 1;
    col_p1.column[col][number] = 1;
    ret = insert_without_block_conflict(&b, number, row, col);

    EXPECT_EQ(ret, 0);
    check_contains_number(&b, number, row, col, false);
}

TEST(BlockTests, delete_number)
{
    block b = new_block();
    block row_p1 = new_block(), row_p2 = new_block();
    block col_p1 = new_block(), col_p2 = new_block();

    set_row_partner(&row_p1, &row_p2, &b);
    set_column_partner(&col_p1, &col_p2, &b);
    int number[3] = {3, 5, 9}, rows[3] = {0, 2, 1}, cols[3] = {2, 0, 1};

    // should work
    for (int i = 0; i < 3; i++) {
        insert(&b, number[i], rows[i], cols[i]);
        check_contains_number(&b, number[i], rows[i], cols[i], true);
        int ret = delete_number(&b, number[i]);
        EXPECT_EQ(ret, 1);
        check_contains_number(&b, number[i], rows[i], cols[i], false);
    }

    // does not contain number
    EXPECT_EQ(b.contains_number[1], 0);
    int ret = delete_number(&b, 1);
    EXPECT_EQ(ret, 0);
}

TEST(BlockTests, delete_with_position)
{
    block b = new_block();
    block row_p1 = new_block(), row_p2 = new_block();
    block col_p1 = new_block(), col_p2 = new_block();

    set_row_partner(&row_p1, &row_p2, &b);
    set_column_partner(&col_p1, &col_p2, &b);
    int number[3] = {3, 5, 9};

    // should work
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            insert(&b, number[i], i, j);
            check_contains_number(&b, number[i], i, j, true);
            int ret = delete_with_position(&b, number[i], i, j);
            EXPECT_EQ(ret, 1);
            check_contains_number(&b, number[i], i, j, false);
        }

    // does not contain number
    int num = 1, row = 1, col = 1;
    check_contains_number(&b, num, row, col, false);
    int ret = delete_with_position(&b, num, row, col);
    EXPECT_EQ(ret, 0);
}

TEST(BlockTests, contains)
{
    block b = new_block();
    block row_p1 = new_block(), row_p2 = new_block();
    block col_p1 = new_block(), col_p2 = new_block();

    set_row_partner(&row_p1, &row_p2, &b);
    set_column_partner(&col_p1, &col_p2, &b);

    // contains should be true
    int num = 4, row = 2, col = 1;
    insert(&b, num, row, col);
    check_contains_number(&b, num, row, col, true);

    EXPECT_EQ(contains(&b, num), 1);

    // contains should be false
    delete_with_position(&b, num, row, col);
    check_contains_number(&b, num, row, col, false);

    EXPECT_EQ(contains(&b, num), 0);
}
