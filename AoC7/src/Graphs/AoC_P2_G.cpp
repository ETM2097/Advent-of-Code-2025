// This will be the implementation of the second part of the 7'th day of the Advent of Code 2025 challenge.
// Here we will use a Graph structure to represent the grid and we will use DFS with memoization
// to count the total number of ways the laser can reach the last row.


#include "../../include/Graph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// Here we define a memoization map
map<int, long long> memo; 

// Here we implement a DFS algorithm with memoization, further explanation in the README file
long long dfs(const Graph& graph, int nodeID) {
    // Base case: if we are in the last row, there is one valid path
    if (graph.isLastRow(nodeID)) {
        return 1;
    }

    // Second base case, the node has no neighbors
    if (graph.nodes[nodeID].neighbors.empty()) {
        return 0;
    }

    // Now we proceed with the memoization check
    // If we have already computed this node, we return the stored value
    if (memo.count(nodeID)) return memo[nodeID];

    // If we have not met any of the previous cases, we proceed with the DFS
    long long totalPaths = 0;

    // We explore each neighbor
    for (int neighborID : graph.nodes[nodeID].neighbors) {
        totalPaths += dfs(graph, neighborID);
        
    }
    // We store the computed value in the memoization map
    memo[nodeID] = totalPaths;

    // We return the total number of paths from this node
    return totalPaths;
}

int main() {
    // We use the same method that in the first part to read the input file and store it in a grid
    ifstream inputFile("data/AoC7.txt"); // We get the input from this file
    string line;
    vector<string> grid; // Here we will store the grid

    // We read the input file and store it in a grid
    while (getline(inputFile, line)) {
        grid.push_back(line);
    }
    inputFile.close();

    // Now, we create the graph from the grid
    Graph graph(grid);

    // Now we call the DFS from the starting node
    long long totalWays = dfs(graph, graph.startNodeID);

    cout << "Different time lines of the ray: " << totalWays << endl;
}