# Implementation of Advent of Code 2025 Day 5

## Folder Structure
- `texts/`: Contains input data files for the challenges.
- `include/`: Contains header files for the project.
    - `Tree.h`: Implementation of the tree data structure.
- `src/`: Contains the source code files.
    - `AoC5_P1.cpp`: Solution for Part 1 of the challenge.
    - `AoC5_P2.cpp`: Solution for Part 2 of the challenge.

## Building and Running
To build and run the project, we have implemented a Makefile. You can use the following commands:

```bash
make all # To build all solutions
make AoC5_P1 # To build the solution for Part 1
./programs/AoC5_P1
make AoC5_P2 # To build the solution for Part 2
./programs/AoC5_P2
make clean # To clean up the build files
```
## Problem Overview
- The first problem of day 5 (Part 1) consists in finding which ID's from the given input are contained in any of the given intervals. 
- The second problem of day 5 (Part 2) consists in finding how many ID's from the given ranges we have in total.

## Implementation Details
- The solutions for Day 5 are implemented in `AoC5_P1.cpp` for the first problem and `AoC5_P2.cpp` for the second problem.
- We use a unified `Tree` data structure from Tree.h that can store both simple elements and intervals using std::variant. This provides the functionality of an Interval Tree while maintaining the flexibility of a standard Binary Search Tree.

## Tree
- The `Interval Tree` is a data structure that allows us to efficiently store and query intervals. We made it support insertion of intervals and checking for overlaps. The implementation can be found in the `include/Tree.h` file. Here we will explain in detail how it works.

- We implemented a structure called `Interval` to represent an interval with a start and end.

  ```cpp
    template <typename T>
    struct Interval {
        T start; // Start of the interval
        T end;   // End of the interval
    };
  ```

- We used a data structure called `Node` to represent each node in the tree. Each node contains:
    - **data**: that stores the value or interval.
    - **left / right**: pointers to the left and right children.
    - **maxRange**: the maximum value of end in the subtree.

  ```cpp
    struct Node {
        std::variant<T, Interval<T>> data;  //Can store either T or Interval<T>
        Node* left;
        Node* right;
        T maxRange;  //Maximum end value in the subtree


        //Constructor for intervals
        Node(T start, T end, Node* left = nullptr, Node* right = nullptr) :
            data(Interval<T>(start, end)), left(left), right(right), maxRange(end) {}
        
    };
    ```
- Then we implemented the `Tree` class, which contains methods for inserting intervals, checking for overlaps, searching and balancing the tree (for better performance).

- - This class cointains the following methods:
  - `insert`: Inserts a new interval into the tree. Greater intervals go to the right subtree, smaller to the left.
  - `overlaps`: Checks if a given interval overlaps with any interval in the tree. If ID is found in a node, it returns `true`.
  - `searchIntervalContains`: Searches for an ID in the tree. 
  - `buildBalanced`: Balances the tree to ensure optimal performance. More explained below.
  - `buildFromIntervals`: Builds a balanced tree from a list of intervals. More explained below.

  #### -> insert() 
  Here is the implementation of the `insert` method, it is like a typical binary search tree insertion, but we also update the `maxEnd` value of each node.

  ```cpp
    //Unified insert for intervals (overload)
    Node<T>* insert(Node<T>* n, const Interval<T>& interval) {
        // Base case: If the tree is empty, return a new node
        if (n == nullptr) {
            return new Node<T>(interval);
        }
        // Otherwise, recur down the tree
        if (interval.start < n->getStart()) {
            n->left = insert(n->left, interval);
        } else {
            n->right = insert(n->right, interval);
        }
        // Here we update the max value of the ancestor node
        // Here we had a critical bug before, we corrected it by ensuring we consider both left and right children
        updateMaxRange(n);
        // Finally return the node pointer
        return n;
    }

        // Here we update the max value of the ancestor node
    // Here we had a critical bug before, we corrected it by ensuring we consider both left and right children
    void updateMaxRange(Node<T>* n) const {
        if (n == nullptr) return;
        n->maxRange = n->getEnd();
        if (n->left) n->maxRange = std::max(n->maxRange, n->left->maxRange);
        if (n->right) n->maxRange = std::max(n->maxRange, n->right->maxRange);
    }
  ```
  #### -> overlaps()
  Here is the implementation of the `overlaps` method, which checks if a given interval overlaps with any interval in the tree.

  ```cpp
    //Check if two intervals overlap
    bool overlaps(const Interval<T>& a, const Interval<T>& b) const {
        return (a.start <= b.end && b.start <= a.end);
    }
  ```
  It is quite a simple check, we just see it the ID is between the start and end of the interval.

  #### -> searchIntervalContains()
  Here is the implementation of the `searchIntervalContains` method, which searches for an ID in the tree recursively.
  ```cpp
   //Search if a point falls within any interval
    bool searchIntervalContains(Node<T>* n, T point) const {
        if (n == nullptr) return false;
        
        //Check if the current interval contains the point
        if (n->isInterval()) {
            Interval<T> range = n->getRange();
            if (range.start <= point && point <= range.end) {
                return true;
            }
        }
        
        //If the left child exists and its maxRange >= point, search there
        if (n->left != nullptr && n->left->maxRange >= point) {
            if (searchIntervalContains(n->left, point)) {
                return true;
            }
        }
        
        //Search in the right child
        return searchIntervalContains(n->right, point);
    }
  ```
  
  #### buildBalanced
  Here is the implementation of the `buildBalanced` method, which balances the tree to ensure optimal performance. We decided to implement this for making the cost of operations O(log n) on average, because unbalanced trees can degrade to O(n). 
  We had to research how to do this, and we found that the best way was to sort the intervals and then build the tree recursively by choosing the median interval as the root at each step.

  This is done in two steps:
  1. We perform an in-order traversal of the tree to collect all intervals in a sorted vector (we use the function `buildBalancedFromIntervals()` for sorting).
  2. We recursively build a balanced tree from the sorted intervals by choosing the median interval as the root at each step (this function).

  - **Note**: This was done in order to use divide and conquer in order to fullfill all the assigment requirements, but improved performance as well.

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

    //Build balanced tree from intervals
    Node<T>* buildBalancedFromIntervals(std::vector<Interval<T>>& intervals, int start, int end) {
        // Base case: no elements to add in this range (ended recursion because we are out of bounds)
        if (start > end) return nullptr;

        // 1. Find the middle element
        int mid = start + (end - start) / 2;

        // 2. Create a node with the middle interval (this becomes the main of subtree)
        Node<T>* node = new Node<T>(intervals[mid]);

        // 3. Recursively build left subtree from elements smaller than mid
        node->left = buildBalancedFromIntervals(intervals, start, mid - 1);

        // 4. Recursively build right subtree from elements greater than mid
        node->right = buildBalancedFromIntervals(intervals, mid + 1, end);

        // 5. Calculate maxRange for this node (maximum end value in entire subtree) this is also calculated recursively for each node
        updateMaxRange(node);

        // We return the constructed node, because it will replace the previous subtree root
        return node;
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

//Build balanced tree from a vector of intervals
void buildFromIntervals(std::vector<Interval<T>>& intervals) {
    if (intervals.empty()) return;

    //Sort by interval start using lambda
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval<T>& a, const Interval<T>& b) {
            return a.start < b.start;
        });

    deleteAll(root);
    root = buildBalancedFromIntervals(intervals, 0, intervals.size() - 1);
    nelem = intervals.size();
}

```
As you can see, we used a lambda function to define the comparison logic for sorting the intervals based on their start values. Because if we had not used a lambda function, we would have to implement a separate function in order to implement that comparison.

So this function first sorts all the intervals, stored in a vector at the main function, an then deletes de previous tree and builds a new balanced tree from the sorted intervals using the `buildFromIntervals` method, finally asigning the new tree to the `root` pointer of the `Tree` class.

## First Solution AoC5_P1.cpp
- The solution for the first part of day 5 is implemented in the `AoC5_P1.cpp` file. The main steps of the solution are:
  1. Read the input intervals from the `texts/AoC5.txt` file.
  2. Insert each interval into the `Tree` using the `buildFromIntervals` method.
  3. For each ID in the input, check if it overlaps with any interval in the tree using the `searchIntervalContains` method.
  4. Count and output the number of IDs that overlap with any interval.

## Second Solution AoC5_P2.cpp
- The solution for the second part of day 5 is implemented in the `AoC5_P2.cpp` file. The main steps of the solution are:
  1. Read the input intervals from the `texts/AoC5.txt` file.
  2. Insert each interval into the `Interval Vector`.
  3. Now we have implemented a new method called `countUniqueIDs`, which counts how many IDs from a given range overlap with any interval in the vector.
  
  #### How do countUniqueIDs() works?
    1. First we sort using the same method as in `buildFromIntervals()`.
    2. Then we iterate through the sorted intervals and merge any overlapping intervals into a single interval.
    3. Finally, we calculate the total number of unique IDs covered by the merged intervals.

```cpp
    // Function to count unique IDs from a list of intervals
    long long countUniqueIDs(std::vector<Interval<long long>>& intervals) {
        if (intervals.empty()) return 0;

        // 1. Sort intervals by start value (same as in buildBalanced)
        std::sort(intervals.begin(), intervals.end(),
            [](const Interval<long long>& a, const Interval<long long>& b) {
                return a.start < b.start;
            });

        // 2. Merge overlapping intervals, for not double counting
        std::vector<Interval<long long>> merge;
        merge.push_back(intervals[0]);

        // Here we iterate though sorted intervals and merge them
        for (size_t i = 1; i < intervals.size(); i++) {
            Interval<long long>& last = merge.back();
            const Interval<long long>& current = intervals[i];

            // Check if current overlaps or is adjacent to last
            if (current.start <= last.end + 1) {
                // Merge: extend the end if needed
                last.end = std::max(last.end, current.end);
            } else {
                // No overlap: add as new interval
                merge.push_back(current);
            }
        }

        // 3. Here we count total unique IDs
        long long total = 0;
        for (const Interval<long long>& interval : merge) {
            total += (interval.end - interval.start + 1);
        }

        return total;
    }
``` 

## Conclusions

Today was a very interesting day, at least the first part. We learned how to implement a `Tree`, which can use a variant known as an interval tree, because its data type is different from a simple element. We also learned how to balance a binary search tree using an ordered array and recursion, which was quite fun to implement. In the second part, we didn't learn anything new, but we implemented a very efficient algorithm for counting unique IDs in ranges by merging intervals.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)