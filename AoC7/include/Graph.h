#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

// Here is the structure of a Node in the graph
struct Node {
    int rows; // It indicates the row of the cell
    int cols; // It indicates the column of the cell
    char cellType; // Here we store the type of cell
    vector<int> neighbors; // Store indices of neighboring nodes
};

// Now we define the Graph class that will implement methods to work with the grid
class Graph {
    public:
        vector<Node> nodes; // This will store all the nodes in the graph
        int rows; // Number of rows in the grid
        int cols; // Number of columns in the grid
        int startNodeID; // ID of the starting node (where 'S' is located)

        // Constructor to initialize the graph from a grid (that we take as input), we will just use this constructor as this
        // is an specialized graph for this problem, there will be no base constructor or other overloads
        Graph(const vector<string>& grid);
        
        // Method to get the Node ID based on row and column (transforms 2D to 1D)
        int getNodeID(int row, int col) const;

        // Method to get the row of a node based on its ID
        pair<int, int> getRowCol(int nodeID) const;

        // Method to get the number of nodes in the graph
        int getNumNodes() const;

        // Getter for the last row of the grid
        bool isLastRow(int row) const;
};

#endif