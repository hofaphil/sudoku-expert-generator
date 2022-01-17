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
    for (int runs = 0; runs < 3; runs++) {
        sudoku *sudoku = new_sudoku();

        int exp_free_fields = (rand() % (56 - 45 + 1)) + 45;

        int res_free_fields = create(sudoku, exp_free_fields);
        EXPECT_EQ(res_free_fields, exp_free_fields);

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
            for (int number = 1; number < 10; number++) {
                EXPECT_EQ(sudoku->solution[k].contains_number[number], 1);
            }
        }
    }
}

TEST(SudokuTests, init_blocks)
{
    block block[9];
    init_blocks(block);

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
                EXPECT_EQ(s2->solution[k].column[j][number], s1->solution[k].column[j][number]);
            }
        }
    }

    // 452  738  916
    // 781  926  453
    // 396  154  287
    //
    // 965  847  321
    // 278  319  321
    // 134  265  879
    //
    // 523  471  698
    // 847  692  135
    // 619  583  742

    int numbers[9][3][3] = {
        {{0, 5, 0}, {7, 0, 0}, {0, 9, 6}},
        {{0, 0, 0}, {0, 0, 0}, {1, 0, 4}},
        {{0, 0, 0}, {4, 0, 3}, {2, 0, 0}},

        {{0, 0, 0}, {0, 0, 8}, {0, 0, 0}},
        {{0, 0, 7}, {0, 1, 0}, {2, 0, 0}},
        {{0, 0, 0}, {0, 6, 4}, {0, 0, 9}},

        {{0, 0, 0}, {8, 4, 0}, {6, 0, 9}},
        {{0, 0, 1}, {0, 0, 2}, {0, 0, 3}},
        {{0, 9, 0}, {0, 0, 0}, {7, 0, 0}}
    };

    int solution[9][3][3] = {
        {{4, 5, 2}, {7, 8, 1}, {3, 9, 6}},
        {{7, 3, 8}, {9, 2, 6}, {1, 5, 4}},
        {{9, 1, 6}, {4, 5, 3}, {2, 8, 7}},

        {{9, 6, 5}, {2, 7, 8}, {1, 3, 4}},
        {{8, 4, 7}, {3, 1, 9}, {2, 6, 5}},
        {{3, 2, 1}, {5, 6, 4}, {8, 7, 9}},

        {{5, 2, 3}, {8, 4, 7}, {6, 1, 9}},
        {{4, 7, 1}, {6, 9, 2}, {5, 8, 3}},
        {{6, 9, 8}, {1, 3, 5}, {7, 4, 2}}
    };

    block blocks[9];
    init_blocks(blocks);
    for (int i = 0; i < 9; i++)
        set_numbers(&blocks[i], numbers[i]);

    s1 = new_sudoku();
    set_sudoku(s1, blocks);
    solve(s1);

    for (int k = 0; k < 9; k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // check for equal solution
                int number = solution[k][i][j];
                EXPECT_EQ(number, s1->solution[k].numbers[i][j]);
                EXPECT_EQ(s1->solution[k].contains_number[number], 1);
                EXPECT_EQ(s1->solution[k].row[i][number], 1);
                EXPECT_EQ(s1->solution[k].column[j][number], 1);
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
                EXPECT_EQ(src->solution[k].column[j][number], tar->solution[k].column[j][number]);
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
                EXPECT_EQ(src->blocks[k].column[j][number], tar->blocks[k].column[j][number]);
            }
        }
        for (int i = 0; i < 2; i++) {
            EXPECT_NE(src->blocks[k].row_partner[i], nullptr);
            EXPECT_NE(src->blocks[k].column_partner[i], nullptr);
        }
    }
}