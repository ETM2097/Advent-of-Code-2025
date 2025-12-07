# Implementation of Advent of Code 2025: Day 7

## Introduction
This repository contains the implementation of solutions for Day 7 of Advent of Code 2025. The solutions are structured in C++ and utilize various data structures and algorithms to solve the problems presented in the challenge.
Todays challenge is to simulate a ray bouncing through a grid of mirrors that make it duplicate and change directions.

We saw today's challenge as an opportunity to implement the two remaining structures that we wanted to implement for this year: `Graphs` and `HashMaps`.

## Folder Structure
- `src/`: Contains the source code files.
    - `Graphs/`: Contains graph-related implementations.
    - `DynamicProgramming/`: Contains dynamic programming solutions.
- `include/`: Contains header files for the project.
- `data/`: Contains input data files for the challenges.

`Each folder contains its own README.md file with specific details about the implementations within that folder.`

- Here is a `visual representation` of the folder structure:

```
AoC7/
├---src/
│   ├---Graphs/
│   |---DynamicProgramming/
|---include/
|---data/
```
## Building and Running
To build and run the project, we have implemented a Makefile. You can use the following commands:

```bash
make all # To build all solutions
make First_DP # To build the first dynamic programming solution
make Second_DP # To build the second dynamic programming solution
make First_Graph # To build the first graph solution
make Second_Graph # To build the second graph solution
make clean # To clean up the build files
```

## Implementetion Highlights
- **Graph Implementation**: The graph is constructed from a grid representation, where each cell can contain (S, . or ^), it implements methods for BFS and DFS algorithms.
- **Dynamic Programming**: We have implemented dynamic programming by developing our own HashMap, first we used the map library, but after we decided to implement our own HashMap.
- **Custom HashMap**: As last part, we decided to implement our own HasMap to optimize the graph solution. Later we used this HashMap in the dynamic programming solution as well by tweaking a bit the program.