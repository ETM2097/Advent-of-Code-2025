#include "../../include/Graph.h"

// Constructor to initialize the graph from a grid (that we take as input)
Graph::Graph(const vector<string>& grid) {
    rows = grid.size();
    cols = grid[0].size();
    startNodeID = -1;
    nodes.resize(rows * cols);

    // Here we initialize the nodes based on the grid
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            // Here we implement an ID for processing the nodes easier as it will be a 1D vector instead of 2D
            int iD = getNodeID(row, col);
            nodes[iD] = new Node();
            // Here we put the information of the node into the graph
            nodes[iD]->rows = row;
            nodes[iD]->cols = col;
            nodes[iD]->cellType = grid[row][col];

            // If we find the starting node, we store its ID
            if (grid[row][col] == 'S') {
                startNodeID = iD;
            }
        }
    }
    
    // Now we have to build the edges, that are based on the laser movement rules
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            // We transform back to the 1D 2D index
            int currentID = getNodeID(row, col);
            // We get the type of cell for processing the neighbors
            char cell = nodes[currentID]->cellType;
            if (cell == '^') {
                if (col - 1 >= 0) {
                    nodes[currentID]->neighbors.push_back(getNodeID(row + 1, col - 1));
                }
                if (col + 1 < cols) {
                    nodes[currentID]->neighbors.push_back(getNodeID(row + 1, col + 1));
                }
            } 
            else if (cell == '.' || cell == 'S') {
                // Empty cell or Start: it connects directly downwards, so just connects to the node below
                if (row + 1 < rows) {
                    nodes[currentID]->neighbors.push_back(getNodeID(row + 1, col));
                }
            }
        }   
    }
}

int Graph::getNodeID(int row, int col) const {
    return row * cols + col;
}

pair<int, int> Graph::getRowCol(int nodeID) const {
    int row = nodeID / cols;
    int col = nodeID % cols;
    return {row, col};
}

int Graph::getNumNodes() const {
    return nodes.size();
}

bool Graph::isLastRow(int nodeID) const {
    return nodes[nodeID]->rows == rows - 1;
}