# Implementation of Advent of Code 2025: Day 11

## Introduction
This repository contains the implementation of solutions for Day 11 of Advent of Code 2025. The solutions are structured in C++ and utilize advanced graph algorithms and data structures to solve the problems presented in the challenge.

Today's challenge involves analyzing a directed acyclic graph (DAG) to count paths between nodes under specific conditions. The problem is solved using Depth-First Search (DFS) with memoization for efficiency.

This follows a similar approach to Day 7, but here we implemented a more advanced graph structure and improved our custom hash map.

The description of each part of the implementation is in the corresponding folders.

## Folder Structure
- `text/`: Contains input data files for the challenges.
- `include/`: Contains header files for the project.
    - `Graph.h`: Implementation of the graph data structure and algorithms.
    - `HashMap.h`: Custom hash map implementation used for memoization.
- `src/`: Contains the source code files.
    - `AoC11_P1.cpp`: Solution for Part 1 of the challenge.
    - `AoC11_P2.cpp`: Solution for Part 2 of the challenge.

## Building and Running
To build and run the project, we have implemented a Makefile. You can use the following commands:

```bash
make all # To build all solutions
make AoC11_P1 # To build the solution for Part 1
make AoC11_P2 # To build the solution for Part 2
make clean # To clean up the build files
```

## Implementation Highlights
- **Graph Implementation**: The graph is constructed using adjacency lists, and methods for DFS and memoization are implemented to efficiently count paths in the DAG.
- **Custom HashMap**: A custom hash map is used for memoization to optimize the graph traversal algorithms. This hash map supports custom hash functions for tuples and pairs.
- **DFS with Memoization**: The solutions use DFS with memoization to avoid redundant computations and improve performance.

## Alternative Solutions
While we focused on DFS with memoization for this challenge, alternative approaches could include:
- **Dynamic Programming**: A bottom-up dynamic programming approach could be used to count paths, we ended not implementing it as we found graph traversal was more intuitive for this problem.
- **Breadth-First Search (BFS)**: Although less common for path counting in DAGs, BFS could be adapted for certain scenarios. We implemented BFS in our Graph class for completeness, but it was not used in this challenge.

## Conclusions
Day 11's challenge provided an excellent opportunity to implement and utilize advanced graph algorithms and data structures. The use of DFS with memoization allowed us to solve the problem efficiently, even for large graphs. The custom `HashMap` further optimized the solution by enabling fast lookups and storage of intermediate results.

We are proud of the final implementation, as it demonstrates our ability to apply advanced algorithms and data structures to solve complex problems.

## Authors
- Enric Talens (Group Leader, Implementer, and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer, and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)
