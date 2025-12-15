#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/Graph.h"

using namespace std;

int main() {
    ifstream file("../text/AoC11.txt");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    // Build the dependency graph
    Graph<string> graph;
    
    string line;
    while (getline(file, line)) {
        size_t dotsPos = line.find(':');
        string node = line.substr(0, dotsPos);
        string dependencies = line.substr(dotsPos + 2); // Skip ": "
        
        istringstream iss(dependencies);
        string dep;
        while (iss >> dep) {
            graph.addEdge(node, dep);  // edge from node to dep
        }
    }
    file.close();
    
    // Count paths from "you" to "out" using our Graph's countPaths method (DFS with memoization)
    long long result = graph.countPaths("you", "out");
    cout << "Number of paths from 'you' to 'out': " << result << endl;

    return 0;
}
