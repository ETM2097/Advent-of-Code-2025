# Implementations of Graph and UnionFind

## Graph.h
This contains the implementation of the Graph class that we used to represent the electrical junction boxes in 3D space. The graph is constructed to efficiently store and manage all junction boxes with their coordinates and calculate distances between them.

This implementation is specialized for the Day 8 challenge of Advent of Code 2025. The CPP code is in `src/Graph.cpp`.

### Graph's Implementation Highlights
- First we created a structure called `Node` that represents each electrical junction box. It has the following attributes:
    - `x`, `y`, `z`: the 3D coordinates of the box.

The code for the Node structure is as follows:
```cpp
struct Node {
    int x, y, z;
};
```
- We also created a structure called `Edge` to represent connections between boxes. It has the following attributes:
    - `distance` : the distance between two boxes.
    - `node1`, `node2`: the indices of the two boxes being connected.
    - `operator<`: allow sorting edges by distance in ascending order.

The code for the Edge structure is as follows:
```cpp
// struct for edge(Arista) between two nodes 
struct Edge {
    double distance;
    int node1, node2;
    
    bool operator<(const Edge& other) const {
        return distance < other.distance;
    }
};
```

- Then, we implemented the class `Graph` that has the following attributes:
    - `nodes`: a vector of Node objects to store all junction boxes.
    - `edges`: a vector of Edge objects to store all calculated distances between pairs.


The code of the constructor of the Graph is as follows:
```cpp
// constructor, read the file and load nodes
Graph::Graph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        Node node;

        stringstream ss(line);
        char comma; // this char it uses to skip the comas that separate the coordinates in the line. EX: 10,20,30
        ss >> node.x >> comma >> node.y >> comma >> node.z; // so we read x,y,z in a stringstream and store them in the node
        nodes.push_back(node);// add the node to the nodes vector
    }
    file.close();
}
```

- Next, we implement the `distance()` and `CAllDistance()` methods, which calculate the Euclidean distance between each pair of boxes and store them as Edge objects, respectively.

The code for the methods described is:
```cpp
// calculate distance between two nodes
double Graph::distance(int id1, int id2) const {
    const Node& n1 = nodes[id1];
    const Node& n2 = nodes[id2];

    // we use the Euclidean distance formula to calculate the distance between two nodes
    double dist = sqrt(pow((n1.x - n2.x), 2) + pow((n1.y - n2.y), 2) + pow((n1.z - n2.z), 2));

    return dist; 
}

// calculate all distances and sort
void Graph::CAllDistance() {
    int n = nodes.size();
    // we calculate the distance between each pair of nodes and store it in the edges vector
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Edge e;
            e.node1 = i;
            e.node2 = j;
            e.distance = distance(i, j);
            edges.push_back(e);
        }
    }
    // finally we sort the edges vector by distance
    sort(edges.begin(), edges.end()); 
}
```

- Next, we implement the method `getNumNodes()` that returns the total number of boxes.

The code of method is:
```cpp
// return number of nodes
int Graph::getNumNodes() const {
    return nodes.size();
}
```
- Finally, we create the last method to have all the necessary functionalities, `getKNearestEdges` that returns the K nearest pairs of boxes.

The code of method is: 
```cpp
// return the first K edges (closest)
vector<Edge> Graph::getKNearestEdges(int k) const {
    vector<Edge> result;
    for (int i = 0; i < k && i < edges.size(); i++) {
        result.push_back(edges[i]);
    }
    return result;
}
```

## UnionFind.h
This file contains the implementation of the Union-Find data structure (also known as Disjoint Set Union or DSU). We implement this structure to efficiently manage connected components and determine which junction boxes belong to the same electrical circuit.

We based our implementation on the following YouTube video: https://youtu.be/8uOIs4aXBiA?si=x4nSsaF8_1KiwwLO

### UnionFind's Implementation Highlights
We defined two attributes:
-   parent: a vector that stores that parent of each node.
-   size: a vector that stores the size of each component

-   The constructor initializes each node as its own parent with size 1.

The code of the constructor of the Union Find is as follows:
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
```
- The first method is `find` which searches for the root of the component containing node n. 

The code of method is: 
```cpp
// find the root of n 
int UnionFind::find(int n) {
    if(parent[n] != n) {
        parent[n] = find(parent[n]);
    }
    return parent[n];
}

```

- The second method is `unite` which merges the circuit containing nodes n1 and n2 into a single circuit.

The code of method is: 
```cpp
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
```

- Finally, the third method is `getSize` which return the size of the circuit containing node n.
The code of method is: 
```cpp
// get the size of the set that contains n
int UnionFind::getSize(int n) {
    return size[find(n)];
}
```
