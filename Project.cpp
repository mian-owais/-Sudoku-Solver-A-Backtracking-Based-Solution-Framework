#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
using namespace std;
const int SIZE = 9;
int sudokuBoard[SIZE][SIZE];
bool isSafe(int board[SIZE][SIZE], int row, int col, int num);
void generateSudokuBoard(int difficulty);
void shuffleRowsWithinBands();
void shuffleColsWithinStacks();
void shuffleRowBands();
void shuffleColStacks();
void swapRows(int row1, int row2);
void swapCols(int col1, int col2);
bool validateBoard(int board[SIZE][SIZE]);
void printSudokuBoard(int board[SIZE][SIZE]);
bool solveSudoku(int board[SIZE][SIZE]);
int main()
{
    srand(time(0));
    int difficulty = 1;
    cout << "Generating Sudoku Board...\n";
    generateSudokuBoard(difficulty);
    cout << "Generated Board:\n";
    printSudokuBoard(sudokuBoard);
    cout << "Validating Board...\n";
    if (validateBoard(sudokuBoard))
    {
        cout << "Board is valid!\n";
    }
    else
    {
        cout << "Board is invalid!\n";
    }
    cout << "Attempting to Solve the Sudoku (Work in Progress)...\n";
    if (solveSudoku(sudokuBoard))
    {
        cout << "Solved Sudoku Board:\n";
        printSudokuBoard(sudokuBoard);
    }
    else
    {
        cout << "Solution not found or incomplete implementation.\n";
    }
    return 0;
}
void generateSudokuBoard(int difficulty)
{
    int baseBoard[SIZE][SIZE] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 1, 2, 3, 4, 5, 6},
        {2, 3, 4, 5, 6, 7, 8, 9, 1},
        {5, 6, 7, 8, 9, 1, 2, 3, 4},
        {8, 9, 1, 2, 3, 4, 5, 6, 7},
        {3, 4, 5, 6, 7, 8, 9, 1, 2},
        {6, 7, 8, 9, 1, 2, 3, 4, 5},
        {9, 1, 2, 3, 4, 5, 6, 7, 8}};
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            sudokuBoard[i][j] = baseBoard[i][j];
        }
    }
    for (int i = 0; i < 10; ++i)
    {
        shuffleRowsWithinBands();
        shuffleColsWithinStacks();
        shuffleRowBands();
        shuffleColStacks();
    }
    int cellsToRemove;
    if (difficulty == 1)
    {
        cellsToRemove = 30;
    }
    else if (difficulty == 2)
    {
        cellsToRemove = 40;
    }
    else if (difficulty == 3)
    {
        cellsToRemove = 50;
    }
    else
    {
        cellsToRemove = 30;
    }
    while (cellsToRemove > 0)
    {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (sudokuBoard[row][col] != 0)
        {
            sudokuBoard[row][col] = 0;
            cellsToRemove--;
        }
    }
}
void shuffleRowsWithinBands()
{
    for (int band = 0; band < SIZE; band += 3)
    {
        int row1 = band + rand() % 3;
        int row2 = band + rand() % 3;
        swapRows(row1, row2);
    }
}
void shuffleColsWithinStacks()
{
    for (int stack = 0; stack < SIZE; stack += 3)
    {
        int col1 = stack + rand() % 3;
        int col2 = stack + rand() % 3;
        swapCols(col1, col2);
    }
}
void shuffleRowBands()
{
    int band1 = (rand() % 3) * 3;
    int band2 = (rand() % 3) * 3;
    for (int i = 0; i < 3; ++i)
    {
        swapRows(band1 + i, band2 + i);
    }
}
void shuffleColStacks()
{
    int stack1 = (rand() % 3) * 3;
    int stack2 = (rand() % 3) * 3;
    for (int i = 0; i < 3; ++i)
    {
        swapCols(stack1 + i, stack2 + i);
    }
}
void swapRows(int row1, int row2)
{
    for (int col = 0; col < SIZE; ++col)
    {
        swap(sudokuBoard[row1][col], sudokuBoard[row2][col]);
    }
}
void swapCols(int col1, int col2)
{
    for (int row = 0; row < SIZE; ++row)
    {
        swap(sudokuBoard[row][col1], sudokuBoard[row][col2]);
    }
}
bool validateBoard(int board[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; ++row)
    {
        bool seen[SIZE + 1] = {false};
        for (int col = 0; col < SIZE; ++col)
        {
            int num = board[row][col];
            if (num != 0)
            {
                if (seen[num])
                {
                    return false;
                }
                seen[num] = true;
            }
        }
    }
    for (int col = 0; col < SIZE; ++col)
    {
        bool seen[SIZE + 1] = {false};
        for (int row = 0; row < SIZE; ++row)
        {
            int num = board[row][col];
            if (num != 0)
            {
                if (seen[num])
                {
                    return false;
                }
                seen[num] = true;
            }
        }
    }
    for (int gridRow = 0; gridRow < SIZE; gridRow += 3)
    {
        for (int gridCol = 0; gridCol < SIZE; gridCol += 3)
        {
            bool seen[SIZE + 1] = {false};
            for (int row = 0; row < 3; ++row)
            {
                for (int col = 0; col < 3; ++col)
                {
                    int num = board[gridRow + row][gridCol + col];
                    if (num != 0)
                    {
                        if (seen[num])
                        {
                            return false;
                        }
                        seen[num] = true;
                    }
                }
            }
        }
    }
    return true;
}
void printSudokuBoard(int board[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; ++i)
    {
        if (i % 3 == 0 && i != 0)
        {
            cout << "------+-------+------\n";
        }
        for (int j = 0; j < SIZE; ++j)
        {
            if (j % 3 == 0 && j != 0)
            {
                cout << "| ";
            }
            if (board[i][j] == 0)
            {
                cout << ". ";
            }
            else
            {
                cout << board[i][j] << " ";
            }
        }
        cout << "\n";
    }
}
bool isSafe(int board[SIZE][SIZE], int row, int col, int num)
{
    for (int i = 0; i < SIZE; ++i)
    {
        if (board[row][i] == num)
        {
            return false;
        }
    }
    for (int i = 0; i < SIZE; ++i)
    {
        if (board[i][col] == num)
        {
            return false;
        }
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board[startRow + i][startCol + j] == num)
            {
                return false;
            }
        }
    }
    return true;
}
bool solveSudoku(int board[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            if (board[row][col] == 0)
            {
                for (int num = 1; num <= SIZE; ++num)
                {
                    if (isSafe(board, row, col, num))
                    {
                        board[row][col] = num;
                        if (solveSudoku(board))
                        {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
bool isSafe(int board[SIZE][SIZE], int row, int col, int num)
{
    for (int i = 0; i < SIZE; ++i)
    {
        if (board[row][i] == num)
        {
            return false;
        }
    }
    for (int i = 0; i < SIZE; ++i)
    {
        if (board[i][col] == num)
        {
            return false;
        }
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board[startRow + i][startCol + j] == num)
            {
                return false;
            }
        }
    }
    return true;
}
