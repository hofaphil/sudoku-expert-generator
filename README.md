# sudoku-expert-generator
The sudoku generator for the SudokuExpert app. This generator creates a sudoku with just one solution. It can also produce a solution for a given sudoku.
### Generate a Sudoku
Generating a sudoku is very easy. Just pick your desired number of free fields and create one. After calling `create(int free_fields)` the `blocks` and `solution` arrays are filled.<br>
The `free_fields` indicates how many fields should empty and have to be filled. The passed number can differ from the actual number of free fields, since the algorithm may stop earlier (above all `free_fields > 56`).

```c
sudoku *sudoku = new_sudoku();
int free_fields = 50;
create(sudoku, free_fields); // returns the number of the free fields
```
### Solve a Sudoku
To solve a sudoku, you need a valid (solveable) `block`:
```c
sudoku *sudoku = new_sudoku();
set_sudoku(sudoku, &block);     // set your block as sudoku
solve(sudoku);                  // solve the sudoku
```
### The sudoku and block struct
A sudoku consists of `blocks` and `solution`. The `blocks` array represents the sudoku with its empty fields to be filled. The `solution` is - as the name suggests - the corresponding solution.
```c
typedef struct sudoku
{
    block blocks[9];    // the sudoku with empty fields to be filled
    block solution[9];  // the solution (filled out)
} sudoku;
```
Indexing the block-array:
<table>
  <tr>
    <td>0</td>
    <td>1</td>
    <td>2</td>
  </tr>
  <tr>
    <td>3</td>
    <td>4</td>
    <td>5</td>
  </tr>
  <tr>
    <td>6</td>
    <td>7</td>
    <td>8</td>
  </tr>
</table>

Both of these arrays are `block` types.
```c
struct block
{
    int numbers[3][3];          // the numbers
    block *row_partner[2];      // its row-partner in the sudoku
    block *column_partner[2];   // its column-partner in the sudoku
    int contains_number[10];    // flags to quickly check number existence
    int row[3][10];             
    int column[3][10];
    int latest_del_index_x, latest_del_index_z;
};
```
Indexing the numbers-array:
<table>
  <tr>
    <td>0, 0</td>
    <td>0, 1</td>
    <td>0, 2</td>
  </tr>
  <tr>
    <td>1, 0</td>
    <td>1, 1</td>
    <td>1, 2</td>
  </tr>
  <tr>
    <td>2, 0</td>
    <td>2, 1</td>
    <td>2, 2</td>
  </tr>
</table>
