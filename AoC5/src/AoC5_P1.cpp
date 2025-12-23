// This will be the implementation for the 5th day's challenge of Advent of Code.
// Today consists of processing a list of boarding passes to determine fresh product IDs 
// and checking from a list which IDs are fresh.

#include "../../INCLUDE/Tree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Example usage for AoC5
int main() {
    // Tree object
    Tree<long long> tree;

    // Vector for storing intervals
    vector<Interval<long long>> intervals;

    // Vector for storing single ID requests
    vector<long long> IDs;

    // Here we get the input file
    ifstream file("texts/AoC5.txt");
    string line;

    // Parse input
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t endFirstId = line.find('-');
        // We check if it's a range or a single number
        if  (endFirstId != string::npos && endFirstId > 0) {
            long long start = stoll(line.substr(0, endFirstId));
            long long end = stoll(line.substr (endFirstId + 1));
            intervals.push_back({start, end});
        } else {
            // Single number query
         IDs.push_back(stoll(line));
        }
    }

    // Build balanced tree
    tree.buildFromIntervals(intervals);

    // Now we check which IDs are fresh
    int freshCount = 0;
    // For each ID, we check if it's contained in any interval
    for (long long q : IDs) {
        // If the tree contains the ID in any interval, we count it as fresh
        if (tree.intervalContains(q)) {
            freshCount++;
        }
    }

    // Now we output the result
    cout << "Fresh products: " << freshCount << "\n";

    return 0;
}