#include "gtest/gtest.h"

#include <cstring>

extern "C" {
#include "../src/Sudoku.h"
}

static int value(int numbers[9][3][3], int row, int column)
{
    return numbers[row / 3 * 3 + column / 3][row % 3][column % 3];
}

static void expect_valid_solution(int numbers[9][3][3])
{
    for (int line = 0; line < 9; line++) {
        int in_row[10] = {0};
        int in_column[10] = {0};
        int in_block[10] = {0};
        for (int pos = 0; pos < 9; pos++) {
            int r = value(numbers, line, pos);
            int c = value(numbers, pos, line);
            int b = numbers[line][pos / 3][pos % 3];
            ASSERT_GT(r, 0);
            ASSERT_LT(r, 10);
            in_row[r]++;
            in_column[c]++;
            in_block[b]++;
        }
        for (int n = 1; n < 10; n++) {
            EXPECT_EQ(in_row[n], 1);
            EXPECT_EQ(in_column[n], 1);
            EXPECT_EQ(in_block[n], 1);
        }
    }
}

// plain backtracking, independent of the generator, counts up to limit solutions
static int count_solutions(int numbers[9][3][3], int limit)
{
    for (int row = 0; row < 9; row++)
        for (int column = 0; column < 9; column++) {
            int &cell = numbers[row / 3 * 3 + column / 3][row % 3][column % 3];
            if (cell)
                continue;

            int found = 0;
            for (int digit = 1; digit < 10 && found < limit; digit++) {
                bool ok = true;
                for (int i = 0; i < 9 && ok; i++)
                    ok = value(numbers, row, i) != digit && value(numbers, i, column) != digit &&
                         value(numbers, row / 3 * 3 + i / 3, column / 3 * 3 + i % 3) != digit;
                if (!ok)
                    continue;
                cell = digit;
                found += count_solutions(numbers, limit - found);
                cell = 0;
            }
            return found;
        }
    return 1;
}

static int count_free_fields(int numbers[9][3][3])
{
    int free_fields = 0;
    for (int b = 0; b < 9; b++)
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                free_fields += numbers[b][i][j] == 0;
    return free_fields;
}

TEST(SudokuTests, new_sudoku)
{
    sudoku *s = new_sudoku();

    for (int b = 0; b < 9; b++)
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                EXPECT_EQ(s->blocks[b][i][j], 0);
                EXPECT_EQ(s->solution[b][i][j], 0);
            }
    free(s);
}

TEST(SudokuTests, create)
{
    for (int runs = 0; runs < 10; runs++) {
        sudoku *s = new_sudoku();

        // retrying in create reaches up to 59 free fields reliably
        int exp_free_fields = (rand() % (59 - 45 + 1)) + 45;
        int res_free_fields = create(s, exp_free_fields);

        EXPECT_EQ(res_free_fields, exp_free_fields);
        EXPECT_EQ(count_free_fields(s->blocks), res_free_fields);
        expect_valid_solution(s->solution);

        // sudoku numbers must be the same as in solution
        for (int b = 0; b < 9; b++)
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (s->blocks[b][i][j] != 0)
                        EXPECT_EQ(s->blocks[b][i][j], s->solution[b][i][j]);

        EXPECT_EQ(count_solutions(s->blocks, 2), 1);
        free(s);
    }
}

TEST(SudokuTests, create_max_free_fields)
{
    sudoku *s = new_sudoku();
    // not reachable, so create retries until the time limit and returns the best one
    int free_fields = create(s, 81);

    EXPECT_GE(free_fields, 58);
    EXPECT_LE(free_fields, 64);
    EXPECT_EQ(count_solutions(s->blocks, 2), 1);
    free(s);
}

TEST(SudokuTests, create_no_free_fields)
{
    sudoku *s = new_sudoku();
    EXPECT_EQ(create(s, 0), 0);
    EXPECT_EQ(memcmp(s->blocks, s->solution, sizeof(s->blocks)), 0);
    free(s);
}

TEST(SudokuTests, create_twice)
{
    sudoku *s = new_sudoku();
    create(s, 40);
    EXPECT_EQ(create(s, 40), 40);

    expect_valid_solution(s->solution);
    EXPECT_EQ(count_solutions(s->blocks, 2), 1);
    free(s);
}

TEST(SudokuTests, create_differs)
{
    // the jni creates a new sudoku per call, all within the same second
    sudoku *s1 = new_sudoku();
    create(s1, 45);
    sudoku *s2 = new_sudoku();
    create(s2, 45);

    EXPECT_NE(memcmp(s1->solution, s2->solution, sizeof(s1->solution)), 0);
    free(s1);
    free(s2);
}

TEST(SudokuTests, solve)
{
    sudoku *s1 = new_sudoku();
    create(s1, 50);

    sudoku *s2 = new_sudoku();
    memcpy(s2->blocks, s1->blocks, sizeof(s1->blocks));

    EXPECT_EQ(solve(s2), 0);
    EXPECT_EQ(memcmp(s1->solution, s2->solution, sizeof(s1->solution)), 0);
    free(s1);
    free(s2);

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

    sudoku *s = new_sudoku();
    memcpy(s->blocks, numbers, sizeof(numbers));

    EXPECT_EQ(solve(s), 0);
    EXPECT_EQ(memcmp(s->solution, solution, sizeof(solution)), 0);
    // the puzzle itself stays untouched
    EXPECT_EQ(memcmp(s->blocks, numbers, sizeof(numbers)), 0);
    free(s);
}

TEST(SudokuTests, solve_invalid)
{
    sudoku *s = new_sudoku();

    // two 5 in the same row
    s->blocks[0][0][0] = 5;
    s->blocks[1][0][0] = 5;
    EXPECT_EQ(solve(s), 1);

    // out of range
    s->blocks[1][0][0] = 10;
    EXPECT_EQ(solve(s), 1);
    free(s);
}

TEST(SudokuTests, solve_empty)
{
    sudoku *s = new_sudoku();
    EXPECT_EQ(solve(s), 0);
    expect_valid_solution(s->solution);
    free(s);
}
