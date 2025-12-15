# Implementation of HashMap and Graph for Advent of Code 2025: Day 11
This document describes the implementation details of the `HashMap` and `Graph`. Both classes are templated to allow for flexibility in key and value types. The implementations build upon concepts learned in previous days, with specific adaptations to meet the requirements of Day 11.

As we tried to make them as reusable as possible, as we believe in code reusability, those classes ended up being quite bigger than expected. So we decided to approach this README as if it was a user comprehesive guide, explaining the main features of each class and the decisions we made during the implementation. Get ready for a long read!

# Index

As the document is quite long, here is an index to help you navigate through the different sections so you can check easily the parts you are more interested in:
- [HashMap Implementation](#hashmap-implementation)
  - [Key Features](#key-features)
  - [operator()](#operator)
  - [Append Method](#append-method)
- [Graph Implementation](#graph-implementation)
  - [Graph Template Parameters](#graph-template-parameters)
  - [Graph Class Members](#graph-class-members)
  - [Constructor](#constructor)
  - [Methods](#methods)
    - [Node Management](#node-management)
    - [Edge Management](#edge-management)
    - [Graph Properties](#graph-properties)
    - [Path Finding and Counting](#path-finding-and-counting)

## HashMap Implementation
The `HashMap` class we implemented is a templated hash map that supports generic key-value pairs. We decided to use different collision resolution using separate chaining with linked lists. But this is the same as in Day 7. We added new functionalities to the HashMap to support our needs in Day 11 taht will be described below.

### Key Features
- **Templated Design**: The `HashMap` class is templated to allow for any data type as key and value (Updated on AoC7 as well).
- **Custom Hash Functions**: We implemented a default hash function and provided the ability to define custom hash functions for complex key types, such as tuples, strings and pairs. we have the following hash functions:
  - `DefaultHash`: A simple hash function for basic types.
  - `DefaultHash<std::string>`: A hash function specifically for strings. We used it as DefaultHash for strings.
  - `PairHash`: A hash function for pairs of values.
  - `TupleHash3`: A hash function for tuples of arbitrary size.
  - `TupleHash3<std::string, bool, bool>`: A specialized hash function for tuples containing a string and two booleans. We used it for our Day 11, part 2 implementation.

  In order to calculate the hash value, we updated the operator() of each function, as we made them functor classes. And the operator() now returns the computed hash value.

- **Collision Resolution**: We used separate chaining with linked lists to handle collisions.
- **Dynamic Resizing**: The hash map now dynamically resizes when the load factor exceeds a threshold.
- **Basic Operations**: The class now supports insertion (`set`), appending to vectors (`append`), retrieval (`get`), deletion (`remove`), and checking for key existence (`contains`).
- **Clear Functionality**: Added a `clear` method to reset the hash map to its initial empty state.

#### operator()
We updated the `operator()` of each hash function to compute and return the hash value based on the key type. This allows the `HashMap` to use these functions to determine the appropriate bucket for each key-value pair. Other approach could have been to use specialization of std::hash, but we preferred to keep our own hash functions for better learning experience. The practices of this assignature helped us to understand better how to modify or define the operators for our own types.

#### Append Method
The `append` method allows adding a value to a vector stored at a specific key. We decided to add this method because it became critical for our graph implementation, but we struggled to implement it correctly, mainly because we wanted to keep the HashMap generic enough to be used in other scenarios. We investigated and found that using `std::is_same` from the `<type_traits>` library allowed us to check if the value type is a vector at compile time. This way, we could ensure that the `append` method is only used when the value type is indeed a vector, preventing potential runtime errors. The correct syntax of the conditional was provided by Copilot. 

The code looks as follows:

```cpp
template<typename K, typename V>
template<typename ElementType>
        typename std::enable_if<std::is_same<T, std::vector<ElementType>>::value, void>::type 
``` 
This ensures that the `append` method is only instantiated when `T` is a vector type. The template parameter `ElementType` represents the type of elements stored in the vector. However, it is important to note that this does not allow appending arbitrary types to the vector. The type of the value being appended must match the `ElementType` of the vector. For example, if `T` is `std::vector<int>`, only `int` values can be appended.

The `append` method code is as follows:

```cpp
    void append(const K& key, const ElementType& value) {
        for (auto& pair : map[hashFunction(key)]) {
            if (pair.first == key) {
                pair.second.push_back(value); // Appends to existing vector
                return;
            }
        }
        // If not found, we insert new key-value pair with a vector containing the value
        map[hashFunction(key)].emplace_back(key, T{value});
        numElements++;
        // This is a resizing check after insertion
        if (static_cast<double>(numElements) / hashSize > loadFactorThreshold) {
            resize();
        }
   }
```

## Graph Implementation
The `Graph` class is a templated graph implementation that uses the `HashMap` class for storing adjacency lists, in order to achieve O(1) average complexity for insertion and lookups.
As we intended to make the code reusable for future challenges, we tried to implement a class prepared for both directed and undirected graphs, as well as weighted and unweighted edges and nodes. We could make it even more generic by adding mixed graphs (both directed and undirected edges), but we decided it was out of the scope of this challenge. Anyway, is on the list for future updates, as there is still a lot of improvement left.

This implementation differs from Day 7's graph implementation in several ways, and we will describe them below:

### Key Features
- **Templated Design**: The `Graph` class is templated to allow for any data type as nodes.
- **Adjacency Lists**: The graph uses adjacency lists stored in `HashMap` instances for both forward and backward edges.
- **Weighted Edges**: Although not used in Day 11, we added support for weighted edges by implementing a separate adjacency list for weighted edges.
- **Node Management**: The graph keeps track of all unique nodes and their in-degrees.
- **Node Data Storage**: We added a `HashMap` to store additional data associated with each node, allowing for more complex graph algorithms that may require node-specific information.
- **Basic Operations**: The class supports `adding nodes` (with or without data), `adding edges` (directed or undirected, weighted or unweighted), `removing nodes and edges`, `seting` (data and weights) , `retrieving neighbors`, `checking` for node existence, size, number of neighbors and edges, and `getting` the in-degree, the leaf nodes, and all nodes.
- **Path Counting**: We implemented a method that counts paths between two nodes and another one that counts paths that must go through two specified intermediate nodes. Both implementations use depth-first search (DFS) with memoization to optimize performance.
- **Path Finding**: We implemented a breadth-first search (BFS) method to find the shortest path between two nodes. (Not used in Day 11, but useful for future challenges).
- **Dijkstra's Algorithm**: We implemented Dijkstra's algorithm to find the shortest path in weighted graphs, although it was not used in Day 11.
- **Topological Sort**: We implemented a topological sort method, which can be useful for directed acyclic graphs (DAGs). Although we did not end up using it for Day 11, we included it for completeness and potential future use cases.

As you could see, we put a lot of effort into making the `Graph` class versatile and capable of handling various graph-related tasks. This design allows us to reuse the class in future challenges or problems in class without needing to reimplement basic graph functionalities. 
This was made to show our understanding of graph theory and data structures, as well as to prepare us for more complex probblems that we may face in the future.

Now we will start explaining all the logic behind the new `Graph` class.

### Graph Template Parameters
The `Graph` class is templated with three parameters:
- `NodeType`: The type of the nodes in the graph (e.g., `std::string`, `int`).
- `WeightType`: The type of the weights for weighted edges (e.g., `int`, `double`). It is defaulted to `int`.
- `NodeDataType`: The type of additional data associated with each node (e.g., `std::string`, `float`). It is defaulted to `int`.
```cpp
template<typename NodeType, typename WeightType = int, typename NodeDataType = int>
```
### Graph Class Members
The `Graph` class contains several private members to manage the graph's structure and properties, we chose to give the users the freedom to choose the types of nodes, weights and node data, so they can adapt the graph to their needs. The main members are:
- `Adjacency Lists`: We added `forwardAdjList` and `backwardAdjList` to store the edges of a graph, `forwardAdjList` represents the nodes the current node is able to visit, and `backwardAdjList` represents the nodes that can visit the current node. We also added `weightedAdjList` for weighted edges in order to represent the weight for each edge. This is needed to support both weighted and unweighted edges, also as directed and undirected edges. The code looks as follows:
```cpp
    HashMap<NodeType, std::vector<NodeType>> forwardAdjList;
    HashMap<NodeType, std::vector<NodeType>> backwardAdjList;
    HashMap<NodeType, std::vector<std::pair<NodeType, WeightType>>> weightedAdjList;
```
- `Neighbor Count`: A `HashMap` to keep track of the number of entries (upper neighbors) for each node. 1 degree is defined as one incoming edge to the node.
- `Node Data`: A `HashMap` to store additional data associated with each node.

### Constructor
For the constructor, we decided to let the user choose what type of graph they wanted to create, so we added three boolean parameters: `isDirected`, `isWeighted`, and `hasNodeData`. These parameters allow the user to specify whether the graph should be directed or undirected, weighted or unweighted, and whether it should store additional data for each node. The constructor initializes these parameters accordingly:
```cpp
    Graph(bool isDirected = true, bool isWeighted = false, bool hasNodeData = false) : isDirected(isDirected), isWeighted(isWeighted), hasNodeData(hasNodeData) {}
```
Each of these parameters has a default value, making the use able to use the constructor without any arguments if they want a directed, unweighted graph without node data.

### Methods
This is the largest section of the README, as we implemented several methods to manage and manipulate the graph. We tried to make them as generic as possible, because reusablility is the key. We are going to discuss the methods following this order:
- Node Management
- Edge Management
- Graph Properties
- Path Finding and Counting

#### **Node Management**
Here we will discuss the methods related to adding, removing, and managing nodes in the graph. As well as setting and getting node data. We decided to use overloading to provide flexibility in adding nodes with or without data. We added security checks to ensure that nodes are not added multiple times, and that data is only set for nodes that exist in the graph.

- `Adding Nodes`:
We implemented two overloaded methods to add nodes to the graph, one for adding a node without data and another for adding a node with associated data.
The code looks as follows:
```cpp
        // Overloaded method to add a node wthout data
        void addNode(const NodeType& node) {
            allNodes.insert(node); // Insert method from <set>
        }

        // Overloaded method to add a node with data
        void addNode(const NodeType& node, const NodeDataType& data) {
            if (!hasNodeData) {
                throw runtime_error("Graph does not support node data.");
            }
            allNodes.insert(node);
            nodeData.set(node, data);
        }
```

- `Setting Node Data`:
We implemented a method to set the data for a specific node. This method checks if the graph supports node data and if the node exists before setting the data.
The code looks as follows:
```cpp
        void setNodeData(const NodeType& node, const NodeDataType& data) {
            if (!hasNodeData) {
                throw runtime_error("Graph does not support node data.");
            }
            if (!containsNode(node)) {
                throw runtime_error("Node does not exist in the graph.");
            }
            nodeData.set(node, data);
        }
```

- `Getting Node Data`:
We implemented a method to retrieve the data associated with a specific node. This method checks if the graph supports node data and if the node exists before retrieving the data.
The code looks as follows:
```cpp
        NodeDataType getNodeData(const NodeType& node) const {
            if (!hasNodeData) {
                throw runtime_error("Graph does not support node data.");
            }
            if (!containsNode(node)) {
                throw runtime_error("Node does not exist in the graph.");
            }
            return nodeData.get(node);
        }
``` 
- `Removing Nodes`:
We implemented a method to remove a node from the graph. This method uses a helper function to handle the removal process, ensuring that all associated edges and data are also removed.
The code looks as follows:
```cpp
        // API
        void removeNode(const NodeType& node) {
            removeNodeFromGraph(node);
        }

        // Helper function to remove a node and its associated edges and data
        void removeNodeFromGraph(const NodeType& node) {
            // Remove all outgoing edges
            if (forwardAdjacents.contains(node)) {
                vector<NodeType> neighbors = forwardAdjacents.get(node);
                for (const NodeType& neighbor : neighbors) {
                    removeEdge(node, neighbor); // Here e use removeEdge to handle outgoing edges
                }
            }

            // Remove all incoming edges
            if (backwardAdjacents.contains(node)) {
                vector<NodeType> neighbors = backwardAdjacents.get(node);
                for (const NodeType& neighbor : neighbors) {
                    removeEdge(neighbor, node); // Now we use removeEdge to handle incoming edges
                }
            }

            // Now we remove the node from all relevant data structures
            forwardAdjacents.remove(node);
            backwardAdjacents.remove(node);
            inDegrees.remove(node);
            if (weightedAdjacents.contains(node)) {
                weightedAdjacents.remove(node); // Now we remove from weightedAdjacents if applicable 
            }
            allNodes.erase(node);
        }
```

#### **Edge Management**
In this section, we will discuss the methods related to adding and removing edges in the graph. As well for setting weights for weighted edges. We implemented methods to handle both directed and undirected edges, as well as weighted and unweighted edges. We added security checks to ensure that edges are only added between existing nodes, and that weights are only set for weighted edges. Also made the edge adding/removing/setting directed or undirected based on the graph's configuration.
For this methods we also used overloading to provide flexibility in adding edges with or without weights, and used helper methods in order to avoid code duplication and mantain clarity and simplicity for the public methods that the user will use.

- `Adding Edges`:
We implemented two overloaded methods to add edges to the graph, one for adding unweighted edges and another for adding weighted edges.
The code looks as follows:
```cpp
        // API method to add an unweighted edge
        void addEdge(const NodeType& from, const NodeType& to) {
            if (isWeighted) {
                throw runtime_error("Graph is weighted, use addNode with weight parameter.");
            }
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            addEdgeToGraph(from, to);
            if (!isDirected) {
                addEdgeToGraph(to, from); // For undirected graphs, we add the reverse edge
            }
        }

        // API method to add a weighted edge
        void addEdge(const NodeType& from, const NodeType& to, const WeightType& weight) {
            if (!isWeighted) {
                throw runtime_error("Graph is unweighted, cannot add weighted edges.");
            }
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            addEdgeToGraph(from, to, weight);
            if (!isDirected) {
                addEdgeToGraph(to, from, weight); // For undirected graphs, we add the reverse edge
            }
        }

        // Helper method to add unweighted edges to the graph
        void addEdgeToGraph(const NodeType& from, const NodeType& to) {
            forwardAdjacents.append(from, to); // We use the append method from our custom HashMap
            backwardAdjacents.append(to, from);
            int currentDegree = inDegrees.contains(to) ? inDegrees.get(to) : 0; // Here we get current in-degree, if it doesn't exist, we assume it is 0
            inDegrees.set(to, currentDegree + 1); // We increment the in-degree of the 'to' node
            allNodes.insert(from); // We use a set to avoid duplicates
            allNodes.insert(to); 
        }

        // Helper method to add weighted edges to the graph
        void addEdgeToGraph(const NodeType& from, const NodeType& to, const WeightType& weight) {
            // We add the edge to the adjacency lists
            addEdgeToGraph(from, to);

            // Then we add the weighted edge to weightedAdjacents
            // If 'from' does not exist yet in weightedAdjacents, we initialize it (empty vector)
            if (!weightedAdjacents.contains(from)) {
                weightedAdjacents.set(from, vector<pair<NodeType, WeightType>>());
            }
            // Finally we add the weighted edge
            auto neighbors = weightedAdjacents.get(from); // We get current neighbors
            neighbors.emplace_back(to, weight); // We add the new neighbor with weight
            weightedAdjacents.set(from, move(neighbors)); // We update the neighbors list using move to avoid unnecessary copies
        }
``` 
- `Setting Edge Weights`:
We implemented a method to set the weight for a specific edge. This method checks if the graph is weighted, the edge exists, and if both nodes exist before setting the weight.
The code looks as follows:
```cpp
        void setWeight(const NodeType& from, const NodeType& to, const WeightType& weight) {
            if (!isWeighted) throw runtime_error("Graph is not weighted, cannot set weights.");
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            // update forward weighted edge
            if (!weightedAdjacents.contains(from)) throw runtime_error("Edge does not exist to set weight.");
            auto edges = weightedAdjacents.get(from);
            bool found = false;
            for (auto& e : edges) {
                if (e.first == to) { 
                    e.second = weight; 
                    found = true; 
                    break; 
                }
            }
            if (!found) throw runtime_error("Edge does not exist to set weight.");
            // we set back the modified edge weights
            weightedAdjacents.set(from, std::move(edges));

            // if undirected, we update reverse weighted edge
            if (!isDirected) {
                auto reverse = weightedAdjacents.get(to);
                bool foundRev = false;
                for (auto& e : reverse) {
                    if (e.first == from) { 
                        e.second = weight; 
                        foundRev = true; 
                        break; 
                    }
                }
                if (!foundRev) reverse.emplace_back(from, weight);
                weightedAdjacents.set(to, move(reverse));
            }
        }
``` 
- `Removing Edges`:
We implemented a method to remove an edge from the graph. This method checks if both nodes exist before removing the edge. It then removes the edge from the adjacency list and, if the graph is undirected, also removes the reverse edge. It updates the in-degree of the destination node accordingly.
The code looks as follows:
```cpp
        // API method to remove an edge
        void removeEdge(const NodeType& from, const NodeType& to) {
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph."); // We check if both nodes exist
            }
            removeFromAdjacencyList(from, to); // We use a helper method to remove the edge
            if (!isDirected) {
                removeFromAdjacencyList(to, from); // For undirected graphs, we remove the reverse edge
            }
        }

        // Helper method to remove an edge from the adjacency list, as the API is the responsible for handling directed/undirected logic, 
        // this method only removes the edge in one direction
        void removeFromAdjacencyList(const NodeType& from, const NodeType& to) {
            // We remove 'to' from the forward adjacency list of 'from'
            if (forwardAdjacents.contains(from)) { // If 'from' exists in the forward adjacency list we proceed
                vector<NodeType> neighbors = forwardAdjacents.get(from);
                neighbors.erase(remove(neighbors.begin(), neighbors.end(), to), neighbors.end());
                forwardAdjacents.set(from, neighbors);
            }
            // We remove 'from' from the backward adjacency list of 'to'
            if (backwardAdjacents.contains(to)) { // If 'to' exists in the backward adjacency list we proceed
                vector<NodeType> neighbors = backwardAdjacents.get(to);
                neighbors.erase(remove(neighbors.begin(), neighbors.end(), from), neighbors.end());
                backwardAdjacents.set(to, neighbors);
            }
            // We update the in-degree count for 'to'
            if (inDegrees.contains(to)) {
                int currentDegree = inDegrees.get(to);
                if (currentDegree > 0) {
                    inDegrees.set(to, currentDegree - 1);
                }
            }
        }
```
#### **Graph Properties**
In this section, we will discuss the methods related to retrieving properties of the graph, such as size, number neighbors, in-degrees, and leaf nodes. We will just discuss briefly each method, as they are quite straightforward.
- `Size of the Graph`: This is done using the `allNodes` set size function.
```cpp
        size_t size() const {
            return allNodes.size();
        }
```
- `Checking Node Existence`: This is done using the `allNodes` set find function.
```cpp
        bool containsNode(const NodeType& node) const {
            return allNodes.find(node) != allNodes.end();
        }
```
- `Getting Neighbors`: This is done using the `forwardAdjacents` or `backwardAdjacents` HashMap get function.
```cpp
        // Just putting the headers, as the code is quite straightforward
        vector<NodeType> getForwardNeighbors(const NodeType& node) const {}
        vector<NodeType> getBackwardNeighbors(const NodeType& node) const {}
```
- `Getting Number of Neighbors`: This is done using the size of the vector returned by the `forwardAdjacents` HashMap get function.
```cpp
        size_t getNumNeighbors(const NodeType& node) const {
            if (!forwardAdjacents.contains(node)) {
                return 0; // If the node has no outgoing edges, it has 0 neighbors
            }
            return forwardAdjacents.get(node).size();
        }
```
- `Getting In-Degree`: This is done using the `inDegrees` HashMap get function.
```cpp
        int getInDegree(const NodeType& node) const {
            if (!inDegrees.contains(node)) {
                return 0; // If the node has no incoming edges, its in-degree is 0
            }
            return inDegrees.get(node);
        }
```
- `Getting Leaf Nodes`: We implemented a method that iterates through `allNodes` and collects nodes with no outgoing edges (i.e., nodes not present in `forwardAdjacents` or with an empty adjacency list).
```cpp
        vector<NodeType> getLeafNodes() const {
            vector<NodeType> leaves;
            for (const NodeType& node : allNodes) {
                if (getForwardNeighbors(node).empty()) {
                    leaves.push_back(node);
                }
            }
            return leaves;
        }
```
- `Getting Root Nodes`: We implemented a method that iterates through `allNodes` and collects nodes with no incoming edges (i.e., nodes with an in-degree of 0).
```cpp
        vector<NodeType> getRootNodes() const {
            vector<NodeType> roots;
            for (const NodeType& node : allNodes) {
                if (getInDegree(node) == 0) {
                    roots.push_back(node);
                }
            }
            return roots;
        }
```
- `Checking if edges exist between two nodes`: This is done using the `adjacency lists` to check if the edge exists in the specified direction(s).
```cpp
        // Just putting the headers, as the code is quite straightforward
        bool hasEdgeForward(const NodeType& from, const NodeType& to) const {}
        bool hasEdgeBackward(const NodeType& to, const NodeType& from) const {}
        bool hasEdgeBoth(const NodeType& from, const NodeType& to) const {}
```

#### **Path Finding and Counting**
In this section, we will discuss the methods related to finding and counting paths in the graph. We implemented 4 methods: one for finding the shortest path using BFS (unweighted graphs), one for finding the shortest path using Dijkstra's algorithm (weighted graphs), one for counting all paths between two nodes using DFS with memoization, and one for counting all paths that must go through two specified intermediate nodes using DFS with memoization.
- `Finding Shortest Path (BFS)`:
We implemented a method that uses breadth-first search (BFS) to find the shortest path between two nodes in an unweighted graph. The method returns a vector of nodes representing the shortest path.