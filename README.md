# sudoku-expert-generator
The sudoku generator for the [Sudoku Expert](https://sudoku-expert.com) app. This generator creates a sudoku with just one solution. It can also produce a solution for a given sudoku.
### Generate a Sudoku
Generating a sudoku is very easy. Just pick your desired number of free fields and create one. After calling `create(int free_fields)` the `blocks` and `solution` arrays are filled.<br>
The `free_fields` indicates how many fields should be empty and have to be filled. The returned number of free fields can be lower than requested: `create` retries for up to 0.5 seconds and returns the best sudoku found. Up to 59 free fields are reached reliably, 60 most of the time, more than that hardly ever (the maximum possible is 64).

```c
sudoku *sudoku = new_sudoku();
int free_fields = 50;
create(sudoku, free_fields); // returns the number of the free fields
```

### Solve a Sudoku
To solve a sudoku, you need valid (solvable) numbers:
```c
sudoku *sudoku = new_sudoku();
memcpy(sudoku->blocks, numbers, sizeof(sudoku->blocks));   // set your numbers as sudoku
solve(sudoku);                                             // returns 0 and fills solution, 1 if not solvable
```

### The sudoku struct
A sudoku consists of `blocks` and `solution`. The `blocks` array represents the sudoku with its empty fields (`0`) to be filled. The `solution` is - as the name suggests - the corresponding solution.
```c
typedef struct sudoku
{
    int blocks[9][3][3];    // the sudoku, 0 = empty field
    int solution[9][3][3];  // the filled out solution
} sudoku;
```
A number is accessed with `blocks[block][row][column]`.

Indexing the blocks:
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

Indexing the row and column within a block:
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
