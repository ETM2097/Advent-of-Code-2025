// Here we are going to implement the second part of the Advent of Code Day 1 challenge
// Now we must count any click that poits to position 0, not only the final one.
// For example: 
//              We are at position 2 and we recieve L4, we point once at position 0, so we must count it.
//              We are at position 4 and revice R6, we do not point at position 0, so we do not count it.

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
    int contador = 0;
    while (getline(inputFile, line)) {
        // Whe get the first character to determine direction
        char direction = line[0];
        // Here we get the substing counting from index 1 to the end and convert it to integer
        int steps = stoi(line.substr(1));
        int laps = 0;
        // Here we implent the logic to move the dial
        // Now whe have to know when de dial is pointing to the number 0
        // We must know if the number of steps will make the dial point to 0 or not
        if (direction == 'L') {
            // We first know how many full rounds the dial will make:
            laps = steps / 100;
            // Add them to the counter
            zeroCount += laps;
            // Now we calculate the remainder of steps to take
            int remaining = steps % 100;
            // If it is greater than 0, and greater the actual position and we are not in 0, we add one.
            if (remaining > 0 && remaining >= position && position > 0) zeroCount++;
            // We calculate the absolute position.
            position = ((position - steps) % 100 + 100) % 100;
        } 
        else {
            laps = steps / 100;
            zeroCount += laps;
            int remaining = steps % 100;
            if (remaining > 0 && position + remaining >= 100) zeroCount++;
            position = (position + steps) % 100;
        }
        contador ++;
    }

    // We close the file after reading all lines
    inputFile.close();

    // Here we show the result.
    cout << "Total times at position 0: " << zeroCount << endl;

    return 0;
}