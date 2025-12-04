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
#include <set>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// We have investigated and decided to implement a KMP Failure finction even if it does not fit the requirements for
// making this problem to be elected for calification, as any of the data structures nor divide and conquer give any improvement
// to this problem.

// Here we implement the failure table
vector<int> buildFailure(const string& s) {
    // We get the length of the string
    int n = s.length();
    // We create a vector initialized to 0 and size n
    vector<int> failure(n, 0);
    int j = 0;
    
    // Here we use the for loop to build the table
    for (int i = 1; i < n; i++) {
        // Fall back until we find a match or reach position 0
        // This is further explained in the README file.
        while (j > 0 && s[i] != s[j]) {
            j = failure[j - 1];
        }
        // If characters match, extend the prefix, this means we found a longer prefix which is also a suffix
        if (s[i] == s[j]) {
            j++;
        }
        // Set the failure function for position i to j
        failure[i] = j;
    }
    return failure;
}

// Here we check if there is a repetitive sequence using the failure table
bool hasRepetitiveSequence(const string& id) {
    int n = id.length();
    if (n < 2) return false;
    
    vector<int> failure = buildFailure(id);
    
    int patternLength = n - failure[n - 1];
    
    // Must divide evenly AND repeat at least twice
    return (n % patternLength == 0) && (n / patternLength >= 2);
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
            
            // We get the two IDs by extracting the substrings and converting to long long for being able to handle large numbers
            long long firstID = stoll(range.substr(0, linePos));
            long long lastID = stoll(range.substr(linePos + 1));

            // Iterate through all IDs in the range
            for (long long id = firstID; id <= lastID; id++) {
                string idStr = to_string(id);
                
                // Check if it starts with '0' or has repetitive sequence
                if (idStr[0] == '0' || hasRepetitiveSequence(idStr)) {
                    invalidSum += id;
                }
            }
        }
    }

    cout << "Sum of invalid IDs: " << invalidSum << endl;
    return 0;
}