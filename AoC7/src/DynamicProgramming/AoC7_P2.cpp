#include "../../include/HashMap.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

vector<string> grid;
int rows, cols;
HashMap<int, long long> memo; // Use your custom HashMap

// Convert (row, col) to a unique int key for utilizing it in our HashMap
int getKey(int row, int col) {
    return row * cols + col;
}

long long countPaths(int row, int col) {
    // Base case: out of bounds
    if (col < 0 || col >= cols) return 0;
    
    // Base case: reached the last row
    if (row == rows - 1) return 1;
    
    // Check if already computed
    int key = getKey(row, col);
    if (memo.contains(key)) return memo.get(key);
    
    char cell = grid[row][col];
    long long result = 0;
    
    if (cell == '^') {
        // Split: go left and right
        result = countPaths(row + 1, col - 1) + countPaths(row + 1, col + 1);
    } else if (cell == '.' || cell == 'S') {
        // Continue downwards
        result = countPaths(row + 1, col);
    }
    // Other characters: laser stops, result stays 0
    
    memo.set(key, result); // Store in memoization map
    return result;
}
int main() {
    ifstream inputFile("data/AoC7.txt");
    string line;

    while (getline(inputFile, line)) {
        grid.push_back(line);
    }
    inputFile.close();

    rows = grid.size();
    cols = grid[0].size();

    // Find the starting position of the laser (S)
    int startCol = -1;
    for (int col = 0; col < cols; ++col) {
        if (grid[0][col] == 'S') {
            startCol = col;
            break;
        }
    }

    long long totalPaths = countPaths(0, startCol);
    cout << "Total number of different ways: " << totalPaths << endl;

    return 0;
}