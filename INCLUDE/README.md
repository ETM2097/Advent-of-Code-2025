# Implementation of HashMap and Graph for Advent of Code 2025:
This document describes the implementation details of the `HashMap` and `Graph`. Both classes are templated to allow for flexibility in key and value types. The implementations build upon concepts learned in previous days, with specific adaptations to meet the requirements of Day 11, as it was the last day we worked on.

We tried to make our code as reusable as possible. While this made the classes a bit larger than expected, we believe it was worth it. Think of this README as a complete user guide: we’ll cover the main features and explain why we built things the way we did. Grab a coffee—this is going to be a long read!

**Actualization:** We indexed here the Tree implementation as well (summarized), as it is part of the reusable code that we developed during the challenge. It is after the Graph implementation.

# Index

As the document is quite long, here is an index to help you navigate through the different sections so you can check easily the parts you are more interested in:
- [HashMap Implementation](#hashmap-implementation)
  - [Key Features](#key-features)
  - [operator()](#operator)
  - [Append Method](#append-method)
- [Graph Implementation](#graph-implementation)
  - [Key Features](#graph-features)
  - [Graph Template Parameters](#graph-template-parameters)
  - [Graph Class Members](#graph-class-members)
  - [Constructor](#constructor)
  - [Methods](#methods)
    - [Node Management](#node-management)
    - [Edge Management](#edge-management)
    - [Graph Properties](#graph-properties)
    - [Path Finding and Counting](#path-finding-and-counting)
- [Tree Implementation](#tree-implementation)
    - [Key Features](#tree-features)
    - [Tree Template Parameters](#tree-template-parameters)
    - [Tree Class Members](#tree-class-members)
    - [Methods](#tree-methods)
- [Final Notes](#final-notes)

## HashMap Implementation
The `HashMap` class we implemented is a templated hash map that supports generic key-value pairs. We decided to use different collision resolution using separate chaining with linked lists. But this is the same as in Day 7 ([you can find it's original readme here for comparison](../AoC7/include/Readme.md#hashmaph)). We added new functionalities to the HashMap to support our needs in Day 11 taht will be described below.

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
- **Basic Operations**: The class now supports insertion (`set`), appending to vectors (`append`), retrieval (`get`), deletion (`remove`), and checking for key existence (`contains`), in the last update we added `getRef()` that permitted us to access values by const reference, improving performance by avoiding unnecessary copying.
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

<a id="graph-features"></a>

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
- **Error Handling**: We added error handling to ensure that operations are performed on existing nodes and edges, throwing exceptions when necessary.
- **Performance Optimization**: We used `getRef()` method from `HashMap` to access adjacency lists by const reference, reducing unnecessary copying and improving performance. This was introduced in the last revision (17/12/2025).
-

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
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(node);
                for (const NodeType& neighbor : neighbors) {
                    removeEdge(node, neighbor); // Here e use removeEdge to handle outgoing edges
                }
            }

            // Remove all incoming edges
            if (backwardAdjacents.contains(node)) {
                const vector<NodeType>& neighbors = backwardAdjacents.getRef(node);
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
This method is not used in Day 11, but we decided to implement it in order to give the user more tools without needing to implement thei own path finding algorithm, just if they need to tune it, but at least, they have a template to start from.
Now, we will explain in detail how does this method works, as is an important algorithm in the theory that this assignature is based on.
The code looks as follows:
```cpp
        // API method to find the shortest path using BFS
        vector<NodeType> bfsShortestPath(const NodeType& start, const NodeType& end) const {
            return bfsShortestPathHelper(start, end);
        }

        // Helper method that implements BFS to find the shortest path
        vector<NodeType> bfsShortestPathHelper(const NodeType& start, const NodeType& end) const {
            if (start == end) return {start}; // Trivial case, the starting node is the ending one.

            HashMap<NodeType, bool> visited; // We use a HashMap to keep track of visited nodes.
            HashMap<NodeType, NodeType> parent; // We use a HasMap to store the parent of each node, for reconstructing the path later.
            queue<NodeType> q; // BFS queue, to process nodes level by level.

            q.push(start); // We push start node to the queue, in order to process it.
            visited.set(start, true); // We mark it as visited, in order to not visit it again.

            bool found = false; // Flag to indicate if we found the end node, for early exit.
            while (!q.empty() && !found) { // While there are nodes to process and we haven't found the end:
                NodeType current = q.front(); q.pop(); // We get the front node and remove it from the queue.
                if (!forwardAdjacents.contains(current)) continue; // If there are no neighbors we must not check this node.
                for (const NodeType& neighbor : forwardAdjacents.getRef(current)) { // We iterate through its neighbors
                    if (visited.contains(neighbor)) continue; // If already visited, skip
                    visited.set(neighbor, true); // We mark it as visited.
                    parent.set(neighbor, current); // We set its parent for path reconstruction.
                    if (neighbor == end) { // If we found the end node, we stop.
                        found = true;
                        break;
                    }
                    q.push(neighbor); // Otherwise, we add the neighbor to the queue.
                }
            }

            // If we didn't find the end node, return empty path
            if (!found) return {};

            // Else we reconstruct the path from end to start using the parent map
            vector<NodeType> path; // Now we use a vector to sotre the inversed path
            for (NodeType at = end; /* NO CONDITION */ ; at = parent.get(at)) { // We go backwards from end to start
                path.push_back(at);
                if (at == start) break;
            }
            reverse(path.begin(), path.end()); // We reverse the path to get it from start to end
            return path; // Fianlly we return the path
        }

```
- `Finding Shortest Path (Dijkstra's Algorithm)`:
We implemented a method that uses Dijkstra's algorithm to find the shortest path between two nodes in a weighted graph. The method returns a vector of nodes representing the shortest path to each node from the start node.
This method is not used in Day 11, but as with the BFS, we decided to implement it in order to make this class more complete. Now we will explain in detail how does this method works, as Dijkstra's algorithm is a fundamental algorithm in graph theory.
```cpp
        // API method to find the shortest distance using Dijkstra's algorithm, finding all shortest distance to each node from start
        vector<NodeType> dijkstraShortestPath(const NodeType& start) const {
            if (!isWeighted) {
                throw runtime_error("Graph is not weighted, cannot perform Dijkstra's algorithm.");
            }
            if (!hasNode(start)) throw runtime_error("Start node must exist in the graph.");
            return dijkstraShortestPathHelper(start);
        }

        // Helper method that implements Dijkstra's algorithm to find the shortest deistance
        vector<NodeType> dijkstraShortestPathHelper(const NodeType& start) const {
            // We use a priority queue to store (distance, node) of the nodes to explore
            // This is a priority queue that sorts by smallest distance first.
            // priority_queue< ELEMENT, CONTAINER, COMPARISON >
            priority_queue<pair<WeightType, NodeType>, vector<pair<WeightType, NodeType>>, greater<pair<WeightType, NodeType>>> pq;
            HashMap<NodeType, WeightType> distances; // To store shortest distances (or whatever WeightType is)
            HashMap<NodeType, bool> visited; // To track visited nodes, we track each node we have processed
            vector<pair<NodeType, WeightType>> result; // To store final distances for each node
            // Here we initialize the algorithm by pushing the start node with distance 0
            pq.push({0, start}); // We start with distance 0 at start node
            distances.set(start, 0); // Distance to start is 0 as we begin there
            while (!pq.empty()) { // While there are nodes to process:
                auto [currentDist, currentNode] = pq.top(); // We get the node with the smallest distance
                pq.pop(); // We remove it from the queue
                if (visited.contains(currentNode)) { // We check if we have already processed this node
                    continue; // Already processed
                }
                visited.set(currentNode, true); // We set it as visited
                result.emplace_back(currentNode, currentDist); // Now we store the current node and its distance in the result
                // Here if we want, we could add an early exit if we reach the end node
                // Then we explore neighbors
                if (weightedAdjacents.contains(currentNode)) { // If there are neighbors
                    for (const auto& [neighbor, weight] : weightedAdjacents.getRef(currentNode)) { // We iterate through them
                        WeightType newDist = currentDist + weight; // Here we calculate the new distance with that neighbor weight
                        if (!distances.contains(neighbor) || newDist < distances.get(neighbor)) { // If the path is shorter or not calculated yet
                            distances.set(neighbor, newDist); // We update the shortest distance
                            pq.push({newDist, neighbor}); // And we add it to the priority queue for further exploration
                        }
                    }
                }
            }
            return result;
        }
```

Finally, we reached the main algorithms that we used in Day 11. Those ones will be explained step by step down below, they will be named in the README in /src and if possible, we'll link you back here from there.

#### Counting All Paths Between Two Nodes

We implemented a method that counted all possible paths that existed between two nodes using Depth-First-Search (DFS) and added memoization to it in order to improve the performace of the alogrithm. This methos is the main algorithm that makes the resolution of Day 11 first part possible. Now we'll proceed to explain in depth this method.
```cpp
        // As the methods avobe, we added an API method and a helper method to do the actual processing

        // API method to count all paths between two nodes
         long long countPaths(const NodeType& start, const NodeType& end) const {
            HashMap<NodeType, long long> memo; // Here we create the memoization HashMap
            return countPathsHelper(start, end, memo); // And now we call the helper method
        }

        // Helper method that implements DFS with memoization to count all paths
        // Really is not a complicated concept, we just need to explore all paths recursively and store the results in a HashMap to avoid recomputation
        long long countPathsHelper(const NodeType& current, const NodeType& target, 
                                   HashMap<NodeType, long long>& memo) const {

            // As we opted by using dynamic programing, the first thing we , make in this case, is to check the memoization HashMap
            // This way we avoid recomputing the number of paths from current to target if we have already computed it before, even the base case.
            if (memo.contains(current)) {
                return memo.get(current); // If we have already computed the number of paths from current to target, we return that value
            }
            
            // As in any recursive function, we declare the base cases, in this case, 
            // the base case is that we reached the target node (current == target).
                if (current == target) {
                    memo.set(current, 1); // We memoize the result for the target node, so we do not compute it again.
                return 1; // We retrun 1 in order to add up the number of possible paths to the end node.
            }
            
            // If we have not computed the path previously, and it is not the base case, we proceed to explore the neighbors of the current node.
            long long totalPaths = 0; // We set the total paths counter to 0, as the paths to the target from current are not computed yet.
            if (forwardAdjacents.contains(current)) { // If the current node has neighbors, we proceed to explore them
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(current); // We get the neighbors of the current node in order to iterate through them
                for (const NodeType& neighbor : neighbors) { // Now for each neighbor, we recursively call the method to count paths from that neighbor to the target
                    totalPaths += countPathsHelper(neighbor, target, memo); // We add the number of paths from neighbor to target to the total paths
                }
            }
            
            // Finally we memoize the result for the current node before returning it
            memo.set(current, totalPaths);
            return memo.get(current); // And we return the memoized result
        }
```
This method does get the total amount of paths that reach `end` form `start`, by exploring all possible paths recursively using DFS, and storing the results in a `HashMap` to avoid recomputation of paths from nodes that have already been processed. This optimization significantly improves the performance of the algorithm, especially in graphs with many overlapping paths.

#### Counting All Paths That Must Go Through Two Intermediate Nodes

We implemented a method that counts all possible paths from a start node to an end node that must pass through two specific intermediate nodes. This method extends the basic path counting algorithm by tracking whether each required node has been visited along the path. This is the algorithm used for Day 11, part 2. Now we'll proceed to explain in depth this method.

```cpp
        // As with the previous methods, we added an API method and a helper method to do the actual processing

        // API method to count paths that must pass through both node1 AND node2
        long long countPathsThrough2(const NodeType& start, const NodeType& end, 
                                     const NodeType& node1, const NodeType& node2) const {
            // State: (current, visited_node1, visited_node2)
            HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>> memo;
            return countPathsThrough2Helper(start, end, node1, node2, false, false, memo);
        }

        // Helper method that implements DFS with memoization to count paths through specific nodes
        // This is a more complex version of countPathsHelper that tracks visited state for two required nodes
        long long countPathsThrough2Helper(const NodeType& current, const NodeType& target,
                                           const NodeType& node1, const NodeType& node2,
                                           bool visited1, bool visited2,
                                           HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>>& memo) const {
            
            // Create state tuple for memoization
            // The state includes the current node AND the visited flags for both required nodes
            tuple<NodeType, bool, bool> state = make_tuple(current, visited1, visited2);
            
            // As with countPathsHelper, we use dynamic programming: check memo first
            // This is crucial: different states of the same node produce different path counts
            // We must check before the base case to avoid recomputing states we've already seen
            if (memo.contains(state)) {
                return memo.get(state); // Return previously computed result for this exact state
            }
            
            // Base case: reached target, only count if BOTH required nodes were visited
            if (current == target) {
                long long result = (visited1 && visited2) ? 1 : 0; // We only count valid paths that visited both nodes
                memo.set(state, result); // We memoize the base case result, so we don't recompute it
                return result;
            }
            
            // If we have not computed the path previously, and it is not the base case, we proceed to explore the neighbors
            // Update visited flags if we're currently at one of the required nodes
            bool newVisited1 = visited1 || (current == node1); // Mark node1 as visited if we're at it
            bool newVisited2 = visited2 || (current == node2); // Mark node2 as visited if we're at it
            
            // Recursively count paths through all outgoing edges
            long long totalPaths = 0;
            if (forwardAdjacents.contains(current)) { // If the current node has outgoing edges
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(current); // We get its neighbors
                for (const NodeType& neighbor : neighbors) { // And we iterate through them
                    // Recursively count paths, propagating the updated visited flags
                    totalPaths += countPathsThrough2Helper(neighbor, target, node1, node2, 
                                                          newVisited1, newVisited2, memo);
                }
            }
            
            // Finally we memoize the result for this specific state before returning it
            memo.set(state, totalPaths);
            return totalPaths; // And we return the result directly
        }
```

This method counts all paths from `start` to `end` that pass through both `node1` AND `node2` (in any order). The key difference from `countPathsHelper` is that the memoization state includes not just the current node, but also two boolean flags tracking whether each required node has been visited. This is necessary because the same node can have different path counts depending on which required nodes have been visited so far.

For example, paths reaching node X after visiting node1 but not node2 will have a different count than paths reaching X after visiting both. The tuple `(current, visited1, visited2)` serves as the memoization key, and we use a custom `TupleHash3` hash function to efficiently hash these tuples in our `HashMap`.

The algorithm only counts paths that reach the target with both `visited1` and `visited2` set to true, ensuring that every counted path passes through both required intermediate nodes. This approach allows us to efficiently compute constrained path counts in directed acyclic graphs (DAGs). 

# Implementation of Tree for Advent of Code 2025: Day 5
This document describes the implementation details of the `HashMap` and `Graph`. Both classes are templated to allow for flexibility in key and value types. The implementations build upon concepts learned in previous days, with specific adaptations to meet the requirements of Day 11, as it was the last day we worked on.

We tried to make our code as reusable as possible. While this made the classes a bit larger than expected, we believe it was worth it. Think of this README as a complete user guide: we’ll cover the main features and explain why we built things the way we did. Grab a coffee—this is going to be a long read!

# Index

As the document is quite long, here is an index to help you navigate through the different sections so you can check easily the parts you are more interested in:
- [Tree Implementation](#tree-implementation)
    - [Key Features](#tree-features)
      - [Tree Template Parameters](#tree-template-parameters)
    - [Tree Class Members](#tree-class-members)
    - [Methods](#tree-methods)

## Tree Implementation
The `Tree` class is a template tree in which we wanted the code to be reusable for future challenges, so we tried to implement a class that was ready for both binary trees of simple elements and intervals. 

It could have been made even more generic by adding other types of trees, but we decided that this was beyond the scope of this challenge. In any case, it is on the list of future updates, as there is still much room for improvement.

<a id="tree-features"></a>

### Key Features

- **Templated Design**: The `Tree` class is template-based to allow any type of data as nodes, whether they are simple elements or ranges.
- **Node Structure**: Each node can store a single value or a range, using `std::variant` to handle both data types.
- **Basic Operations**: The class supports `insertion`, `deletion`, `printing` (inorder, postorder, preorder), `searching`, among others.  
- **Rebalancing and Construction**: We add rebalancing based on the inorder and balanced construction from value or interval vectors (sort, remove duplicates, and rebuild).
- **Error Handling**: We implement error handling to prevent the insertion of duplicate elements.

Now, we will begin to explain all the logic behind the new `Tree` class, but first we will explain two important details: the node class and the interval structure.

### Interval
This structure represents a closed interval defined for two values `start` and `end` of the same type `T`. We also implemented two operators that compare two intervals, one comparing only the start (`operator<`) and the other comparing the start and end of both intervals (`operator==`).

```cpp
template <typename T>
struct Interval {
    T start;
    T end;
    
    Interval() : start(T()), end(T()) {}
    Interval(T s, T e) : start(s), end(e) {}
    
    bool operator<(const Interval& other) const {
        return start < other.start;
    }
    
    bool operator==(const Interval& other) const {
        return start == other.start && end == other.end;
    }
};
```


### Node
This class represents a tree node capable of storing a single value or an interval thanks to `std::variant`. 

In detail: 
- **data**: that stores the value or interval.
- **left / right**: pointers to the left and right children.
- **maxRange**: the maximum value of end in the subtree.
```cpp
    std::variant<T, Interval<T>> data;
    Node* left;
    Node* right;
    T maxRange;  
```
#### Constructors:
We also added three constructors because we needed it to be able to construct from a simple element, an interval, and another that supports intervals.
```cpp
//Constructor for simple elements
Node(T elem, Node* left = nullptr, Node* right = nullptr) : 
    data(elem), left(left), right(right), maxRange(elem) {}

//Constructor for intervals
Node(T start, T end, Node* left = nullptr, Node* right = nullptr) :
    data(Interval<T>(start, end)), left(left), right(right), maxRange(end) {}

//Constructor with Interval
Node(const Interval<T>& interval, Node* left = nullptr, Node* right = nullptr) :
    data(interval), left(left), right(right), maxRange(interval.end) {}
    
```

#### Helpers: 
- **isInterval**: indicates whether data contains an interval.
- **getStart**: return start whether it is a simple value or an interval.
- **getEnd**: return end whether it is a simple value or an interval.
- **getRange**: returns the entire interval; if the node has a simple value, it returns [val, val].

```cpp
//Helper methods to check and access data
bool isInterval() const {
    return std::holds_alternative<Interval<T>>(data);
}

//Get start value (for both T and Interval)
T getStart() const {
    if (isInterval()) {
        return std::get<Interval<T>>(data).start;
    } else {
        return std::get<T>(data);
    }
}

//Get end value (for both T and Interval)
T getEnd() const {
    if (isInterval()) {
        return std::get<Interval<T>>(data).end;
    } else {
        return std::get<T>(data);
    }
}

//Get the interval (if it's a single value, return [val, val])
Interval<T> getRange() const {
    if (isInterval()) {
        return std::get<Interval<T>>(data);
    } else {
        T val = std::get<T>(data);
        return Interval<T>(val, val);
    }
}
```

The `node class` and the `Interval struct` are necessary for the `Tree class` to function. This is how we will now begin to describe the `Tree class`.

### Tree Template Parameters
The `Tree` class is templated with a single parameter:
- `T`: The type of data stored in the tree nodes or interval boundaries (e.g., `int`, `long long`, `double`).

### Tree Class Members
The `Tree` class contains two **private members**:
- `nelem`: This is the current number of stored nodes.
- `root`: This is a pointer to the root node of the tree.

<a id="tree-methods"></a>

### Methods
In this section, we will describe the methods we implement to manage and manipulate the tree.

### Constructor
The `Tree` class has a default constructor that initializes an empty tree:
```cpp
Tree() {
    this->nelem = 0;
    this->root = nullptr;
}
```

#### Insertion
We implemented two insertion methods, one for simple elements and another for ranges, and a method for updating the maximum range.

```cpp
//Unified insert for single elements
Node<T>* insert(Node<T>* n, T e){
    if (n == nullptr) {
        return new Node<T>(e);
    } else if(n->getStart() == e){
        throw runtime_error("Duplicate element");
    } else if(n->getStart() < e){
        n->right = insert(n->right, e);
    } else {
        n->left = insert(n->left, e);
    }
    updateMaxRange(n);
    return n;
}

//Unified insert for intervals (overload)
Node<T>* insert(Node<T>* n, const Interval<T>& interval) {
    // Base case: If the tree is empty, return a new node
    if (n == nullptr) {
        return new Node<T>(interval);
    }
    // Otherwise, recur down the tree
    if (interval.start < n->getStart()) {
        n->left = insert(n->left, interval);
    } else {
        n->right = insert(n->right, interval);
    }
    // Here we update the max value of the ancestor node
    // Here we had a critical bug before, we corrected it by ensuring we consider both left and right children
    updateMaxRange(n);
    // Finally return the node pointer
    return n;
}

    // Here we update the max value of the ancestor node
// Here we had a critical bug before, we corrected it by ensuring we consider both left and right children
void updateMaxRange(Node<T>* n) const {
    if (n == nullptr) return;
    n->maxRange = n->getEnd();
    if (n->left) n->maxRange = std::max(n->maxRange, n->left->maxRange);
    if (n->right) n->maxRange = std::max(n->maxRange, n->right->maxRange);
}
```

#### Balanced build
We needed to implement functions that would build balanced trees from sorted vectors, guaranteeing a minimum height. 
We used the divide and conquer approach: selecting the central element as the root and recursively building the left and right subtrees. That is why we created two balancing functions, one for simple elements(`buildBalanced`) and the other specialized for intervals(`buildBalancedFromIntervals`). We also added a function that checks that the tree is balanced(`isBalanced`) and two collectors to store the elements of the tree and thus balance them(`collectInorder and collectIntervalsInorder`). Finally, we added a public method to balance the tree(`balance`).

```cpp
private:
//Build balanced tree from sorted elements
Node<T>* buildBalanced(std::vector<T>& elements, int start, int end) {
    if (start > end) return nullptr;

    int mid = start + (end - start) / 2;
    Node<T>* node = new Node<T>(elements[mid]);

    node->left = buildBalanced(elements, start, mid - 1);
    node->right = buildBalanced(elements, mid + 1, end);

    updateMaxRange(node);
    return node;
}

//Build balanced tree from intervals
Node<T>* buildBalancedFromIntervals(std::vector<Interval<T>>& intervals, int start, int end) {
    if (start > end) return nullptr;

    int mid = start + (end - start) / 2;
    Node<T>* node = new Node<T>(intervals[mid]);

    node->left = buildBalancedFromIntervals(intervals, start, mid - 1);
    node->right = buildBalancedFromIntervals(intervals, mid + 1, end);

    updateMaxRange(node);
    return node;
}

//Collect elements inorder
void collectInorder(Node<T>* n, std::vector<T>& elements) const {
    if (n != nullptr) {
        collectInorder(n->left, elements);
        elements.push_back(n->getStart());
        collectInorder(n->right, elements);
    }
}

//Collect intervals inorder
void collectIntervalsInorder(Node<T>* n, std::vector<Interval<T>>& intervals) const {
    if (n != nullptr) {
        collectIntervalsInorder(n->left, intervals);
        if (n->isInterval()) {
            intervals.push_back(n->getRange());
        }
        collectIntervalsInorder(n->right, intervals);
    }
}

//Check if the tree is balanced
bool isBalanced(Node<T>* n, int& height) const {
    if (n == nullptr) {
        height = 0;
        return true;
    }

    int leftHeight = 0, rightHeight = 0;
    bool leftBalanced = isBalanced(n->left, leftHeight);
    bool rightBalanced = isBalanced(n->right, rightHeight);

    height = 1 + std::max(leftHeight, rightHeight);

    if (abs(leftHeight - rightHeight) <= 1 && leftBalanced && rightBalanced) {
        return true;
    }

    return false;
}

public: 

//Balance the tree
void balance() {
    if (empty()) return;

    std::vector<T> elements;
    collectInorder(this->root, elements);

    deleteAll(root);
    root = buildBalanced(elements, 0, elements.size() - 1);
}

//Build balanced tree from a vector of intervals
void buildFromIntervals(std::vector<Interval<T>>& intervals) {
    if (intervals.empty()) return;

    //Sort by interval start
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval<T>& a, const Interval<T>& b) {
            return a.start < b.start;
        });

    deleteAll(root);
    root = buildBalancedFromIntervals(intervals, 0, intervals.size() - 1);
    nelem = intervals.size();
}

//Check if the tree is balanced
bool isBalanced() const {
    int h = 0;
    return isBalanced(this->root, h);
}
```

#### Others
We added other methods such as finding the maximum (`max`), the minimum (`min`), the height of the tree (`height`), the size of the tree(`size`), and whether the tree is empty(`empty`).

```cpp
//Find maximum element in the tree
T max(Node<T>* n) const{
    if(n == nullptr){
        throw runtime_error("Empty tree");
    } else if (n->right != nullptr){
        return max(n->right);
    } else {
        return n->getStart();
    }
}

//Find minimum element in the tree
T min(Node<T>* n) const{
    if(n == nullptr){
        throw runtime_error("Empty tree");
    } else if (n->left != nullptr){
        return min(n->left);
    } else {
        return n->getStart();
    }
}

//Calculate height of the tree
int height(Node<T>* n) const {
    if (n == nullptr) {
        return 0;
    }
    int leftHeight = height(n->left);
    int rightHeight = height(n->right);
    return 1 + std::max(leftHeight, rightHeight);
}

//Get number of elements
int size() const {
    return this->nelem;
}

//Check if tree is empty
bool empty() const {
    return this->nelem == 0;
}
```

## Final Notes

If you’ve made it this far, thank you! We are incredibly grateful. We hope you found this documentation (and these classes) both useful and entertaining.

We worked hard to create something unique. Rather than settling for "flat" implementations, AI-generated snippets, or copy-pasted solutions often seen in Advent of Code. Since the challenges themselves weren't as algorithmically heavy as we expected, we decided to challenge ourselves in a different way: by building a robust, reusable framework. This pushed us to think outside the box and focus on general-purpose software design.

We would love to hear your feedback. We know the code isn't perfect (this is our first time implementing a system this complex from scratch) but we are eager to learn and improve for future projects.

Thanks for reading!