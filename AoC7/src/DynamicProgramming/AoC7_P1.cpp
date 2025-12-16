// This is the 7'th day implementation of the Advent of Code 2025 challenge.
// Todays consist in counting how many times a laser is divided.
// The laser start from the character (S) and allways goes down in empty spaces (.).
// When it encounters a spliter (^), it divides in two lasers going inmediately left and right and keeping going down.
// The laser stops when it encounters the last line.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// #include <set> Changed approach
#include "../../include/HashMap.h"

using namespace std;

// IMPORTANT, changed the first approach of set to the custom HashMap

int main() {
    ifstream inputFile("data/AoC7.txt"); // We get the input from this file
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
    vector<int> currentRowVec;

    currentRowVec.push_back(startCol); // We start with the laser at the starting column

    for (int row = 0; row < rows && !currentRowVec.empty(); ++row) {
        vector<int> nextRowVec; // Here we store the next row lasers
        HashMap<int, bool> nextSeen; // To avoid duplicates in the next row we now use our HashMap

        for (int col : currentRowVec) { // We process each laser in the current row
            if (col < 0 || col >= cols) continue;
            char cell = grid[row][col];

            if (cell == '^') {
                if (row + 1 < rows) {
                    int a = col - 1, b = col + 1;
                    if (!nextSeen.contains(a)) {  // We check if we have already seen 'a' using contains()
                        nextSeen.set(a, true); // We mark it as seen
                        nextRowVec.push_back(a); // We push the new laser to the next row
                    }
                    if (!nextSeen.contains(b)) { // Same with b
                        nextSeen.set(b, true); 
                        nextRowVec.push_back(b); 
                    }
                }
                totalSplittings++; // we hit a splitter, then we count it
            } else if (cell == '.' || cell == 'S') {
                if (row + 1 < rows) {
                    if (!nextSeen.contains(col)) { nextSeen.set(col, true); nextRowVec.push_back(col); }
                }
            }
        }

        currentRowVec.swap(nextRowVec); // Here we move to the next row
    }

    cout << "Total number of splittings: " << totalSplittings << endl;

    return 0;
}