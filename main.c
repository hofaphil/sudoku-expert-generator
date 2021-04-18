#include "Sudoku.h"
#include <stdio.h>
#include <time.h>

int main()
{
    setbuf(stdout, 0);
    double overall = 0.0;

    for (int i = 0; i < 50; i++) {
        clock_t tic = clock();
        sudoku *s = new_sudoku();
        //printf("creating\n");
        int free_fields = create(s, 2);
        clock_t toc = clock();
        printf("%i free fields in %f sec\n", free_fields, (double) (toc - tic) / CLOCKS_PER_SEC);
        overall += (double) (toc - tic) / CLOCKS_PER_SEC;
        print_sudoku(s);
    }

    printf("Overall: %f sec \n", overall / 50.0 );

    printf("starting\n");

    return 0;
}