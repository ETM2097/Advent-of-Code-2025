#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

// struct for Node
struct Node {
    int x, y, z; // 3D coordinates
};

// struct for edge(Arista) between two nodes 
struct Edge {
    double distance;
    int node1, node2;
    
    bool operator<(const Edge& other) const {
        return distance < other.distance;
    }
};

class Graph {
    public:
        vector<Node> nodes;
        vector<Edge> edges;
        
        // Constructor: lee archivo y carga nodos
        Graph(const string& filename);
        
        // Calcula distancia euclidiana entre dos nodos
        double distance(int id1, int id2) const;
        
        // Calcula todas las distancias y ordena (para obtener las 1000 menores)
        void CAllDistance();
        
        // Retorna cantidad de nodos
        int getNumNodes() const;
        
        // Retorna los K primeros edges (más cercanos)
        vector<Edge> getKNearestEdges(int k) const;
};

#endif