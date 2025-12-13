#include "HashMap.h"
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <stdexcept>

using namespace std;

template<typename NodeType>
class Graph {
    private:
        HashMap<NodeType, vector<NodeType>> forwardAdjacents; // Adjacency list representation
        HashMap<NodeType, vector<NodeType>> backwardAdjacents; // Reverse adjacency list
        HashMap<NodeType, int> neighborCounts; // To store in-degrees
        set<NodeType> allNodes; // To store all unique nodes

        // Helper function for recursive DFS with memoization
        long long countPathsHelper(const NodeType& current, const NodeType& target, 
                                   HashMap<NodeType, long long>& memo) const {
            
            // Base case: reached target
            if (current == target) {
                return 1;
            }
            
            // Check memo
            if (memo.contains(current)) {
                return memo.get(current);
            }
            
            // Recursively count paths through all outgoing edges
            long long totalPaths = 0;
            if (forwardAdjacents.contains(current)) {
                vector<NodeType> neighbors = forwardAdjacents.get(current);
                for (const NodeType& neighbor : neighbors) {
                    totalPaths += countPathsHelper(neighbor, target, memo);
                }
            }
            
            // Memoize and return
            memo.set(current, totalPaths);
            return totalPaths;
        }

        // Helper for counting paths that visit both required nodes explained in README
        long long countPathsThrough2Helper(const NodeType& current, const NodeType& target,
                                           const NodeType& node1, const NodeType& node2,
                                           bool visited1, bool visited2,
                                           HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>>& memo) const {
            // Base case: reached target with both nodes visited
            if (current == target) {
                return (visited1 && visited2) ? 1 : 0;
            }
            
            // Create state for memoization
            tuple<NodeType, bool, bool> state = make_tuple(current, visited1, visited2); // make_tuple from <tuple>
            
            // Check memo
            if (memo.contains(state)) {
                return memo.get(state);
            }
            
            // Update visited flags if we're at one of the required nodes:
            bool newVisited1 = visited1 || (current == node1); // Update if we reached node1 or node2
            bool newVisited2 = visited2 || (current == node2); // Just checks fi current matches and keeps previous state
            
            // Recursively count paths through all outgoing edges
            long long totalPaths = 0;
            if (forwardAdjacents.contains(current)) {
                vector<NodeType> neighbors = forwardAdjacents.get(current);
                for (const NodeType& neighbor : neighbors) {
                    totalPaths += countPathsThrough2Helper(neighbor, target, node1, node2, 
                                                          newVisited1, newVisited2, memo);
                }
            }
            
            // Memoize and return
            memo.set(state, totalPaths);
            return totalPaths;
        }

    public:
        // Constructor
        Graph() {} // We initialize empty graph

        // Construction of the graph from edges and nodes
        void addNode(const NodeType& node) {
            allNodes.insert(node);
        }

        void addEdge(const NodeType& from, const NodeType& to) {
            forwardAdjacents.append(from, to);
            backwardAdjacents.append(to, from);
            int currentDegree = neighborCounts.contains(to) ? neighborCounts.get(to) : 0; // Here we get current in-degree
            neighborCounts.set(to, currentDegree + 1);
            allNodes.insert(from); // We use a set to avoid duplicates
            allNodes.insert(to);
        }

        // Getters

        // Checks if a node exists in the graph
        bool hasNode(const NodeType& node) const {
            return allNodes.find(node) != allNodes.end();
        }

        // Get the forward neighbors of a node
        vector<NodeType> getForwardNeighbors(const NodeType& node) const {
            if (forwardAdjacents.contains(node)) {
                return forwardAdjacents.get(node);
            }
            return vector<NodeType>(); // Return empty vector if no neighbors
        }

        // Get the backward neighbors of a node
        vector<NodeType> getBackwardNeighbors(const NodeType& node) const {
            if (backwardAdjacents.contains(node)) {
                return backwardAdjacents.get(node);
            }
            return vector<NodeType>(); // Return empty vector if no neighbors
        }

        // Get in-degree of a node
        int getInDegree(const NodeType& node) const {
            if (neighborCounts.contains(node)) {
                return neighborCounts.get(node);
            }
            return 0; // If node has no incoming edges
        }

        // Get the size of the graph (number of unique nodes)
        size_t size() const {
            return allNodes.size();
        }

        // Get all nodes in the graph with no dependencies (leaf nodes)
        vector<NodeType> getLeafNodes() const {
            vector<NodeType> leaves;
            for (const NodeType& node : allNodes) {
                if (getInDegree(node) == 0) {
                    leaves.push_back(node);
                }
            }
            return leaves;
        }

        // Get all nodes in the graph
        vector<NodeType> getAllNodes() const {
            vector<NodeType> nodes;
            for (const NodeType& node : allNodes) {
                nodes.push_back(node);
            }
            return nodes;
        }

        //Methods

        // Count all paths from start node to end node using DFS with memoization
        long long countPaths(const NodeType& start, const NodeType& end) const {
            HashMap<NodeType, long long> memo;
            return countPathsHelper(start, end, memo);
        }

        // Count paths from start to end that visit both node1 AND node2
        long long countPathsThrough2(const NodeType& start, const NodeType& end, 
                                     const NodeType& node1, const NodeType& node2) const {
            // State: (current, visited_node1, visited_node2)
            HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>> memo;
            return countPathsThrough2Helper(start, end, node1, node2, false, false, memo);
        }

        // Topological sort for AoC11_P1, we ended not using it but is fully implemented, explained in detail in the README
        vector<NodeType> topologicalSort() const {
            // Copy in-degrees for all nodes, handling nodes with no incoming edges
            HashMap<NodeType, int> Degrees;
            for (const auto& node : allNodes) {
                int degree = neighborCounts.contains(node) ? neighborCounts.get(node) : 0;
                Degrees.set(node, degree);
            }

            // Now we implement a queue for processing nodes with in-degree 0
            queue<NodeType> processingQueue;
            for(const auto& node : allNodes) { // We use auto in order to let the compiler decide the best type
                if (Degrees.get(node) == 0) {
                    processingQueue.push(node);
                }
            }

            // Now we create a vector where we will store the result
            vector<NodeType> sortedOrder;
            while (!processingQueue.empty()) {
                NodeType toCheck = processingQueue.front(); // We get the front node
                processingQueue.pop(); // We remove it from the queue
                sortedOrder.push_back(toCheck); // We add it to the sorted order

                // Now we process the dependents of toCheck (nodes that depend on this one)
                // We use backward adjacents because we need to update nodes that depend on toCheck
                if (backwardAdjacents.contains(toCheck)) {
                    for (const NodeType& dependent : backwardAdjacents.get(toCheck)) {
                        int currentDegree = Degrees.get(dependent);
                        Degrees.set(dependent, currentDegree - 1); // Decrease in-degree
                        if (Degrees.get(dependent) == 0) {
                            processingQueue.push(dependent); // If in-degree is 0, add to queue
                        }
                    }
                }
            }
            return sortedOrder;
        }
};