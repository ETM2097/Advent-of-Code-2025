#include "../include/Graph.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

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

// return number of nodes
int Graph::getNumNodes() const {
    return nodes.size();
}

// return the first K edges (closest)
vector<Edge> Graph::getKNearestEdges(int k) const {
    vector<Edge> result;
    for (int i = 0; i < k && i < edges.size(); i++) {
        result.push_back(edges[i]);
    }
    return result;
}