#include <stdlib.h>
#include <time.h>

#include "Sudoku.h"

#define ALL_DIGITS 0x3FE

// how long create may retry to reach the wanted free fields
#define CREATE_TIME_LIMIT (CLOCKS_PER_SEC / 2)

// the sudoku as 81 cells (row by row) plus bitmasks of the digits used per row, column and box
typedef struct grid {
    int cells[81];
    unsigned short rows[9];
    unsigned short cols[9];
    unsigned short boxes[9];
} grid;

static int cell_index(const int block, const int row, const int column) {
    return ((block / 3) * 3 + row) * 9 + (block % 3) * 3 + column;
}

static unsigned short candidates(grid const *g, const int cell) {
    const int row = cell / 9;
    const int column = cell % 9;
    return ~(g->rows[row] | g->cols[column] | g->boxes[(row / 3) * 3 + column / 3]) & ALL_DIGITS;
}

static void toggle(grid *g, const int cell, const int digit) {
    const int row = cell / 9;
    const int column = cell % 9;
    const unsigned short bit = (unsigned short) (1 << digit);
    g->rows[row] ^= bit;
    g->cols[column] ^= bit;
    g->boxes[(row / 3) * 3 + column / 3] ^= bit;
}

static void place(grid *g, const int cell, const int digit) {
    g->cells[cell] = digit;
    toggle(g, cell, digit);
}

static void unplace(grid *g, const int cell, const int digit) {
    g->cells[cell] = 0;
    toggle(g, cell, digit);
}

static void shuffle(int *array, const int n) {
    for (int i = n - 1; i > 0; i--) {
        const int j = rand() % (i + 1);
        const int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

/*
 * Backtracking that always branches on the empty cell with the fewest candidates.
 * Returns the number of solutions found, but stops at limit. With limit 1 the grid stays filled
 * with the found solution. random tries the candidates in random order (used for generating).
 */
static int search(grid *g, const int limit, const int random) {
    int best = -1;
    int best_count = 10;
    unsigned short best_candidates = 0;

    for (int cell = 0; cell < 81; cell++) {
        if (g->cells[cell]) {
            continue;
        }

        const unsigned short cand = candidates(g, cell);
        const int count = __builtin_popcount(cand);
        if (count < best_count) {
            best = cell;
            best_count = count;
            best_candidates = cand;
            if (count <= 1) {
                break;
            }
        }
    }

    if (best == -1) {
        return 1;
    }

    int digits[9];
    int n = 0;
    for (; best_candidates; best_candidates &= best_candidates - 1) {
        digits[n++] = __builtin_ctz(best_candidates);
    }
    if (random) {
        shuffle(digits, n);
    }

    int found = 0;
    for (int i = 0; i < n; i++) {
        place(g, best, digits[i]);
        found += search(g, limit - found, random);
        if (found >= limit) {
            return found;
        }
        unplace(g, best, digits[i]);
    }
    return found;
}

// returns 0 if the numbers break the sudoku rules
static int load_grid(grid *g, int blocks[9][3][3]) {
    *g = (grid){0};

    for (int b = 0; b < 9; b++)
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++) {
                const int cell = cell_index(b, r, c);
                const int digit = blocks[b][r][c];
                if (!digit) {
                    continue;
                }
                if (digit < 1 || digit > 9 || !(candidates(g, cell) & (1 << digit))) {
                    return 0;
                }
                place(g, cell, digit);
            }
    return 1;
}

static void store_grid(grid const *g, int blocks[9][3][3]) {
    for (int block = 0; block < 9; block++)
        for (int row = 0; row < 3; row++)
            for (int column = 0; column < 3; column++)
                blocks[block][row][column] = g->cells[cell_index(block, row, column)];
}

sudoku *new_sudoku(void) {
    sudoku *s = calloc(1, sizeof(sudoku));

    // seed only once, otherwise sudokus created within the same second are identical
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned) time(NULL));
        seeded = 1;
    }
    return s;
}

// one attempt: random solution, then remove numbers in random order as long as the solution stays unique
static int create_once(sudoku *s, const int free_fields) {
    grid g = {0};
    search(&g, 1, 1);
    store_grid(&g, s->solution);

    int order[81];
    for (int i = 0; i < 81; i++)
        order[i] = i;
    shuffle(order, 81);

    // remove numbers in random order, but only if the sudoku keeps exactly one solution
    int removed = 0;
    for (int i = 0; i < 81 && removed < free_fields; i++) {
        const int cell = order[i];
        const int digit = g.cells[cell];
        unplace(&g, cell, digit);

        grid copy = g;
        if (search(&copy, 2, 0) == 1) {
            removed++;
        } else {
            place(&g, cell, digit);
        }
    }

    store_grid(&g, s->blocks);
    return removed;
}

/*
 * A single attempt often gets stuck before reaching free_fields (typically around 57),
 * so retry with new sudokus and keep the best one until free_fields or the time limit is reached.
 */
int create(sudoku *s, const int free_fields) {
    const clock_t start = clock();
    sudoku attempt;
    int best = -1;

    do {
        int removed = create_once(&attempt, free_fields);
        if (removed > best) {
            best = removed;
            *s = attempt;
        }
    } while (best < free_fields && clock() - start < CREATE_TIME_LIMIT);

    return best;
}

int solve(sudoku *s) {
    grid g;
    if (!load_grid(&g, s->blocks) || !search(&g, 1, 0)) {
        return 1;
    }

    store_grid(&g, s->solution);
    return 0;
}
