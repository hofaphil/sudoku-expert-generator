#include "gtest/gtest.h"

extern "C" {
#include "../src/Sudoku.h"
}

TEST(SudokuTests, new_sudoku)
{
    sudoku *sudoku = new_sudoku();

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                EXPECT_EQ(sudoku->blocks[k].numbers[i][j], 0);
                EXPECT_EQ(sudoku->solution[k].numbers[i][j], 0);
            }

        for (int j = 0; j < 3; j++)
            for (int i = 1; i < 10; i++) {
                EXPECT_EQ(sudoku->blocks[k].contains_number[i], 0);
                EXPECT_EQ(sudoku->blocks[k].row[j][i], 0);
                EXPECT_EQ(sudoku->blocks[k].column[j][i], 0);

                EXPECT_EQ(sudoku->solution[k].contains_number[i], 0);
                EXPECT_EQ(sudoku->solution[k].row[j][i], 0);
                EXPECT_EQ(sudoku->solution[k].column[j][i], 0);
            }

        EXPECT_EQ(sudoku->blocks[k].latest_del_index_x, -1);
        EXPECT_EQ(sudoku->blocks[k].latest_del_index_x, sudoku->blocks[k].latest_del_index_z);

        EXPECT_EQ(sudoku->solution[k].latest_del_index_x, -1);
        EXPECT_EQ(sudoku->solution[k].latest_del_index_x, sudoku->blocks[k].latest_del_index_z);

        for (int i = 0; i < 2; i++) {
            EXPECT_NE(sudoku->blocks[k].row_partner[i], nullptr);
            EXPECT_NE(sudoku->blocks[k].column_partner[i], nullptr);

            EXPECT_NE(sudoku->solution[k].row_partner[i], nullptr);
            EXPECT_NE(sudoku->solution[k].column_partner[i], nullptr);
        }
    }
}

TEST(SudokuTests, create)
{
    for (int diff = 0; diff < 3; diff++) {
        sudoku *sudoku = new_sudoku();

        int free_fields = create(sudoku, diff);
        EXPECT_GT(free_fields, (42 + 7 * diff) - 2);

        for (int k = 0; k < 9; k++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    // sudoku numbers must be the same as in solution
                    if (sudoku->blocks[k].numbers[i][j] != 0)
                        EXPECT_EQ(sudoku->blocks[k].numbers[i][j], sudoku->solution[k].numbers[i][j]);

                    // check that all numbers are set in the solution
                    EXPECT_GT(sudoku->solution[k].numbers[i][j], 0);
                    EXPECT_LT(sudoku->solution[k].numbers[i][j], 10);
                }
            }
        }
    }
}

TEST(SudokuTests, init_block)
{
    block block[9];
    init_block(block);

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                EXPECT_EQ(block[k].numbers[i][j], 0);
            }
        }
        for (int i = 0; i < 2; i++) {
            EXPECT_NE(block[k].row_partner[i], nullptr);
            EXPECT_NE(block[k].column_partner[i], nullptr);
        }
    }
}

TEST(SudokuTests, solve)
{
    sudoku *s1 = new_sudoku();
    create(s1, 1);

    sudoku *s2 = new_sudoku();
    set_sudoku(s2, s1->blocks);

    int ret = solve(s2);
    EXPECT_EQ(ret, 0);

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // check that all numbers are set in the solution
                EXPECT_GT(s2->solution[k].numbers[i][j], 0);
                EXPECT_LT(s2->solution[k].numbers[i][j], 10);

                // check for equal solution
                int number = s2->solution[k].numbers[i][j];
                EXPECT_EQ(number, s1->solution[k].numbers[i][j]);
                EXPECT_EQ(s2->solution[k].contains_number[number], s1->solution[k].contains_number[number]);
                EXPECT_EQ(s2->solution[k].row[i][number], s1->solution[k].row[i][number]);
                EXPECT_EQ(s2->solution[k].column[i][number], s1->solution[k].column[i][number]);
            }
        }
    }
}

TEST(SudokuTests, set_solution)
{
    sudoku *src = new_sudoku();
    create(src, 0);

    sudoku *tar = new_sudoku();
    set_solution(tar, src->solution);

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int number = src->solution[k].numbers[i][j];
                EXPECT_EQ(number, tar->solution[k].numbers[i][j]);
                EXPECT_EQ(src->solution[k].contains_number[number], tar->solution[k].contains_number[number]);
                EXPECT_EQ(src->solution[k].row[i][number], tar->solution[k].row[i][number]);
                EXPECT_EQ(src->solution[k].column[i][number], tar->solution[k].column[i][number]);
            }
        }
        for (int i = 0; i < 2; i++) {
            EXPECT_NE(src->solution[k].row_partner[i], nullptr);
            EXPECT_NE(src->solution[k].column_partner[i], nullptr);
        }
    }
}

TEST(SudokuTests, set_sudoku)
{
    sudoku *src = new_sudoku();
    create(src, 0);

    sudoku *tar = new_sudoku();
    set_sudoku(tar, src->blocks);

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int number = src->blocks[k].numbers[i][j];
                EXPECT_EQ(number, tar->blocks[k].numbers[i][j]);
                EXPECT_EQ(src->blocks[k].contains_number[number], tar->blocks[k].contains_number[number]);
                EXPECT_EQ(src->blocks[k].row[i][number], tar->blocks[k].row[i][number]);
                EXPECT_EQ(src->blocks[k].column[i][number], tar->blocks[k].column[i][number]);
            }
        }
        for (int i = 0; i < 2; i++) {
            EXPECT_NE(src->blocks[k].row_partner[i], nullptr);
            EXPECT_NE(src->blocks[k].column_partner[i], nullptr);
        }
    }
}