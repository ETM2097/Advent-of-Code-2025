#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/Graph.h"
#include "../include/UnionFind.h"

using namespace std;

int main() {
    // we load graph from file
    Graph graph("data/input.txt");
    
    // we calculate all distances
    graph.CAllDistance();
    
    // we use Union-Find to find connected components
    UnionFind uf(graph.getNumNodes());
    int lastNode1 = -1, lastNode2 = -1;
    
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
    
    // we multiply X coordinates of the last two connected nodes
    long long result = (long long)graph.nodes[lastNode1].x * graph.nodes[lastNode2].x;
    
    cout << "Resultado: " << result << endl;
    
    return 0;
}