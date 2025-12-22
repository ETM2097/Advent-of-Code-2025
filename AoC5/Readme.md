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
make First # To build the solution for Part 1
./programs/AoC5_P1
make Second # To build the solution for Part 2
./programs/AoC5_P2
make clean # To clean up the build files
```
## Problem Overview
- The first problem of day 5 (Part 1) consists in finding which ID's from the given input are contained in any of the given intervals. 
- The second problem of day 5 (Part 2) consists in finding how many ID's from the given ranges we have in total.

## Implementation Details
- The solutions for Day 5 are implemented in `AoC5_P1.cpp` for the first problem and `AoC5_P2.cpp` for the second problem.
- We use a unified `Tree` data structure from Tree.h that can store both simple elements and intervals using std::variant. This provides the functionality of an Interval Tree while maintaining the flexibility of a standard Binary Search Tree.

## Alternative Approaches
- For the first part, we have discussed to use a simple sorted vector of intervals and performed binary search for each ID. However, this would have resulted in a time complexity of `O(n log n)` for building the vector and `O(m log n)` for searching m IDs, leading to a total of `O((n + m) log n)`. Even if it was simpler and a clear use of divide and conquer, we opted to use the Tree structure to practice tree implementations and for better average-case performance. In order to solve our lack of divide and conquer in the tree, we implemented the balancing method, making our tree implementation even more efficient, as our tree operations now run in `O(log n)` on average. 

## Conclusions

Today was a very interesting day, especially during the first part. We successfully implemented a versatile `Tree` structure that combines the functionality of a Binary Search Tree and an Interval Tree using `std::variant`. This allowed us to handle both simple elements and intervals efficiently. Additionally, we learned how to balance a binary search tree using an ordered array and recursion. This also gave us the opportunity to learn and apply the divide-and-conquer paradigm, improving our understanding of efficient algorithm design.

The balancing method significantly improved the performance of our tree operations, reducing the average-case time complexity to `O(log n)`. This optimization ensured that our solution was not only correct but also efficient, meeting the requirements of the challenge and requirements for qualification.

For the second part, we learned nothing new, we designed a straightforward solution that leveraged our existing tree implementation to count the total number of IDs within the specified ranges. Quite simple but effective.

Overall, the day provided valuable insights into advanced tree implementations, balancing techniques, and their applications in solving problems.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)