// This will be the code of the first part of the second day of Advent of Code 2025
// Today consist in to see if there is a valid ID number according to some rules
// 1st rule: there is a range: FirstID-LastID
// 2nd rule: Each range is separated by ","
// 3rd rule: If any ID has a repetitive sequence of numbers, it is invalid
// 4th rule: No ID starts with a 0
// 5th rule: We must add up all invalid IDs

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Simple approach: just cut in half and compare
bool hasRepetitiveSequence(const string& id) {
    int n = id.length();
    
    // Must have even length to be split in half
    if (n < 2 || n % 2 != 0) return false;
    
    int half = n / 2;
    
    // Check if first half equals second half
    return id.substr(0, half) == id.substr(half);
}

int main() {
    ifstream inputFile("AoC2.txt");
    string line;
    long long invalidSum = 0;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string range;
        
        // Split by ','
        while (getline(ss, range, ',')) {
            // We split by '-' by finding its position
            size_t linePos = range.find('-');

            // We get the two IDs by extracting the substrings
            long long firstID = stoll(range.substr(0, linePos));
            long long lastID = stoll(range.substr(linePos + 1));

            // Iterate through all IDs in the range
            for (long long id = firstID; id <= lastID; id++) {
                string idStr = to_string(id);
                
                // Check if it has repetitive sequence (no need to check for leading 0)
                if (hasRepetitiveSequence(idStr)) {
                    invalidSum += id;
                }
            }
        }
    }

    cout << "Sum of invalid IDs: " << invalidSum << endl;
    return 0;
}