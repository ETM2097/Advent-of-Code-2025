# AoC11 — Source Notes

Here are the implementations for the Day 11 challenge of Advent of Code 2025, focusing on graph path counting with specific constraints.
We will not explain a lot here as most of the implementation details are already explained in the `include/README.md` file, as we developed the whole reusable code for graph management and path counting there. 

## Code explanation
### AoC11_P1
Here we implemented an easy function in order to take the input correctly and build the graph using our reusable `Graph` class. 
Then we just call the function `countAllPaths` from the `Graph` class, which is a general implementation for counting all paths between two nodes in a directed acyclic graph (DAG) using depth-first search (DFS) with memoization. You can find how `countAllPaths` is implemented in the `Reference Part 1` source in `Soruce Notes`.

### AoC11_P2
Similar to Part 1, we read the input and build the graph using our reusable `Graph` class. 
Then we call the function `countPathsWithTwoIntermediateNodes`, which is a general implementation for counting all paths between two nodes in a directed acyclic graph (DAG) that must pass through two specified intermediate nodes. This function uses depth-first search (DFS) with memoization to efficiently count the paths while ensuring the constraints are met. You can find how `countPathsWithTwoIntermediateNodes` is implemented in the `Reference Part 2` source in `Soruce Notes

## AoC11 — Source Notes

- **Reference Part 1:** [Counting All Paths Between Two Nodes](../include/README.md#counting-all-paths-between-two-nodes)
- **Reference Part 2:** [Counting Paths with Two Intermediate Nodes](../include/README.md#counting-paths-with-intermediate-nodes)

Additional implementation notes and examples can be found in the `include/README.md` linked above.
