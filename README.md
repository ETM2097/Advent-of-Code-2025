# Advent of code 2025 |Long Long and the Shorts|

## Introduction
This is the repository of the Advent of Code challenges of 2025, here we are showing the implementations that we chose for each day.
There is a folder containing the impementation and technical explanation of each day, where we discuss the details of each implementation

We wanted to make this repository as reusable as possible, so we invested a lot of time in creating general purpose data structures and algorithms that could be used in multiple days, specially in the last days of the challenge.

## Folder structure of each day
For this challenge, we chose to keep the project modular and organized, in order to make us able to reimplement and reuse code easily. We created a folder for each day of the challenge, where we implemented the solution(s) for that specific day.
Each day folder contains the following structure:
- `data/`: Contains the input data files used for testing the implementation.
- `include/`: Contains any additional headers or libraries used in the implementation.
- `src/` : Contains the source code of the implementation, with a subfolder for each approach used (if more than one).
- `Readme.md` : Contains the explanation of the implementation, the data structures and algorithms used, and any other relevant information about the day. (Days 7 and 11 have subfolders with their own README files for each approach used and the root README explains the overall approach).

In the next section we will highlight the days that we consider more relevant in terms of data structures and algorithms used. But we will encourage you to check the `root Readme.md` of each day for more details about the implementation and the approach used, as we discuss time complexities and comparissons. Please, check the `Readme.md`of each folder as well as there is extra information about the implementation that you may find interesting or helpful.


## Days chosen for presentation
 We had a lot of fun developing the code for the challenges, but the main motivation that made us decided to present the following implementations, was that these ones were the ones that made us really develop our skills and knowledge about data structures and algorithms. We believe that these implementations showcase our ability to implement and utilize advanced data structures and algorithms to solve complex problems efficiently. 
 The problems that we consider more relevant for this purpose are:

- [AoC5](AoC5) : We would like to `present just the first part` of this day where whe implemented a `Tree` class and used it in order to resolve the challenge, we also chose to use `Divide and Conquer` in order to re-balance the tree. We would like to to check [this implementation](AoC5/src/AoC5_P1.cpp) for the tree usage example. Also, please check the [README](AoC5/src/Readme.md) of that day for more details about the implementation, and also [the balancing algorithm](AoC5/Readme.md#buildbalanced) to check the divide and conquer technique. 

- [AoC7](AoC7) : We chose to `present the first part` here we implemented a `HashMap` in order to check duplicity of element efficiently ([check the implementation](AoC7/src/DynamicProgramming/AoC7_P1.cpp), and also you can check [it's description](AoC7/src/DynamicProgramming/Readme.md#AoC_P1.cpp)), and `also the second part` where we used `Dynamic Programming` in order to solve the second part of this chalenge [implementation here](AoC7/src/DynamicProgramming/AoC7_P2.cpp), as it proved to be the most efficient and straightforward way to solve it. Please check [this README](AoC7/src/DynamicProgramming/Readme.md#AoC7_P2.cpp) for more details about the DP with memoization implementation.

    - **Note**: We would also like to highlight that while we were discussing about what was the best approach, we designed another solution with `Graphs` and `DFS with memoization` and created a `Graph class`, which can be found [here](AoC7/src/Graphs), and also explained in [this README](AoC7/src/Graphs/Readme.md). You are more than welcome to check it out, even if is is not the main solution we present for this day.

- [AoC11](AoC11) :Here we present the whole day where we greatly improved the `HashMap` and `Graph` classes from Day 7 in order to solve more complex graph path counting problems. We would like to highlight that this is the last day we worked on, so we invested more time in making the code as reusable as possible.

- [INCLUDE](INCLUDE) : Here we present the reusable code that we developed during all the challenge, it evolved as we kept improving and generalizing it. We believe this code could be useful for future and past challenges, so we decided to make it as general and functional as possible. We would like you to take a look at it, even if it is not a direct solution for any specific challenge, it gives a powerful framework for graph management, HashMap usage and tree data structures. This folder can be also found as a submodule in Day 5 (as Tree.h) Day 7 (with it's original README and implementation of graph and hashmap classes) and Day 11 (with a not up-to-date README) folders. `Please take a look at it and tell us what do you think about it as we would love to know professional opinions!`

## Collaborators
- Enric Talens (Group Leader, Implementer, and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer, and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)