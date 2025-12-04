// This is the code for Advent of Code Day 1 challenge
// It reads from a file a secuence of orers and sums the total of times it gets to position 0.
// The dial starts at position 50, and goes from 0 to 99, wrapping around.
// Each line of the input file contains L (left) or R (right) followed by a number of steps.

// This code will be done without using data structures, there will be another file using them.

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream inputFile("../AoC1.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string line;
    int position = 50;
    int zeroCount = 0;

    while (getline(inputFile, line)) {
        // Whe get the first character to determine direction
        char direction = line[0];
        // Here we get the substing counting from index 1 to the end and convert it to integer
        int steps = stoi(line.substr(1));

        // Here we implent the logic to move the dial
        if (direction == 'L') {
            // We use %100 to wrap around the dial as it goes from 0 to 99
            position = (position - steps + 100) % 100;
        } else if (direction == 'R') {
            position = (position + steps) % 100;
        }
        // Here we check if we are at position 0 and increment the counter.
        if (position == 0) {
            zeroCount++;
        }
    }

    // We close the file after reading all lines
    inputFile.close();

    // Here we show the result.
    cout << "Total times at position 0: " << zeroCount << endl;

    return 0;
}