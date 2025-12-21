# Implementation of Part 1 and Part 2 of Day 7 - Advent of Code 2025 with Dynamic Programming

This folder shows the implementation we made for Day 7 of Advent of Code 2025 using Dynamic Programming. The problem involves simulating a laser bouncing through a grid where mirrors cause it to split and change directions. We solved this with an iterative simulation for Part 1 and recursive memoization for Part 2.

Both parts use a direct DP approach without constructing an explicit graph structure, making the implementation simpler and faster to write for competitive programming scenarios.

## AoC7_P1.cpp

This file contains the implementation for Part 1 of Day 7 using an iterative simulation approach. We process the grid row by row, tracking which columns have active lasers at each level. We will show a simple visual representation with a 6 x 6 grid:
```
. . S . . .      . . | . . .
. . ^ . . .      . / ^ \ . .
. ^ . ^ . .      / ^ | ^ \ .
. . . . ^ .      | . | . | .
^ . ^ . . .      | . | . | .
. . . ^ . .      | . | ^ | .        
```

Where `S` is the starting point of the laser, `.` are empty cells, and `^` are mirrors that cause the laser to split. The (|), (/), and (\) symbols represent the direction of the laser after interacting with cells.

### AoC7_P1 Implementation Highlights
- We read the input grid from a file and store it in a 2D vector of strings.
- We use a `vector<int>` named `currentRowVec` to track which columns have active lasers in the current row.
- We use a `HashMap<int, bool>` named `nextSeen` to deduplicate columns for the next row before adding them to `nextRowVec` [info about the HashMap implementation here](../../include/Readme.md#hashmaph).
- We iterate through the grid row by row, processing each active column:
  - If we hit a splitter (`^`), we increment the counter and add two new columns (left and right) to the next row.
  - If we hit an empty cell (`.`) or the start (`S`), we continue downwards to the same column in the next row.
- This level-by-level simulation continues until we reach the last row or run out of active lasers.

#### How does the iterative simulation work?
- We maintain `currentRowVec` (a vector of active columns in the current row) and `nextRowVec` (columns that will be active in the next row).
- For each column in `currentRowVec`:
  - We check the cell type at `grid[row][col]`.
  - Based on the cell type, we determine which columns in the next row will receive lasers.
  - Before adding a column to `nextRowVec`, we check `nextSeen.contains()` to avoid duplicates.
- After processing all columns in the current row, we move to the next row using `currentRowVec.swap(nextRowVec)`.
- We count splitters as we encounter them, which is our answer for Part 1.

Here is the code snippet of the iterative simulation:
```cpp
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
```

Finally, we output the total number of splittings encountered.

## AoC7_P2.cpp

This file contains the implementation for Part 2 of Day 7 using Dynamic Programming with recursive memoization. In this part, we need to count all the different paths the laser could take from the start to the last row. As the number of paths can exceed 2 trillion, we use memoization to avoid recalculating the same subproblems.

We use our custom HashMap (implemented in `../../include/HashMap.h`) to store the memoization results, demonstrating the practical application of our hash map data structure.

### How does DP with Memoization work in this case?
- We define a recursive function `countPaths(row, col)` that returns the number of distinct paths from position `(row, col)` to the last row.
- **Base cases:**
  - If we're out of bounds (`col < 0` or `col >= cols`), return 0 (invalid path).
  - If we've reached the last row (`row == rows - 1`), return 1 (found one valid path).
- **Memoization:** Before computing, we check if we've already calculated `countPaths(row, col)`. If so, we return the stored result.
- **Recursive cases:**
  - If we hit a splitter (`^`), the total paths = paths going left + paths going right:
    ```cpp
    result = countPaths(row + 1, col - 1) + countPaths(row + 1, col + 1);
    ```
  - If we hit an empty cell (`.`) or start (`S`), continue downwards:
    ```cpp
    result = countPaths(row + 1, col);
    ```
  - For any other character, the laser stops, so result = 0.
- We store the result in our HashMap before returning it.

#### Key Encoding for Memoization
Since our HashMap uses a single key type, we convert 2D coordinates `(row, col)` into a unique 1D key:
```cpp
int getKey(int row, int col) {
    return row * cols + col;
}
```

This ensures each grid position has a unique integer identifier for efficient lookup and storage in the HashMap.

Here is the code snippet of the recursive DP with memoization:
```cpp
HashMap<int, long long> memo; // Custom HashMap for memoization

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
```

By implementing recursive DP with memoization, we efficiently counted all unique paths the laser could take to reach the last row, even with trillions of possible paths. The time complexity is O(rows × cols) since each cell is computed at most once and stored in the memo table.
