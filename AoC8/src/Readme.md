# Implementation of Part 1 and Part 2 of Day 8 - Advent of Code 2025 with Graphs

This folder shows the implementation we made for day 8 of Advent of Code 2025, which consists of connecting electrical junction boxes in a 3D space. Given N boxes, we connect the 1000 closest pairs using Euclidean distance, forming electrical circuits.  

We use graphs structures to store the boxes and calculate the distances between them.


In the first part, we need to obtain the product of the three largest circuit sizes.
In the second part, we need to connect all boxes into a single circuit and multiply the X coordinates of the last two connected boxes.

## AoC8_P1.cpp

This file contains the implementation of part 1 of day 8 using graphs. First, all the electrical junction boxes in the file are loaded, storing their 3D coordinates (X, Y, Z). Then, the Euclidean distance between each pair of boxes is calculated and these distances are stored as edges. After sorting all the edges by distance in ascending order, the program selects the 1000 closest pairs. These 1000 edges are used to construct the circuits using the Union Find algorithm. Finally, the program counts the size of each electrical circuit by identifying all the root nodes of the Union-Find structure, sorts these sizes in descending order, and multiplies the sizes of the three largest circuits to obtain the final result.

To achieve this, we implemented our own Graph structure in `Graph.h` and `Graph.cpp`. You can read the Readme.md files in the `include` folder for more details on the implementation of the Graph structure.

### UnionFind.cpp
We also implement the Union-Find algorithm, as this algorithm helps us find the connection between two different nodes and, with this connection, build the electrical circuits.

We base our implementation on the following YouTube video: https://youtu.be/8uOIs4aXBiA?si=x4nSsaF8_1KiwwLO


Here is the code snippet of the UnionFind implementation :
```cpp
// constructor, initialize parent and size vectors with n elements
UnionFind::UnionFind(int n) {
    parent.resize(n);
    size.resize(n, 1);
    // each node is its own parent at the beginning
    for(int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

// find the root of n 
int UnionFind::find(int n) {
    if(parent[n] != n) {
        parent[n] = find(parent[n]);
    }
    return parent[n];
}

// unite the sets that contain n1 and n2
void UnionFind::unite(int n1, int n2) {
    n1 = find(n1);
    n2 = find(n2);
    if(n1 == n2) return;
    if(size[n1] < size[n2]) {
        swap(n1, n2);
    }
    parent[n2] = n1;
    size[n1] += size[n2];
}

// get the size of the set that contains n
int UnionFind::getSize(int n) {
    return size[find(n)];
}
```

### AoC8_P1 Implementation Highlights
- We use graphs to store the boxes.
- We implemented the FindUnion algorithm to connect the circuits efficiently.
- Sorted edge selection guarantees the 1000 selected connections are truly the closest pairs.

## AoC8_P2.cpp
This file contains the implementation of part 2 of day 8 using graphs. First, its loads all the boxes and calculates the distances between them, then uses the Union Find algorithm to find which boxes are connected. Starting with the shortest distance, it iterates through all the edges and connects boxes belonging to different circuits using the `unite()` operation. The program continues this process until all boxes form a single circuit, storing the indices of the last two boxes that were connected. Finally, it multiplies the X coordinates of these last two connected boxes to obtain the answer.

Here is the code snippet for one of the most important parts of the implementation:
```cpp
// we connect edges until all nodes are in one circuit
for(const Edge& e : graph.edges) {
    int root1 = uf.find(e.node1);
    int root2 = uf.find(e.node2);
    
    // if they're not connected yet
    if(root1 != root2) {
        uf.unite(e.node1, e.node2);
        lastNode1 = e.node1;
        lastNode2 = e.node2;
    }
    
    // check if all are connected 
    if(uf.getSize(0) == graph.getNumNodes()) {
        break;
    }
}
```
### AoC8_P2 Implementation Highlights
- We implemented the Union-Find algorithm to track connected components and merge circuits.
- We track the last connection made, which is essential for identifying the last two boxes that complete the single circuit.