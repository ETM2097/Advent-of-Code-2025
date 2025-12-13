# Implementation of Advent of Code 2025 Day 5

## Folder Structure
- `src/`: Contains the main source code files for solving the puzzles.
- `include/`: Contains header files and implementations for data structures used in the solutions.
- `texts/`: Contains input text files for the puzzles.

```
include
    |-- IntervalTree.cpp -> Implementation of the Interval Tree data structure
    |-- IntervalTree.h -> Header file for the Interval Tree data structure
src
    |-- AoC5.cpp -> Solution for Part 1 of Day 5
    |-- AoC5_P2.cpp -> Solution for Part 2 of Day 5
texts
    |-- AoC5.txt
    |-- AoC5_2.txt
```
## How to Run
- We implemented a Makefile to compile the code. To compile and run the solutions, use the following commands in the terminal at the root of the project:

```bash
make First # Compiles AoC5.cpp
./AoC5 # Runs the solution for Part 1

make Second # Compiles AoC5_P2.cpp
./AoC5_P2 # Runs the solution for Part 2
```

## Problem Overview
- The first problem of day 5 (Part 1) consists in finding wich ID's from the given input are contained in any of the given intervals. 
- The second problem of day 5 (Part 2) consists in finding how many ID's from the given ranges we have in total.

## Implementation Details
- The solutions for Day 5 are implemented in `AoC5_P1.cpp` for the first problem and `AoC5_P2.cpp` for the second problem.
- Today we saw that this problem could be efficiently solved using a binary search tree variant called an `Interval Tree`. The implementation of this data structure can be found in `IntervalTree.cpp` and `IntervalTree.h`. This problem could also be solved via binary searchon sorted intervals, but we prefered to try to implement this data structure for educational purposes.

## Interval Tree
- The `Interval Tree` is a data structure that allows us to efficiently store and query intervals. We made it support insertion of intervals and checking for overlaps. The implementation can be found in the `include/IntervalTree.cpp` and `include/IntervalTree.h` files. Here we will explain in detail how it works.

- We implemented a structure called `Interval` to represent an interval with a start and end.

  ```cpp
    struct Interval {
        long long start; // Start of the interval
        long long end;   // End of the interval
    };
  ```

- We used a data structure called `IntervalNode` to represent each node in the tree. Each node contains:
  - `start` and `end`: The start and end of the interval. Implemented as a Interval `range`.
  - `maxEnd`: The maximum end value in the subtree rooted at this node.
  - `left` and `right`: Pointers to the left and right child nodes.

  ```cpp
    struct IntervalNode {
        Interval range; // The interval represented by this node
        long long maxEnd;  // The maximum end value in the subtree rooted at this node
        IntervalNode* left;  // Pointer to the left child
        IntervalNode* right; // Pointer to the right child

        IntervalNode(long long start, long long end)
            : range{start, end}, maxEnd(end), left(nullptr), right(nullptr) {}
    };
    ```
- Then we implemented the `IntervalTree` class, which contains methods for inserting intervals, checking for overlaps, searching and balancing the tree (for better performance).

- - This class cointains the following methods:
  - `insert`: Inserts a new interval into the tree. Greater intervals go to the right subtree, smaller to the left.
  - `overlaps`: Checks if a given interval overlaps with any interval in the tree. If ID is found in a node, it returns `true`.
  - `searchTree`: Searches for an ID in the tree. 
  - `buildBalanced`: Balances the tree to ensure optimal performance. More explained below.
  - `buildFromIntervals`: Builds a balanced tree from a list of intervals. More explained below.

  #### -> insert() 
  Here is the implementation of the `insert` method, it is like a typical binary search tree insertion, but we also update the `maxEnd` value of each node.

  ```cpp
  IntervalNode* IntervalTree::insert(IntervalNode* node, long long start, long long end) {
    // If the tree is empty, return a new node that will be the root
    if (node == nullptr) {
        return new IntervalNode(start, end);
    }
    // Otherwise, recur down the tree, greater intervals go to the right subtree, smaller to the left
    if (start < node->range.start) {
        node->left = insert(node->left, start, end);
    } 
    else {
        node->right = insert(node->right, start, end);
    }

    // Here we update the max value of the ancestor node
    if (node->maxRange < end) {
        node->maxRange = end;
    }
    // Finally return the node pointer (this ensures the tree structure is maintained)
    return node;
  }
  ```
  #### -> overlaps()
  Here is the implementation of the `overlaps` method, which checks if a given interval overlaps with any interval in the tree.

  ```cpp
  bool IntervalTree::overlaps(const Interval& a, long long ID) const {
    return (a.start <= ID && ID <= a.end);
  }
  ```
  It is quite a simple check, we just see it the ID is between the start and end of the interval.

  #### -> searchTree()
  Here is the implementation of the `searchTree` method, which searches for an ID in the tree recursively.
  ```cpp
  bool IntervalTree::search(IntervalNode* node, long long ID) const {
    // If the node is null, return false (we reached a leaf)
    if (node == nullptr) {
        return false;
    }
    // If the current node overlaps with the ID, return true
    if (overlaps(node->range, ID)) {
        return true;
    }
    // If the left child exists and its maxEnd is greater than or equal to ID, search the left subtree
    if (node->left != nullptr && node->left->maxEnd >= ID) {
        if (search(node->left, ID)) {
            return true;
        }
    }
    // If not, search the right subtree
    return search(node->right, ID);
  }
  ```
  We would like to highlight that this function uses `Divide and Conquer` technique to efficiently search the tree, as we skip entire subtrees on each iteration.

  #### -> buildBalanced()
  Here is the implementation of the `buildBalanced` method, which balances the tree to ensure optimal performance. We decided to implement this for making the cost of operations O(log n) on average, because unbalanced trees can degrade to O(n). 
  We had to research how to do this, and we found that the best way was to sort the intervals and then build the tree recursively by choosing the median interval as the root at each step.

  This is done in two steps:
  1. We perform an in-order traversal of the tree to collect all intervals in a sorted vector (we use the function `buildFromInterval()` for sorting).
  2. We recursively build a balanced tree from the sorted intervals by choosing the median interval as the root at each step (this function).

  A visual example of balancing a tree could be this one:

  ```
      Unbalanced Tree:                Balanced Tree:
            5                              20
             \                            /  \
              10                        10    30
               \                       /  \   / \
                15                    5  15 25  35
                 \
                  20
                   \
                    25
                     \
                      30
                       \
                        35
  ```
  Here we can see that we sorted the intervals and chose the `median (20)` as the root, making now a balanced tree that has the same amount of nodes in the left and right subtrees. This balanced tree is optimal anyway, the balanced tree can also vary depending on the input intervals and not be perfectly balanced like this example.

Here is the code for building the balanced tree:
```cpp
IntervalNode* IntervalTree::buildBalanced(std::vector<Interval>& intervals, int start, int end) {
    // Base case: no elements to add in this range (ended recursion because we are out of bounds)
    if (start > end) return nullptr;

    // 1. Find the middle element
    int mid = start + (end - start) / 2;
    
    // 2. Create a node with the middle interval (this becomes the main of subtree)
    IntervalNode* node = new IntervalNode(intervals[mid].start, intervals[mid].end);

    // 3. Recursively build left subtree from elements smaller than mid
    node->left = buildBalanced(intervals, start, mid - 1);
    
    // 4. Recursively build right subtree from elements greater than mid
    node->right = buildBalanced(intervals, mid + 1, end);

    // 5. Calculate maxRange for this node (maximum end value in entire subtree) this is also calculated recursively for each node
    node->maxRange = node->range.end;
    if (node->left && node->left->maxRange > node->maxRange) { // If left child exists and has a greater maxRange
        node->maxRange = node->left->maxRange; // Update maxRange
    }
    if (node->right && node->right->maxRange > node->maxRange) { // If right child exists and has a greater maxRange
        node->maxRange = node->right->maxRange; // Update maxRange
    }

    return node; // We return the constructed node, because it will replace the previous subtree root
}
```

#### -> buildFromIntervals()
This function takes the vector of intervals and builds a balanced tree from them. It first sorts the intervals and then calls the `buildBalanced` function.

In this function, we use a `lambda function` to sort the intervals based on their start values. We are going to explain it here as is out of the contents of this assignature, but it is widely used in modern C++.

  #### Lambda Functions in C++
  Lambda functions are a way to define anonymous functions in C++. They are useful for short functions that are not going to be used anywhere else. The syntax for a lambda function is as follows:

  ```cpp
  [capture](parameters) -> return_type {
      // function body
  }
  ```
  - `capture`: This is where is specified which variables from the surrounding scope are accessible inside the lambda. It can be empty `[]`, or it can capture variables by value `[=]` or by reference `[&]`.
  - `parameters`: This is where the parameters of the function are defined, similar to regular functions.
  - `return_type`: This is optional and specifies the return type of the function.
  - `function body`: This is where the code of the function is written.

Here is the implementation of the `buildFromIntervals` method we used:
```cpp
void IntervalTree::buildFromIntervals(std::vector<Interval>& intervals) {
    if (intervals.empty()) return;

    // Sort by start value using lambda
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

    deleteTree(main);
    main = buildBalanced(intervals, 0, intervals.size() - 1);
}
```
As you can see, we used a lambda function to define the comparison logic for sorting the intervals based on their start values. Because if we had not used a lambda function, we would have to implement a separate function in order to implement that comparison.

So this fucntion first sorts all the intervals, stored in a vector at the main fucntion, an then deletes de previous tree and builds a new balanced tree from the sorted intervals using the `buildBalanced` method, finalli asigning the new tree to the `main` pointer of the `IntervalTree` class.

## First Solution AoC5_P1.cpp
- The solution for the first part of day 5 is implemented in the `AoC5.cpp` file. The main steps of the solution are:
  1. Read the input intervals from the `texts/AoC5.txt` file.
  2. Insert each interval into the `IntervalTree` using the `buildFromIntervals` method.
  3. For each ID in the input, check if it overlaps with any interval in the tree using the `searchTree` method.
  4. Count and output the number of IDs that overlap with any interval.

## Second Solution AoC5_P2.cpp
- The solution for the second part of day 5 is implemented in the `AoC5_P2.cpp` file. The main steps of the solution are:
  1. Read the input intervals from the `texts/AoC5_2.txt` file.
  2. Insert each interval into the `Interval Vector`.
  3. Now we have implemented a new method called `countOverlapsInRange`, which counts how many IDs from a given range overlap with any interval in the vector.
  
  #### How do countUniqueIDs() works?
    1. First we sort using the same method as in `buildFromIntervals()`.
    2. Then we iterate through the sorted intervals and merge any overlapping intervals into a single interval.
    3. Finally, we calculate the total number of unique IDs covered by the merged intervals.

    ```cpp
    long long countUniqueIDs(std::vector<Interval>& intervals) {
    if (intervals.empty()) return 0;

    // 1. Sort intervals by start value (same as in buildBalanced)
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

    // 2. Merge overlapping intervals
    std::vector<Interval> merge;
    merge.push_back(intervals[0]); // We push the first interval to start merging

    // Here we iterate though sorted intervals and merge them
    for (size_t i = 1; i < intervals.size(); i++) { // We start from the second interval
        Interval& last = merge.back(); // Last merged interval
        const Interval& current = intervals[i]; // Current interval to check

        // Check if current overlaps or is adjacent to last
        if (current.start <= last.end + 1) {
            // Merge: extend the end if needed
            last.end = std::max(last.end, current.end);
        } else {
            // No overlap: add as new interval
            merge.push_back(current);
        }
    }

    // 3. Here we count total unique IDs with a simple iterator that gets the length of each merged interval
    long long total = 0;
    for (const Interval& interval : merge) {
        total += (interval.end - interval.start + 1);
    }

    return total;
    }
    ``` 
## Conclusions

Today was a very interesting day, at least, the first part. We learned about how to implement an `Interval Tree`, that technically is a variant of a binary search tree but with a differnt type of data. We also learned how to balance a binary search tree using a sorted array and recursion, which was quite fun to implement. For the second part, we learned nothing new, but we implemented a very efficient algorithm to count unique IDs in ranges by merging intervals.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)