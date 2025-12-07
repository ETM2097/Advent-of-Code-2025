// Here we will implement tha solution to the first part of Advent of Code 2025 day 7 using a graph structure
// We will use BFS to traverse the graph and count the number of splitters (^) the laser hits until it reaches the last row
// We just count the splitters once because we just need to know how many times the laser is divided

#include "../../include/Graph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>

using namespace std;

long long bfs(const Graph& graph, int startNodeID) {
    // Now we implement the BFS to traverse the graph and count the splitters, an in-depth explanation will be in the README file
    // We create a queue for the BFS
    queue<int> bfs;
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
        return totalSplittings;
    }
}

int main() {
    // We use the same method in order to read the input file and store it in a grid
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

    // Now we call the BFS from the starting node
    long long totalSplittings = bfs(graph, graph.startNodeID);
    
    cout << "Total splitters hit: " << totalSplittings << endl;
}