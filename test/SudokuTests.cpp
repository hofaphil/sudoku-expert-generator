#include "gtest/gtest.h"

extern "C" {
#include "../src/Sudoku.h"
}

TEST(SudokuTest, new_sudoku)
{
    sudoku *sudoku = new_sudoku();

    for (int k = 0; k < 9; k++){
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                EXPECT_EQ(sudoku->blocks[k].numbers[i][j], 0);

        for (int j = 0; j < 3; j++)
            for (int i = 1; i < 10; i++) {
                EXPECT_EQ(sudoku->blocks[k].contains_number[i], 0);
                EXPECT_EQ(sudoku->blocks[k].row[j][i], 0);
                EXPECT_EQ(sudoku->blocks[k].column[j][i], 0);
            }

        EXPECT_EQ(sudoku->blocks[k].latest_del_index_x, -1);
        EXPECT_EQ(sudoku->blocks[k].latest_del_index_x, sudoku->blocks[k].latest_del_index_z);
    }
}