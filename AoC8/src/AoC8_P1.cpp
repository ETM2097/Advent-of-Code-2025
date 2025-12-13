#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/Graph.h"
#include "../include/UnionFind.h"

using namespace std;

int main() {
    // load graph from file
    Graph graph("data/input.txt");
    
    // calculate all distances
    graph.CAllDistance();
    
    // obtain the 1000 nearest edges
    vector<Edge> nearest = graph.getKNearestEdges(1000);
    
    // we use Union-Find to find connected components
    UnionFind uf(graph.getNumNodes());
    for(const Edge& e : nearest) {
        uf.unite(e.node1, e.node2);
    }
    
    // count sizes of each component
    vector<int> componentSizes;
    for(int i = 0; i < graph.getNumNodes(); i++) {
        if(uf.find(i) == i) {  // is root of component
            componentSizes.push_back(uf.getSize(i));
        }
    }
    
    // sort sizes and take the top 3
    sort(componentSizes.rbegin(), componentSizes.rend());
    
    // the result is the product of the sizes of the three largest components
    long long result = (long long)componentSizes[0] * componentSizes[1] * componentSizes[2];
    cout << "Resultado: " << result << endl;
    
    return 0;
}