// This is the second part of the AoC Day 3 2025 challenge
// Now we must add up 12 digit numbers formed by picking digits from the string
// Those digits do not have to be next to each other but the final number will be formed left to right

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    string input;
    ifstream file("AoC3.txt");
    long long sum_total = 0;

    while (getline(file, input)) {
        string result;
        size_t start = 0;

        // Greedily pick 12 digits, more explanation in the Readme file
        for (int i = 0; i < 12; ++i) {
            // We need to leave enough digits for the remaining picks
            size_t end = input.length() - (12 - i - 1);
            auto max_it = max_element(input.begin() + start, input.begin() + end);
            result += *max_it;
            start = distance(input.begin(), max_it) + 1;
        }

        sum_total += stoll(result);
    }

    cout << "The sum of the largest 12-digit numbers is: " << sum_total << endl;
    return 0;
}