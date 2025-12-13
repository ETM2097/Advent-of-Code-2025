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

    // Build the graph
    Graph<string> graph;
    
    string line;
    while (getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == string::npos) continue;
        
        string node = line.substr(0, colonPos);
        string depsStr = line.substr(colonPos + 2); // Skip ": "
        
        istringstream iss(depsStr);
        string dep;
        while (iss >> dep) {
            graph.addEdge(node, dep);  // edge from node to dep
        }
    }
    file.close();

    cout << "Total nodes: " << graph.size() << endl;
    
    // Here we count paths from "svr" to "out" that visit both "dac" and "fft"
    long long result = graph.countPathsThrough2("svr", "out", "dac", "fft");

    cout << "Paths from 'svr' to 'out' that visit both 'dac' and 'fft': " << result << endl;
    
    return 0;
}
