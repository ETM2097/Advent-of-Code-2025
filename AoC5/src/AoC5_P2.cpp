// This will be the implementation for the 5th day's challenge of Advent of Code.
// Today consists of processing a list of boarding passes to determine fresh product IDs 
// and checking from a list which IDs are fresh.

// The code is commented, but for a better explanation, please, read the Readme file.

#include "../../INCLUDE/Tree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Function to count unique IDs from a list of intervals
long long countUniqueIDs(std::vector<Interval<long long>>& intervals) {
    if (intervals.empty()) return 0;

    // 1. Sort intervals by start value (same as in buildBalanced)
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval<long long>& a, const Interval<long long>& b) {
            return a.start < b.start;
        });

    // 2. Merge overlapping intervals, for not double counting
    std::vector<Interval<long long>> merge;
    merge.push_back(intervals[0]);

    // Here we iterate though sorted intervals and merge them
    for (size_t i = 1; i < intervals.size(); i++) {
        Interval<long long>& last = merge.back();
        const Interval<long long>& current = intervals[i];

        // Check if current overlaps or is adjacent to last
        if (current.start <= last.end + 1) {
            // Merge: extend the end if needed
            last.end = std::max(last.end, current.end);
        } else {
            // No overlap: add as new interval
            merge.push_back(current);
        }
    }

    // 3. Here we count total unique IDs
    long long total = 0;
    for (const Interval<long long>& interval : merge) {
        total += (interval.end - interval.start + 1);
    }

    return total;
}

// Example usage for AoC5
int main() {
    
    // Vector for storing intervals
    std::vector<Interval<long long>> intervals;

    // Vector for storing single ID requests
    std::vector<long long> IDs;

    // Here we get the input file
    std::ifstream file("texts/AoC5.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file.\n";
        return 1;
    }
    std::string line;

    // Parse input
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t endFirstId = line.find('-');
        // We check if it's a range or a single number
        if  (endFirstId != std::string::npos && endFirstId > 0) {
            long long start = std::stoll(line.substr(0, endFirstId));
            long long end = std::stoll(line.substr (endFirstId + 1));
            intervals.push_back({start, end});
        } else {
            // Single number query
         IDs.push_back(std::stoll(line));
        }
    }

    // Now we check the total IDs that are fresh
    long long freshCount = countUniqueIDs(intervals);

    std::cout << "Total fresh IDs: " << freshCount << "\n";

    return 0;
}