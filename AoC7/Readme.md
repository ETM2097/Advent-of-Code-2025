# Implementation of Advent of Code 2025: Day 7

## Introduction
This repository contains the implementation of solutions for Day 7 of Advent of Code 2025. The solutions are structured in C++ and utilize various data structures and algorithms to solve the problems presented in the challenge.
Todays challenge is to simulate a ray bouncing through a grid of mirrors that make it duplicate and change directions.

We saw today's challenge as an opportunity to implement the two remaining structures that we wanted to implement for this year: `Graphs` and `HashMaps`.

## Folder Structure
- `data/`: Contains input data files for the challenges.
- `include/`: Contains header files for the project.
- `src/`: Contains the source code files.
    - `Graphs/`: Contains graph-related implementations.
    - `DynamicProgramming/`: Contains dynamic programming solutions.



`Each folder contains its own README.md file with specific details about the implementations within that folder.`

- Here is a `visual representation` of the folder structure:

```
AoC7/
|---data/
|---include/
├---src/
│   ├---Graphs/
│   |---DynamicProgramming/
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

## Conclusions
Today's challenge gave us a great opportunity to implement and utilize the advanced structures that we have studied at class. We chose to implement a `Graph` structure to represent the conections between the cells of the grid and a custom `HashMap` to optimize our dynamic programming solution. And we implemented various resolutions, two with just dynamic programming and two with graphs, in order to prove that one problem has not only one solution.

We are quite happy of the final result that we got on today's challenge because we got a strong example in order to show all our abilitites on data structures and algorithms.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)
