#include "HashMap.h"
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <tuple>
#include <utility>
#include <functional>

// IMPORTANT UPDATE 1: We now use getRef() method from HashMap to avoid unnecessary copying of vectors when getting adjacency lists, it returns
// a const reference to the vector stored in the HashMap, improving performance. Changed in several places in the code below.

using namespace std;

template<typename NodeType, typename WeightType = int, typename NodeDataType = int>
class Graph {
    private:
        //========================================================================================================================
        //                                                  Data Members
        //========================================================================================================================

        HashMap<NodeType, vector<NodeType>> forwardAdjacents; // Adjacency list representation
        HashMap<NodeType, vector<NodeType>> backwardAdjacents; // Reverse adjacency list
        HashMap<NodeType, vector<pair<NodeType, WeightType>>> weightedAdjacents; // For weighted graphs if needed in the future, 
                                                                                 // stores pairs of (neighbor, weight) for each node.
        HashMap<NodeType, int> inDegrees; // To store in-degrees
        set<NodeType> allNodes; // To store all unique nodes
        HashMap<NodeType, NodeDataType> data; // To store data associated with each node
        bool isWeighted = false; // Flag to indicate if the graph is weighted
        bool hasNodeData = false; // Flag to indicate if nodes have associated data
        bool isDirected = true; // Flag to indicate if the graph is directed
        
        //========================================================================================================================
        //                                                  Helpers
        //========================================================================================================================

        // Helper for removing nodes
        void removeNodeFromGraph(const NodeType& node) {
            // Remove all outgoing edges
            if (forwardAdjacents.contains(node)) {
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(node); // We get by reference to avoid copying (new method in HashMap)
                                                                                   // const is used to avoid modifying the original vector, (The vector in the HashMap)
                                                                                   // as we only need to read it here.
                for (const NodeType& neighbor : neighbors) {
                    removeEdge(node, neighbor); // Here e use removeEdge to handle outgoing edges
                }
            }

            // Remove all incoming edges
            if (backwardAdjacents.contains(node)) {
                const vector<NodeType>& neighbors = backwardAdjacents.getRef(node);
                for (const NodeType& neighbor : neighbors) {
                    removeEdge(neighbor, node); // Now we use removeEdge to handle incoming edges
                }
            }

            // Now we remove the node from all relevant data structures
            forwardAdjacents.remove(node);
            backwardAdjacents.remove(node);
            inDegrees.remove(node);
            if (weightedAdjacents.contains(node)) {
                weightedAdjacents.remove(node); // Now we remove from weightedAdjacents if applicable 
            }
            allNodes.erase(node);
        }

        // Helper for adding edges
        void addEdgeToGraph(const NodeType& from, const NodeType& to) {
            forwardAdjacents.append(from, to); // We use the append method from our custom HashMap
            backwardAdjacents.append(to, from);
            int currentDegree = inDegrees.contains(to) ? inDegrees.get(to) : 0; // Here we get current in-degree
            inDegrees.set(to, currentDegree + 1);
            allNodes.insert(from); // We use a set to avoid duplicates
            allNodes.insert(to);
        }

        // Helper method to add weighted edges to the graph
        void addEdgeToGraph(const NodeType& from, const NodeType& to, const WeightType& weight) {
            // We add the edge to the adjacency lists
            addEdgeToGraph(from, to);

            // Then we add the weighted edge to weightedAdjacents
            // If 'from' does not exist yet in weightedAdjacents, we initialize it (empty vector)
            if (!weightedAdjacents.contains(from)) {
                weightedAdjacents.set(from, vector<pair<NodeType, WeightType>>());
            }
            // Finally we add the weighted edge
            auto neighbors = weightedAdjacents.get(from);
            neighbors.emplace_back(to, weight);
            weightedAdjacents.set(from, move(neighbors));
        }

        // Helper for removing edges
        void removeFromAdjacencyList(const NodeType& from, const NodeType& to) {
            // We remove 'to' from the forward adjacency list of 'from'
            if (forwardAdjacents.contains(from)) {
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(from);
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
            if (inDegrees.contains(to)) {
                int currentDegree = inDegrees.get(to);
                if (currentDegree > 0) {
                    inDegrees.set(to, currentDegree - 1);
                }
            }
        }

        // Helper function for recursive DFS with memoization
        long long countPathsHelper(const NodeType& current, const NodeType& target, 
                                   HashMap<NodeType, long long>& memo) const {
            // Check memo
            if (memo.contains(current)) {
                return memo.get(current);
            }

            // Base case: reached target
            if (current == target) {
                memo.set(current, 1);
                return 1;
            }
            
            // Recursively count paths through all outgoing edges
            long long totalPaths = 0;
            if (forwardAdjacents.contains(current)) {
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(current);
                for (const NodeType& neighbor : neighbors) {
                    totalPaths += countPathsHelper(neighbor, target, memo);
                }
            }
            
            // Memoize and return
            memo.set(current, totalPaths);
            return memo.get(current);
        }

        // Helper for counting paths that visit both required nodes (DFS). Explained in README
        long long countPathsThrough2Helper(const NodeType& current, const NodeType& target,
                                           const NodeType& node1, const NodeType& node2,
                                           bool visited1, bool visited2,
                                           HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>>& memo) const {
            // Create state for memoization
            tuple<NodeType, bool, bool> state = make_tuple(current, visited1, visited2); // make_tuple from <tuple>
            
            // We check memo first
            if (memo.contains(state)) {
                return memo.get(state);
            }
            
            // Base case: reached target with both nodes visited
            if (current == target) {
                long long result = (visited1 && visited2) ? 1 : 0; // We only count if both have been visited
                memo.set(state, result); // Memoize base case
                return result;
            }
            
            // Update visited flags if we're at one of the required nodes:
            bool newVisited1 = visited1 || (current == node1); // Update if we reached node1 or node2
            bool newVisited2 = visited2 || (current == node2); // Just checks fi current matches and keeps previous state
            
            // Recursively count paths through all outgoing edges
            long long totalPaths = 0;
            if (forwardAdjacents.contains(current)) { // If the fordward adjacents contains the current node
                const vector<NodeType>& neighbors = forwardAdjacents.getRef(current); // We get its neighbors
                for (const NodeType& neighbor : neighbors) { // And we iterate through them
                    totalPaths += countPathsThrough2Helper(neighbor, target, node1, node2, 
                                                          newVisited1, newVisited2, memo);
                }
            }
            
            // Memoize and return
            memo.set(state, totalPaths);
            return totalPaths;
        }

        // BFS helper for shortest path
        vector<NodeType> bfsShortestPathHelper(const NodeType& start, const NodeType& end) const {
            if (start == end) return {start}; // Trivial case

            HashMap<NodeType, bool> visited; // To track visited nodes
            HashMap<NodeType, NodeType> parent; // To reconstruct the path
            queue<NodeType> q; // BFS queue

            q.push(start); // We push start node to the queue
            visited.set(start, true); // We mark it as visited

            bool found = false; // Flag to indicate if we found the end node
            while (!q.empty() && !found) { // While there are nodes to process and we haven't found the end
                NodeType current = q.front(); q.pop(); // We get the front node and remove it from the queue
                if (!forwardAdjacents.contains(current)) continue; // If there are no neighbors we must not check this node
                for (const NodeType& neighbor : forwardAdjacents.getRef(current)) { // We iterate through its neighbors
                    if (visited.contains(neighbor)) continue; // If already visited, skip
                    visited.set(neighbor, true); // We mark it as visited
                    parent.set(neighbor, current); // We set its parent for path reconstruction
                    if (neighbor == end) { // If we found the end node, we stop
                        found = true;
                        break;
                    }
                    q.push(neighbor); // Otherwise, we add the neighbor to the queue
                }
            }

            // If we didn't find the end node, return empty path
            if (!found) return {};

            // Else we reconstruct the path from end to start using the parent map
            vector<NodeType> path;
            for (NodeType at = end; /* NO CONDITION */ ; at = parent.get(at)) { // We go backwards from end to start
                path.push_back(at);
                if (at == start) break;
            }
            reverse(path.begin(), path.end()); // We reverse the path to get it from start to end
            return path; // Fianlly we return the path
        }

        // We will add a Dijkstra helper just for educational purposes, not used in AoC11
        vector<pair<NodeType, WeightType>> dijkstraHelper(const NodeType& start) const {
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
                    for (const auto& [neighbor, weight] : weightedAdjacents.getRef(currentNode)) { // We iterate through them
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
        // Default constructor, directed, unweighted, no node data
        Graph() {}
        // Custom constructor
        Graph(bool directed, bool weighted, bool nodeData) : isDirected(directed), isWeighted(weighted), hasNodeData(nodeData) {} 

        ~Graph() {
            clear();
        }

        //========================================================================================================================
        //                                                  Public Methods
        //========================================================================================================================

        // Construction of the graph from edges and nodes (NOT WEIGHTED)
        void addNode(const NodeType& node) {
            if(!hasNode(node)) allNodes.insert(node);
        }


        // Adds a node with data (if the own nodes have data, not the edges)
        void addNode(const NodeType& node, const NodeDataType& nodeData) {
            if(!hasNodeData) {
                throw runtime_error("This graph's nodes do not have associated data.");
            }
            allNodes.insert(node);
            // We aadd it to weightedAdjacents with empty vector
            data.set(node, nodeData);
        }

        // Remove node from the graph
        void removeNode(const NodeType& node) {
            removeNodeFromGraph(node);
        }

        // Add edge (from, to) to the graph. If you add an edge with nodes that do not exist, they are created without data. Use addNode beforehand if you want data
        // or set it later with setNodeData() method.
        void addEdge(const NodeType& from, const NodeType& to) {
            if (isWeighted) {
                throw runtime_error("Graph is weighted, use addNode with weight parameter.");
            }
            if (!hasNode(from) || !hasNode(to)) {
                // We create the nodes without data if they do not exist
                addNode(from);
                addNode(to);
            }
            addEdgeToGraph(from, to);
            if (!isDirected) {
                addEdgeToGraph(to, from); // For undirected graphs, we add the reverse edge
            }
        }

        // Adds a weighted edge to the graph (from, to, weight). If you add an edge with nodes that do not exist, they are created without data. 
        // Use addNode beforehand if you want data or set it later with setNodeData() method.
        void addEdge(const NodeType& from, const NodeType& to, const WeightType& weight) {
            if (!isWeighted) {
                throw runtime_error("Graph is unweighted, cannot add weighted edges.");
            }
            if (!hasNode(from) || !hasNode(to)) {
                // We create the nodes without data if they do not exist
                addNode(from);
                addNode(to);
            }
            addEdgeToGraph(from, to, weight);
            if (!isDirected) {
                addEdgeToGraph(to, from, weight); // For undirected graphs, we add the reverse edge
            }
        }

        // Remove edge (from, to) from the graph
        void removeEdge(const NodeType& from, const NodeType& to) {
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            removeFromAdjacencyList(from, to);
            if (!isDirected) {
                removeFromAdjacencyList(to, from); // For undirected graphs, we remove the reverse edge
            }
        }

        // Clear function, removes all nodes and edges
        void clear() {
            forwardAdjacents.clear();
            backwardAdjacents.clear();
            inDegrees.clear();
            weightedAdjacents.clear();
            allNodes.clear();
            data.clear();
        }

        // Count all paths from start node to end node using DFS with memoization (start, end), only for DAGs
        long long countPaths(const NodeType& start, const NodeType& end) const {
            if (!hasNode(start) || !hasNode(end)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            if (!isDirected) {
                throw runtime_error("Graph must be directed to count paths using this method.");
            }
            HashMap<NodeType, long long> memo;
            return countPathsHelper(start, end, memo);
        }

        // Count paths from start to end that visit both node1 AND node2 using DFS with memoization (start, end, node1, node2)
        long long countPathsThrough2(const NodeType& start, const NodeType& end, 
                                     const NodeType& node1, const NodeType& node2) const {
            if (!hasNode(start) || !hasNode(end)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            if (!isDirected) {
                throw runtime_error("Graph must be directed to count paths using this method.");
            }
            // State: (current, visited_node1, visited_node2)
            HashMap<tuple<NodeType, bool, bool>, long long, TupleHash3<NodeType, bool, bool>> memo; // Custom hash for the tuple explained in README
            return countPathsThrough2Helper(start, end, node1, node2, false, false, memo);
        }

        // Common BFS implementation for shortest path (not used in AoC11)
        vector<NodeType> bfsShortestPath(const NodeType& start, const NodeType& end) const {
            return bfsShortestPathHelper(start, end);
        }

        // Dijkstra's algorithm for shortest paths from start node (not used in AoC11)
        vector<pair<NodeType, WeightType>> dijkstra(const NodeType& start) const {
            // We assure the graph is weighted
            if (!isWeighted) {
                throw runtime_error("Graph is not weighted, cannot perform Dijkstra's algorithm.");
            }
            if (!hasNode(start)) throw runtime_error("Start node must exist in the graph.");
            return dijkstraHelper(start);
        }

        //========================================================================================================================
        //                                                  Getters
        //========================================================================================================================

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
            if (!hasNode(node)) {
                throw runtime_error("Node does not exist in the graph.");
            }
            if (inDegrees.contains(node)) {
                return inDegrees.get(node);
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
                if (getForwardNeighbors(node).empty()) {
                    leaves.push_back(node);
                }
            }
            return leaves;
        }

        // Get all nodes in the graph with no incoming edges (in-degree 0)
        vector<NodeType> getRootNodes() const {
            vector<NodeType> roots;
            for (const NodeType& node : allNodes) {
                if (getInDegree(node) == 0) {
                    roots.push_back(node);
                }
            }
            return roots;
        }

        // Get all nodes in the graph
        vector<NodeType> getAllNodes() const {
            vector<NodeType> nodes;
            for (const NodeType& node : allNodes) {
                nodes.push_back(node);
            }
            return nodes;
        }

        // Check if an edge exists in the forward direction
        bool hasEdgeForward(const NodeType& from, const NodeType& to) const {
            if(!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            if (forwardAdjacents.contains(from)) {
                vector<NodeType> neighbors = forwardAdjacents.get(from);
                return find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
            }
            return false;
        }

        // Check if an edge exists in the backward direction
        bool hasEdgeBackward(const NodeType& to, const NodeType& from) const {
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            if (backwardAdjacents.contains(to)) {
                vector<NodeType> neighbors = backwardAdjacents.get(to);
                return find(neighbors.begin(), neighbors.end(), from) != neighbors.end();
            }
            return false;
        }

        // Check if an edge exists in both directions (for undirected graphs)
        bool hasEdgeBoth(const NodeType& from, const NodeType& to) const {
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            return hasEdgeForward(from, to) && hasEdgeBackward(to, from);
        }

        // Get the number of neighbors of a node (outgoing edges)
        int getOutDegree(const NodeType& node) const {
            if (!hasNode(node)) {
                throw runtime_error("Node does not exist in the graph.");
            }
            if (forwardAdjacents.contains(node)) {
                return forwardAdjacents.getRef(node).size();
            }
            return 0;
        }

        // Get node data
        NodeDataType getNodeData(const NodeType& node) const {
            if (!hasNodeData) {
                throw runtime_error("This graph's nodes do not have associated data.");
            }
            if (!hasNode(node)) {
                throw runtime_error("Node does not exist in the graph.");
            }
            return data.get(node);
        }

        // Get the total of Nodes in the graph
        int getTotalNodes() const {
            return allNodes.size();
        }

        // Get weight of an edge (from, to)
        WeightType getWeight(const NodeType& from, const NodeType& to) const {
            if (!isWeighted) throw runtime_error("Graph is not weighted, cannot get weights.");
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            if (!weightedAdjacents.contains(from)) throw runtime_error("Edge does not exist to get weight.");
            const auto& edges = weightedAdjacents.getRef(from);
            for (const auto& e : edges) {
                if (e.first == to) {
                    return e.second;
                }
            }
            throw runtime_error("Edge does not exist to get weight.");
        }

        //========================================================================================================================
        //                                                 Setters
        //========================================================================================================================

        // Set node data (node, nodeData)
        void setNodeData(const NodeType& node, const NodeDataType& nodeData) {
            if (!hasNodeData) {
                throw runtime_error("This graph's nodes do not have associated data.");
            }
            if (!hasNode(node)) {
                throw runtime_error("Node does not exist in the graph.");
            }
            data.set(node, nodeData);
        }

        // Set weight of an edge (from, to)
        void setWeight(const NodeType& from, const NodeType& to, const WeightType& weight) {
            if (!isWeighted) throw runtime_error("Graph is not weighted, cannot set weights.");
            if (!hasNode(from) || !hasNode(to)) {
                throw runtime_error("Both nodes must exist in the graph.");
            }
            // update forward weighted edge
            if (!weightedAdjacents.contains(from)) throw runtime_error("Edge does not exist to set weight.");
            auto edges = weightedAdjacents.get(from);
            bool found = false;
            for (auto& e : edges) {
                if (e.first == to) { 
                    e.second = weight; 
                    found = true; 
                    break; 
                }
            }
            if (!found) throw runtime_error("Edge does not exist to set weight.");
            // we set back the modified edge weights
            weightedAdjacents.set(from, std::move(edges));

            // if undirected, we update reverse weighted edge
            if (!isDirected) {
                auto reverse = weightedAdjacents.get(to);
                bool foundRev = false;
                for (auto& e : reverse) {
                    if (e.first == from) { 
                        e.second = weight; 
                        foundRev = true; 
                        break; 
                    }
                }
                if (!foundRev) reverse.emplace_back(from, weight);
                weightedAdjacents.set(to, move(reverse));
            }
        }

        // EXTRA

        // Topological sort for AoC11_P1 as we misunderstood the challenge, we ended not using it but is fully implemented, explained in the README
        vector<NodeType> topologicalSort() const {
            // Copy in-degrees for all nodes, handling nodes with no incoming edges
            HashMap<NodeType, int> Degrees;
            for (const auto& node : allNodes) {
                int degree = inDegrees.contains(node) ? inDegrees.get(node) : 0;
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
                    for (const NodeType& dependent : backwardAdjacents.getRef(toCheck)) {
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