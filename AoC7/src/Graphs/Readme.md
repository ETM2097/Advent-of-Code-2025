# Implementation of Part 1 and Part 2 of Day 7 - Advent of Code 2025 with Graphs and HashMaps

This folder shows the implementation we made for Day 7 of Advent of Code 2025, which involves simulating a ray bouncing through a grid of mirrors that make it duplicate and change directions. We utilized graph structures to represent the grid and hash maps for efficient state tracking and memoization.

In the first parrt as we just needed to count how many times the laser has been divided, we have not implemented any memoization, but in the second part we needed to count all the different paths the laser could take, so we implemented a custom HashMap to store already computed states and avoid redundant calculations.

## AoC7_P1_G.cpp

This file contains the implementation for Part 1 of Day 7 using Graphs. We represented the grid as a directed graph where each cell is a node, and edges represent possible movements of the laser based on the mirrors' orientations. We will show a simple visual representation with a 6 x 6 grid:
```
. . S . . .      . . | . . .
. . ^ . . .      . / ^ \ . .
. ^ . ^ . .      / ^ | ^ \ .
. . . . ^ .      | . | . | .
^ . ^ . . .      | . | . | .
. . . ^ . .      | . | ^ | .        
```

Where `S` is the starting point of the laser, `.` are empty cells, and `^` are mirrors that cause the laser to split and change direction. The (|) and (/) and (\) symbols represent the direction of the laser after hitting a mirror, and the edges in the graph are created based on these rules.

In order to achieve this we implemented our own Graph structure in `Graph.h` and `Graph.cpp`. You can read the Readme..md files in the `include` folder for more details about the implementation of the Graph structure.

### AoC7_P1_G Implementation Highlights
- We read the input grid from a file and stored it in a 2D vector of characters.
- We constructed a graph representation of the grid, where each cell is a node and edges are created based on the movement rules of the laser.
- We implemented a BFS algorithm to simulate the laser's movement through the grid, counting the number of times the laser splits when it hits a mirror.

#### How does a BFS work in this case?
- We start from the node containing 'S' and explore all possible paths the laser can take in a level-order manner. We will check all lateral movements first before going deeper into the graph.
- Each time the laser hits a mirror ('^'), we enqueue two new paths (left and right) to the BFS queue.
- We continue this process until we reach the last row of the grid, counting the number of splits along the way.
- Technically it goes node by node level by level checking all the possible paths the laser can take. But as we just want to count the number of splittings, we do not need to store all the paths, just count how many times we split. That's why we use `set` to store the visited positions of the laser in each row.

Here is the code snippet of the BFS implementation :
```cpp
long long bfs(const Graph& graph, int startNodeID) {
    queue<int> bfs; // Queue for BFS
    set<int> visited; // We use a set to avoid counting splitters more than once because of multiple paths
    long long totalSplittings = 0; // This will store the total number of splitters hit

    // We start from the starting node
    bfs.push(graph.startNodeID);

    // Now, we start the BFS
    while (!bfs.empty()) {
        // We get the current node
        int currentNodeID = bfs.front();
        // We pop it from the queue
        bfs.pop();

        // If we have already visited this node, we skip it
        if (visited.count(currentNodeID)) continue;

        // Else, we mark it as visited
        visited.insert(currentNodeID);

        // We count if it is a splitter
        if (graph.nodes[currentNodeID]->cellType == '^') {
            totalSplittings++;
        }

        // We add its neighbors to the queue
        for (int neighborID : graph.nodes[currentNodeID]->neighbors) {
                bfs.push(neighborID);
        }
    }
    return totalSplittings;
}
```

Finally we show the total number of splittings when the laser reaches the last row.

## AoC7_P2_G.cpp
This file contains the implementation for Part 2 of Day 7 using Graphs and our custom HashMap for memoization. In this part, we needed to count all the different paths the laser could take from the start to the last row of the grid. As it takes `+2 trillion` possible paths, we decided to choose the dynamic programming approach with memoization to optimize our solution.
The graph structure is the same as in Part 1, but now we implemented a recursive DFS algorithm with memoization to count the number of unique paths. Because we need to know how many paths lead to the end and we mjust get to the end of each one in order to count it and memoize it.

### How does the DFS with Memoization work in this case?
- We start from the node containing 'S' and explore all possible paths the laser can take using a depth-first approach. This means we go as deep as possible along one path before backtracking.
- Each time the laser hits a mirror ('^'), we recursively explore both new paths (left and right).
- We use our custom HashMap to store already computed states (node positions) and their corresponding path counts to avoid redundant calculations.
- When we reach the last row of the grid, we return 1 to count that as a valid path.
- The total number of unique paths is accumulated as we backtrack through the recursive calls.

Here is the code snippet of the DFS with Memoization implementation:
```cpp
long long dfs(const Graph& graph, int nodeID) {
    // Base case: if we reached the last row, we found a valid path
    if (graph.isLastRow(nodeID)) return 1;
    
    // Check if already computed
    if (memo.contains(nodeID)) return memo.get(nodeID);
    
    char cell = graph.nodes[nodeID]->cellType;
    long long result = 0;

    // Explore neighbors based on cell type
    if (cell == '^') {
        // Splitter: explore both left and right paths
        for (int neighborID : graph.nodes[nodeID]->neighbors) {
            result += dfs(graph, neighborID);
        }
    } 
    else if (cell == '.' || cell == 'S') {
        // Empty cell or Start: continue downwards
        for (int neighborID : graph.nodes[nodeID]->neighbors) {
            result += dfs(graph, neighborID);
        }
    }
    // Other characters: laser stops, result stays 0
    
    memo.set(nodeID, result); // Store in memoization map
    return memo.get(nodeID);
}
```

By implementing the DFS with memoization, we efficiently counted all unique paths the laser could take to reach the last row of the grid, even with the large number of possible paths.