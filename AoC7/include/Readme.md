# Implementations of Graph and HashMap

## Graph.h
This contains the implementation of the Graph class that we used to represent the grid given by the input of the challenge. The graph is constructed from a grid representation, where each cell can contain (S, . or ^). The class implements methods for BFS and DFS algorithms to simulate the ray's movement through the grid.

This implementation is not a full graph implementation, it is specialized for this challenge of the Andvent of Code. The CPP code is in `src/Graphs/Graph.cpp`, but we will discuss here the implementation of each method.

### Graph's Implementation Highlights
- First we created a structure called `Node` that is the representation of each cell in the grid. It has the following attributes:
    - `row` and `col`: to store the position of the node in the grid.
    - `type`: to store the type of the node (S, ., ^).
    - `neighbors`: a vector of indices to neighboring nodes.

    The code for the Node structure is as follows:
```cpp
struct Node {
    int row;
    int col;
    char type;
    vector<int> neighbors;
};
```
- Then, we implemented the class `Graph` that has the following attributes:
    - `nodes`: A 1D vector of Node pointers to store all nodes in the graph.
    - `rows` and `cols`: to store the dimensions of the grid.
    - `startNode`: The node wich contains the 'S' character.

    - The constructor of the Graph class takes the grid of the input and initializes the nodes and applies the connections between them, based on the rules of the ray bouncing.

    Now we will explain how the constructor is implemented:
    ```cpp
    Graph::Graph(const vector<string>& grid) {
        rows = grid.size();
        cols = grid[0].size();
        nodes.resize(rows * cols);
        startNodeID = -1;
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                int iD = getNodeID(row, col); // getNodeID converts (row, col) to a unique ID, explained later.
                nodes[iD] = Node();
                nodes[iD].rows = row;
                nodes[iD].cols = col;
                nodes[iD].cellType = grid[row][col];
                if (grid[row][col] == 'S') {
                    startNodeID = iD;
                }
            }
        }
    }
    ``` 
    As you can see above, we initialize the nodes vector with new Node objects and set their attributes based on the grid input. We also identify and store the start node's ID.

    After that, we construct the edges between the nodes based on the rules of the challenge (goes down on '.', bounces left/right on '^'). The edges are created in the following way:
    ```cpp
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                int currentID = getNodeID(row, col);
                char cell = nodes[currentID].cellType;
                if (cell == '^') {
                    if (col - 1 >= 0) {
                        nodes[currentID].neighbors.push_back(getNodeID(row + 1, col - 1));
                    }
                    if (col + 1 < cols) {
                        nodes[currentID].neighbors.push_back(getNodeID(row + 1, col + 1));
                    }
                } 
                else if (cell == '.' || cell == 'S') {
                    if (row + 1 < rows) {
                        nodes[currentID].neighbors.push_back(getNodeID(row + 1, col));
                    }
                }
            }   
        }
    ```
    Here we iterate through each node and based of the `cellType`, we add the appropriate neighbors to the `neighbors` vector of each node.

    - Then, we implemented a few methods to help us store and retrieve nodes:
        - `getNodeID(int row, int col)`: Converts a (row, col) pair to a unique integer ID for easy indexing in the nodes vector.
        - `getRowCol(int nodeID)`: Converts a node ID back to its (row, col) representation.
        - `getNumNodes()`: Returns the total number of nodes in the graph.
        - `isLastRow(int nodeID)`: Checks if a given node is in the last row of the grid.

    ```cpp
    int Graph::getNodeID(int row, int col) const {
        return row * cols + col;
    }

    pair<int, int> Graph::getRowCol(int nodeID) const {
        int row = nodeID / cols;
        int col = nodeID % cols;
        return {row, col};
    }

    int Graph::getNumNodes() const {
        return nodes.size();
    }

    bool Graph::isLastRow(int nodeID) const {
        return nodes[nodeID].rows == rows - 1;
    }
    ```
    These methods gave us more than enough functionality to work with the graph representation of the grid for solving the challenge.

## HashMap.h
This contains the implementation of our custom HashMap, we chose to implement our own HashMap in order to fullfill the challenge of implementing data structures for this year's Advent of Code.
We developed it as a templated class so it can store any type of value, with any type of key. This will permit us to reuse this HashMap and to implement more functionalities in the future challenges if needed.

The structure of the HashMap is quite simple, and we chose to implement it using separate chaining for collision resolution. Each bucket in the HashMap contains a linked list of key-value pairs. As we made the size static for simplicity, we defined a static constant for the number of buckets, but this can be changed to a dynamic resizing implementation in the future if needed. As for now, we used 25k buckets, which is more than enough for the challenge.

### HashMap's Implementation Highlights

We defined two private attributes:
- `map`: A vector of `list` to store the key-value pairs. We used `list` from the C++ Standard Library for efficient insertion and deletion operations as we previously developed our own linked list implementation in previous challenges [you can check the implementation here even if it is not presented for qualification](../data/Readme.md).
- `hashSize`: A static constant defining the number of buckets in the HashMap.

Then we defined a private method for generating hash values:
```cpp
    int hashFunction(K key) const {
        return key % hashSize;
    }
```
Quite simple hashing method, but it works good enough for our use case. If we have time, we can improve it in the future.

The public methods of the HashMap class are as follows:

- `void set(K key, V value)`: Inserts or updates a key-value pair in the HashMap. Uses a iterator to check for existing keys in the corresponding bucket. If none is found, it appends the new key-value pair to the list.

- `V get(K key)`: Retrieves the value associated with a given key. Throws an exception if the key is not found.

- `bool contains(K key)`: Checks if a given key exists in the HashMap.

These are the implementations of these methods:
```cpp
    void set(K key, V value) {
        int hashIndex = hashFunction(key);
        for (auto& pair : map[hashIndex]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        map[hashIndex].emplace_back(key, value);
    }

    V get(K key) {
        int hashIndex = hashFunction(key);
        for (auto& pair : map[hashIndex]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw runtime_error("Key not found");
    }

    bool contains(K key) {
        int hashIndex = hashFunction(key);
        for (auto& pair : map[hashIndex]) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }
```

Those are all the methods that we needed for this challenge, but we can always expand the functionality of the HashMap in the future if needed. And we hope to find another challenge where we can expand this code further.

**23/12:** I come the future and I can confirm that we did expand this HashMap implementation in Day 11 of the challenge, where we added more functionalities and improved the hashing method. Please check INCLUDE's [README](../../INCLUDE/README.md#hashmap-implementation) for more details about the improvements made to this HashMap implementation.