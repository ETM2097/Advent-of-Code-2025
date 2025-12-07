// This is the 7'th day implementation of the Advent of Code 2025 challenge.
// Todays consist in counting how many times a laser is divided.
// The laser start from the character (S) and allways goes down in empty spaces (.).
// When it encounters a spliter (^), it divides in two lasers going inmediately left and right and keeping going down.
// The laser stops when it encounters the last line.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set> // Used for just processing one time each columns per row

using namespace std;

int main() {
    ifstream inputFile("AoC7.txt"); // We get the input from this file
    string line;
    vector<string> grid; // Here we will store the grid

    // We read the input file and store it in a grid
    while (getline(inputFile, line)) {
        grid.push_back(line);
    }
    inputFile.close();

    int rows = grid.size();
    int cols = grid[0].size();
    int totalSplittings = 0;

    // We find the starting position of the laser (S)
    int startCol = -1;
    for (int col = 0; col < cols; ++col) {
        if (grid[0][col] == 'S') {
            startCol = col;
            break;
        }
    }

    // Here we track which columns have lasers in the current row
    set<int> currentRow; // Using a set to avoid duplicates
    currentRow.insert(startCol); // Start from the position of 'S'

    // Here we process each row
    for (int row = 0; row < rows && !currentRow.empty(); ++row) {
        set<int> nextRow;

        for (int col : currentRow) {
            if (col < 0 || col >= cols) continue;  // Out of bounds

            char cell = grid[row][col];
            if (cell == '^') {
                // Count this splitter as hit
                totalSplittings += 1;
                // Send lasers to col-1 and col+1 in the next row
                if (row + 1 < rows) {
                    nextRow.insert(col - 1);
                    nextRow.insert(col + 1);
                }
            } else if (cell == '.' || cell == 'S') {
                // Continue downwards
                if (row + 1 < rows) {
                    nextRow.insert(col);
                }
            }
            // If it's any other character, the lasers stop
            
        }

        currentRow = move(nextRow);
    }

    cout << "Total number of splittings: " << totalSplittings << endl;

    return 0;
}