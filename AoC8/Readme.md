# Implementation of Advent of Code 2025: Day 8

## Introduction
This repository contains the implementation of solutions for Day 8 of Advent of Code 2025. The solutions are structured in C++ and utilize various data structures and algorithms to solve the problems presented in the challenge.
Today's challenge is to connect electrical junction boxes in a 3D space. Given N boxes, we connect the 1000 closest pairs using the Euclidean distance, forming electrical circuits. 


We saw today's challenge as an opportunity to implement the structure `Graphs`.

## Folder Structure
- `data/`: Contains input data files for the challenges.
- `include/`: Contains header files for the project.
- `src/`: Contains the source code files.

`Each folder contains its own README.md file with specific details about the implementations within that folder.`

- Here is a `visual representation` of the folder structure:

```
AoC8/
|---data/
|---include/
├---src/
```
## Building and Running
To build and run the project, we have implemented a Makefile. You can use the following commands:

```bash
make all # To build all solutions
make AoC8_P1 # To build the first part of challenge
make AoC8_P2 # To build the second part of challenge
make clean # To clean up the build files
```

## Implementation Highlights
- **Graph Implementation**: The graph is constructed from the connections between the boxes.
- **Union Find algorithm**: We have implemented union find algorithm or DSU to conect the boxes in the graph.


## Conclusions
Today's challenge gave us a great opportunity to implement and utilize the advanced structures that we have studied at class. We chose to implement a `Graph` structure to represent the conections between the junction boxes of the grid and a algorithm `Union find` to optimize our connection between junction boxes in the solution. 

We are quite happy of the final result that we got on today's challenge because we got a strong example in order to show all our abilities on data structures and algorithms.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)
