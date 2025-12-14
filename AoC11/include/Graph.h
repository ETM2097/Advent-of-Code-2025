#include "HashMap.h"
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <stdexcept>

using namespace std;

template<typename NodeType, typename WeightType = int>
class Graph {
    private:
        //========================================================================================================================
        //                                                  Data Members
        //========================================================================================================================

        HashMap<NodeType, vector<NodeType>> forwardAdjacents; // Adjacency list representation
        HashMap<NodeType, vector<NodeType>> backwardAdjacents; // Reverse adjacency list
        HashMap<NodeType, vector<pair<NodeType, WeightType>>> weightedAdjacents; // For weighted graphs if needed in the future
        HashMap<NodeType, int> neighborCounts; // To store in-degrees
        set<NodeType> allNodes; // To store all unique nodes
        
        //========================================================================================================================
        //                                                  Helpers
        //========================================================================================================================

        // Helper for removing nodes
        void removeNodeFromGraph(const NodeType& node) {
            // Remove all outgoing edges
            if (forwardAdjacents.contains(node)) {
                vector<NodeType> neighbors = forwardAdjacents.get(node);
                for (const NodeType& neighbor : neighbors) {
                    // Decrement in-degree of neighbors
                    if (neighborCounts.contains(neighbor)) {
                        int currentDegree = neighborCounts.get(neighbor);
                    if (currentDegree > 0) {
                        neighborCounts.set(neighbor, currentDegree - 1);
                    }  
            }
                    removeEdge(node, neighbor); // Use removeEdge to handle outgoing edges
                }
            }

            // Remove all incoming edges
            if (backwardAdjacents.contains(node)) {
                vector<NodeType> neighbors = backwardAdjacents.get(node);
                for (const NodeType& neighbor : neighbors) {
                    removeEdge(neighbor, node); // Now we use removeEdge to handle incoming edges
                }
            }

            // Now we remove the node from all relevant data structures
            forwardAdjacents.remove(node);
            backwardAdjacents.remove(node);
            neighborCounts.remove(node);
            if (weightedAdjacents.contains(node)) {
                weightedAdjacents.remove(node); // Now we remove from weightedAdjacents if applicable 
            }
            allNodes.erase(node);
        }

        // Helper for adding edges
        void addEdgeToGraph(const NodeType& from, const NodeType& to) {
            forwardAdjacents.append(from, to); // We use the append method from our custom HashMap
            backwardAdjacents.append(to, from);
            int currentDegree = neighborCounts.contains(to) ? neighborCounts.get(to) : 0; // Here we get current in-degree
            neighborCounts.set(to, currentDegree + 1);
            allNodes.insert(from); // We use a set to avoid duplicates
            allNodes.insert(to);
        }

        // Helper method to add weighted edges to the graph
        void addEdgeToGraph(const NodeType& from, const NodeType& to, const WeightType& weight) {
            // We ensure that the graph is weighted
            if (weightedAdjacents.size() == 0 && !allNodes.empty()) {
                throw runtime_error("Graph is unweighted, cannot add weighted edges.");
            }
            // We add the edge to the adjacency lists
            addEdgeToGraph(from, to);

            // Then we add the weighted edge to weightedAdjacents
            // If 'from' does not exist yet in weightedAdjacents, we initialize it (empty vector)
            if (!weightedAdjacents.contains(from)) {
                weightedAdjacents.set(from, vector<pair<NodeType, WeightType>>());
            }
            // Finally we add the weighted edge
            weightedAdjacents.get(from).push_back({to, weight});
        }

        // Helper for removing edges
        void removeFromAdjacencyList(const NodeType& from, const NodeType& to) {
            // We remove 'to' from the forward adjacency list of 'from'
            if (forwardAdjacents.contains(from)) {
                vector<NodeType> neighbors = forwardAdjacents.get(from);
                neighbors.erase(remove(neighbors.begin(), neighbors.end(), to), neighbors.end());
                forwardAdjacents.set(from, neighbors);
            }
            // We remove 'from' from the backward adjacency list of 'to'
            if (backwardAdjacents.contains(to)) {
                vector<NodeType> neighbors = backwardAdjacents.get(to);
                neighbors.erase(remove(neighbors.begin(), neighbors.end(), from), neighbors.end());
                backwardAdjacents.set(to, neighbors);
            }
            // We update the in-degree count for 'to'
            if (neighborCounts.contains(to)) {
                int currentDegree = neighborCounts.get(to);
                if (currentDegree > 0) {
                    neighborCounts.set(to, currentDegree - 1);
                }
            }
        }

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

        // Helper for counting paths that visit both required nodes (DFS). Explained in README
        long long countPathsThrough2Helper(const NodeType& current, const NodeType& target,
                                           const NodeType& node1, const NodeType& node2,
                                           bool visited1, bool visited2,
                                           HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>>& memo) const {
            // Base case: reached target with both nodes visited
            if (current == target) {
                return (visited1 && visited2) ? 1 : 0; // We only count if both have been visited
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
            if (forwardAdjacents.contains(current)) { // If the fordward adjacents contains the current node
                vector<NodeType> neighbors = forwardAdjacents.get(current); // We get its neighbors
                for (const NodeType& neighbor : neighbors) { // And we iterate through them
                    totalPaths += countPathsThrough2Helper(neighbor, target, node1, node2, 
                                                          newVisited1, newVisited2, memo);
                }
            }
            
            // Memoize and return
            memo.set(state, totalPaths);
            return totalPaths;
        }

        // We will add a Dijkstra helper just for educational purposes, not used in AoC11
        vector<pair<NodeType, WeightType>> dijkstraHelper(const NodeType& start) const {
            // We assure the graph is weighted
            if (weightedAdjacents.size() == 0) {
                throw runtime_error("Graph is not weighted, cannot perform Dijkstra's algorithm.");
            }
            // We use a priority queue to store (distance, node)
            priority_queue<pair<WeightType, NodeType>, vector<pair<WeightType, NodeType>>, greater<pair<WeightType, NodeType>>> pq;
            HashMap<NodeType, WeightType> distances; // To store shortest distances (or whatever WeightType is)
            HashMap<NodeType, bool> visited; // To track visited nodes
            vector<pair<NodeType, WeightType>> result; // To store final distances 
            // Initialize
            pq.push({0, start}); // We start with distance 0 at start node
            distances.set(start, 0); // Distance to start is 0 as we begin there
            while (!pq.empty()) { // While there are nodes to process:
                auto [currentDist, currentNode] = pq.top(); // We get the node with the smallest distance
                pq.pop(); // We remove it from the queue
                if (visited.contains(currentNode)) { // We check if we have already processed this node
                    continue; // Already processed
                }
                visited.set(currentNode, true); // We set it as visited
                result.emplace_back(currentNode, currentDist); // Now we store the result
                // Then we explore neighbors
                if (weightedAdjacents.contains(currentNode)) { // If there are neighbors
                    for (const auto& [neighbor, weight] : weightedAdjacents.get(currentNode)) { // We iterate through them
                        WeightType newDist = currentDist + weight; // Here we calculate the new distance with that neighbor weight
                        if (!distances.contains(neighbor) || newDist < distances.get(neighbor)) { // If the path is shorter or not calculated yet
                            distances.set(neighbor, newDist); // We update the shortest distance
                            pq.push({newDist, neighbor}); // And we add it to the priority queue for further exploration
                        }
                    }
                }
            }
            return result;
        }

    public:
        //========================================================================================================================  
        //                                              Constructor & Destructor
        //========================================================================================================================
        Graph() {} // We initialize empty graph

        ~Graph() {
            clear();
        }

        //========================================================================================================================
        //                                                  Public Methods
        //========================================================================================================================

        // Construction of the graph from edges and nodes (NOT WEIGHTED)
        void addNode(const NodeType& node) {
            if (weightedAdjacents.size() > 0) {
                throw runtime_error("Graph is weighted, use addNode with weight parameter.");
            }
            allNodes.insert(node);
        }

        // Adds a node with weight (if the own nodes have weights, not the edges)
        void addNode(const NodeType& node, const WeightType& weight) {
            allNodes.insert(node);
            // We aadd it to weightedAdjacents with empty vector
            if (!weightedAdjacents.contains(node)) {
                weightedAdjacents.set(node, vector<pair<NodeType, WeightType>>());
            }
        }

        // Remove node from the graph
        void removeNode(const NodeType& node) {
            removeNodeFromGraph(node);
        }

        // Add edge to the graph
        void addEdge(const NodeType& from, const NodeType& to) {
            if (weightedAdjacents.size() > 0) {
                throw runtime_error("Graph is weighted, use addNode with weight parameter.");
            }
            addEdgeToGraph(from, to);
        }

        // Adds a weighted edge to the graph
        void addEdge(const NodeType& from, const NodeType& to, const WeightType& weight) {
            addEdgeToGraph(from, to, weight);

        }

        // Remove edge from the graph
        void removeEdge(const NodeType& from, const NodeType& to) {
            removeFromAdjacencyList(from, to);
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

        // Clear function
        void clear() {
            forwardAdjacents.clear();
            backwardAdjacents.clear();
            neighborCounts.clear();
            weightedAdjacents.clear();
            allNodes.clear();
        }

        // Count all paths from start node to end node using DFS with memoization
        long long countPaths(const NodeType& start, const NodeType& end) const {
            HashMap<NodeType, long long> memo;
            return countPathsHelper(start, end, memo);
        }

        // Count paths from start to end that visit both node1 AND node2
        long long countPathsThrough2(const NodeType& start, const NodeType& end, 
                                     const NodeType& node1, const NodeType& node2) const {
            // State: (current, visited_node1, visited_node2)
            HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>> memo; // Custom hash for the tuple explained in README
            return countPathsThrough2Helper(start, end, node1, node2, false, false, memo);
        }

        // Dijkstra's algorithm for shortest paths from start node (not used in AoC11)
        vector<pair<NodeType, WeightType>> dijkstraHelper(const NodeType& start) const {
            return dijkstraHelper(start);
        }

        //========================================================================================================================
        //                                                  Getters
        //========================================================================================================================

        // Check if an edge exists between two nodes
        bool hasEdgeFordward(const NodeType& from, const NodeType& to) const {
            if (forwardAdjacents.contains(from)) {
                vector<NodeType> neighbors = forwardAdjacents.get(from);
                return find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
            }
            return false;
        }

        bool hasEdgeBackward(const NodeType& to, const NodeType& from) const {
            if (backwardAdjacents.contains(to)) {
                vector<NodeType> neighbors = backwardAdjacents.get(to);
                return find(neighbors.begin(), neighbors.end(), from) != neighbors.end();
            }
            return false;
        }

        bool hasEdgeBoth(const NodeType& from, const NodeType& to) const {
            return hasEdgeFordward(from, to) && hasEdgeBackward(to, from);
        }

        // Get the number of neighbors of a node (outgoing edges)
        int getNeighborCount(const NodeType& node) const {
            if (neighborCounts.contains(node)) {
                return neighborCounts.get(node);
            }
            return 0;
        }

        // Get the total of Nodes in the graph
        int getTotalNodes() const {
            return allNodes.size();
        }

        // EXTRA

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