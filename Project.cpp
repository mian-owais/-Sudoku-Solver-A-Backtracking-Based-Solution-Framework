#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <fstream>
#include <algorithm>
#include <chrono>

using namespace std;

// Function declarations
void inputPuzzle(vector<vector<int>> &grid);
void displayGrid(const vector<vector<int>> &grid);
bool validateGrid(const vector<vector<int>> &grid);
bool solveSudoku(vector<vector<int>> &grid, unordered_map<int, unordered_set<int>> &rowConstraints, unordered_map<int, unordered_set<int>> &colConstraints, unordered_map<int, unordered_set<int>> &boxConstraints);
void optimizeSolver(vector<vector<int>> &grid);
void performanceReport(const vector<vector<int>> &grid);
void managePuzzles();
void errorHandling(const vector<vector<int>> &grid);

int main()
{
    vector<vector<int>> grid(9, vector<int>(9, 0));
    int choice;

    do
    {
        cout << "Sudoku Solver Menu:\n";
        cout << "1. Input Puzzle\n";
        cout << "2. Display Puzzle\n";
        cout << "3. Solve Puzzle\n";
        cout << "4. Performance Report\n";
        cout << "5. Manage Puzzles\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            inputPuzzle(grid);
            break;
        case 2:
            displayGrid(grid);
            break;
        case 3:
            if (validateGrid(grid))
            {
                optimizeSolver(grid);
                displayGrid(grid);
            }
            else
            {
                cout << "Invalid puzzle.\n";
            }
            break;
        case 4:
            performanceReport(grid);
            break;
        case 5:
            managePuzzles();
            break;
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

void inputPuzzle(vector<vector<int>> &grid)
{
    int choice;
    cout << "Choose input method:\n";
    cout << "1. Manual input\n";
    cout << "2. Input from file\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        // Manual input
        cout << "Enter the Sudoku puzzle row by row (use 0 for empty cells):\n";
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                int value;
                cin >> value;
                if (value < 0 || value > 9)
                {
                    cout << "Invalid input. Please enter a value between 0 and 9.\n";
                    return;
                }
                grid[i][j] = value;
            }
        }
    }
    else if (choice == 2)
    {
        // Input from file
        string filename;
        cout << "Enter the filename: ";
        cin >> filename;
        ifstream file(filename);
        if (!file)
        {
            cout << "Error opening file. Please try again.\n";
            return;
        }
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                int value;
                file >> value;
                if (value < 0 || value > 9)
                {
                    cout << "Invalid input in file. Please ensure all values are between 0 and 9.\n";
                    return;
                }
                grid[i][j] = value;
            }
        }
        file.close();
    }
    else
    {
        cout << "Invalid choice. Please try again.\n";
    }
}

void displayGrid(const vector<vector<int>> &grid)
{
    cout << "Sudoku Grid:\n";
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (j % 3 == 0 && j != 0)
            {
                cout << " | "; // Add a vertical line every 3 columns
            }
            cout << grid[i][j] << " ";
        }
        cout << endl;
        if ((i + 1) % 3 == 0 && i != 8)
        {
            cout << "------+-------+------\n"; // Add a horizontal line every 3 rows
        }
    }
}

bool validateGrid(const vector<vector<int>> &grid)
{
    // Check if the grid is 9x9
    if (grid.size() != 9 || grid[0].size() != 9)
    {
        return false;
    }

    // Check for duplicates in rows and columns
    for (int i = 0; i < 9; ++i)
    {
        unordered_set<int> rowSet;
        unordered_set<int> colSet;
        for (int j = 0; j < 9; ++j)
        {
            int rowValue = grid[i][j];
            int colValue = grid[j][i];
            if (rowValue != 0 && rowSet.count(rowValue))
            {
                return false;
            }
            if (colValue != 0 && colSet.count(colValue))
            {
                return false;
            }
            rowSet.insert(rowValue);
            colSet.insert(colValue);
        }
    }

    // Check for duplicates in 3x3 subgrids
    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            unordered_set<int> subgridSet;
            for (int k = 0; k < 3; ++k)
            {
                for (int l = 0; l < 3; ++l)
                {
                    int value = grid[i + k][j + l];
                    if (value != 0 && subgridSet.count(value))
                    {
                        return false;
                    }
                    subgridSet.insert(value);
                }
            }
        }
    }

    return true;
}

bool isSafe(const vector<vector<int>> &grid, const unordered_map<int, unordered_set<int>> &rowConstraints, const unordered_map<int, unordered_set<int>> &colConstraints, const unordered_map<int, unordered_set<int>> &boxConstraints, int row, int col, int num)
{
    int boxIndex = (row / 3) * 3 + col / 3;
    return rowConstraints.at(row).count(num) == 0 &&
           colConstraints.at(col).count(num) == 0 &&
           boxConstraints.at(boxIndex).count(num) == 0;
}

void optimizeSolver(vector<vector<int>> &grid)
{
    unordered_map<int, unordered_set<int>> rowConstraints;
    unordered_map<int, unordered_set<int>> colConstraints;
    unordered_map<int, unordered_set<int>> boxConstraints;

    // Initialize constraints
    for (int i = 0; i < 9; ++i)
    {
        rowConstraints[i] = unordered_set<int>();
        colConstraints[i] = unordered_set<int>();
        boxConstraints[i] = unordered_set<int>();
    }

    // Populate constraints
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (grid[row][col] != 0)
            {
                rowConstraints[row].insert(grid[row][col]);
                colConstraints[col].insert(grid[row][col]);
                boxConstraints[(row / 3) * 3 + col / 3].insert(grid[row][col]);
            }
        }
    }

    // Solve the puzzle using the optimized solver
    solveSudoku(grid, rowConstraints, colConstraints, boxConstraints);
}

bool solveSudoku(vector<vector<int>> &grid, unordered_map<int, unordered_set<int>> &rowConstraints, unordered_map<int, unordered_set<int>> &colConstraints, unordered_map<int, unordered_set<int>> &boxConstraints)
{
    vector<pair<int, int>> emptyCells;

    // Collect all empty cells
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (grid[row][col] == 0)
            {
                emptyCells.push_back({row, col});
            }
        }
    }

    // Sort empty cells by the number of possible values (MRV heuristic)
    sort(emptyCells.begin(), emptyCells.end(), [&](const pair<int, int> &a, const pair<int, int> &b)
         {
        int countA = 0, countB = 0;
        for (int num = 1; num <= 9; ++num) {
            if (isSafe(grid, rowConstraints, colConstraints, boxConstraints, a.first, a.second, num)) {
                countA++;
            }
            if (isSafe(grid, rowConstraints, colConstraints, boxConstraints, b.first, b.second, num)) {
                countB++;
            }
        }
        return countA < countB; });

    // Try to fill the empty cells
    for (const auto &cell : emptyCells)
    {
        int row = cell.first;
        int col = cell.second;
        if (grid[row][col] == 0)
        {
            for (int num = 1; num <= 9; ++num)
            {
                if (isSafe(grid, rowConstraints, colConstraints, boxConstraints, row, col, num))
                {
                    grid[row][col] = num;
                    rowConstraints[row].insert(num);
                    colConstraints[col].insert(num);
                    boxConstraints[(row / 3) * 3 + col / 3].insert(num);
                    if (solveSudoku(grid, rowConstraints, colConstraints, boxConstraints))
                    {
                        return true;
                    }
                    grid[row][col] = 0; // Backtrack
                    rowConstraints[row].erase(num);
                    colConstraints[col].erase(num);
                    boxConstraints[(row / 3) * 3 + col / 3].erase(num);
                }
            }
            return false;
        }
    }
    return true;
}

void performanceReport(const vector<vector<int>> &grid)
{
    unordered_map<int, unordered_set<int>> rowConstraints;
    unordered_map<int, unordered_set<int>> colConstraints;
    unordered_map<int, unordered_set<int>> boxConstraints;

    // Initialize constraints
    for (int i = 0; i < 9; ++i)
    {
        rowConstraints[i] = unordered_set<int>();
        colConstraints[i] = unordered_set<int>();
        boxConstraints[i] = unordered_set<int>();
    }

    // Populate constraints
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (grid[row][col] != 0)
            {
                rowConstraints[row].insert(grid[row][col]);
                colConstraints[col].insert(grid[row][col]);
                boxConstraints[(row / 3) * 3 + col / 3].insert(grid[row][col]);
            }
        }
    }

    vector<vector<int>> gridCopy = grid;

    // Measure execution time
    auto start = chrono::high_resolution_clock::now();
    bool solved = solveSudoku(gridCopy, rowConstraints, colConstraints, boxConstraints);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    // Display performance data
    cout << "Performance Report:\n";
    cout << "Execution Time: " << duration.count() << " seconds\n";
    cout << "Time Complexity: O(9^n), where n is the number of empty cells\n";
    cout << "Space Complexity: O(n), where n is the number of empty cells\n";

    if (solved)
    {
        cout << "Puzzle solved successfully!\n";
    }
    else
    {
        cout << "Puzzle is unsolvable.\n";
    }

    // Save performance data for admin's analysis
    ofstream performanceFile("performance_report.txt", ios::app);
    if (performanceFile.is_open())
    {
        performanceFile << "Execution Time: " << duration.count() << " seconds\n";
        performanceFile << "Time Complexity: O(9^n), where n is the number of empty cells\n";
        performanceFile << "Space Complexity: O(n), where n is the number of empty cells\n";
        performanceFile << "Solved: " << (solved ? "Yes" : "No") << "\n\n";
        performanceFile.close();
    }
    else
    {
        cout << "Error opening performance report file.\n";
    }
}

// Placeholder for other functions
void managePuzzles()
{
    // Placeholder implementation
}

void errorHandling(const vector<vector<int>> &grid)
{
    // Placeholder implementation
}
