// This will be the code for AoC Day 3 2025 challenge
// This challenge consists into finding the larger 2 digit number in a string of numbers
// Those two numbers do not have to be next to each other

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// For this approach we will use a greedy algorithm, this will allow us to find the solution in linear time, more explained in the Readme file

int main() {
    std::string input;
    ifstream file("AoC3.txt");
    int sum_total = 0;
    while(getline(file, input)){
        int n = input.length();
        char maxRight[n];
        // Here we set up an array to store the maximum digit to the right of each position

        // There is no need to check the last position since there is no digit to the right
        maxRight[n-1] = input[n-1];

        // We fill the array from right to left
        for (int i = n-2; i >= 0; --i) {
            
            // We fill the array with the maximum digit found to the right
            maxRight[i] = max(input[i+1], maxRight[i+1]);
        }
        int max_number = -1;
        // Now we iterate through the string to find the maximum 2 digit number
        for (int i = 0; i < n-1; ++i) {
            int number = (input[i] - '0') * 10 + (maxRight[i] - '0');
            if (number > max_number) {
                max_number = number;
            }
        }
        sum_total += max_number;
        
    }
    std::cout << "The largest 2-digit number is: " << sum_total << std::endl;

    return 0;
}