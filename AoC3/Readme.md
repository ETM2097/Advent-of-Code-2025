# Implementation of Advent of Code 2025, Day 3

Todays challenge is to find the highest voltage that can be obtained from a battery pack following some rules.
The battery pack is represented as a series of digits, each digit representing a battery cell with a certain voltage.

For the first part of the challenge, we need to find the two highest digits in each line of the input file and sum them up. And for the second, we need to find the 12 highest digits and sum them up.

## File: AoC3_P1.cpp

This is the implementation for Part 1 of the challenge. It reads the input file line by line, finds the two highest digits in each line, and sums them up.

For acomplihing this goal we decided to use a greedy algorithm approach.

Firs, we get the highest number to the right of each position in the line and store it into a `char` array called `rightMax`.
```cpp
int n = line.length(); // We get the length of the line
char rightMax[n]; // We create an array to store the right maximums
rightMax[n - 1] = line[n - 1]; // As there are no elements to the right of the last element, we set it to itself
for (int i = n - 2; i >= 0; i--) { // We iterate from the second last element to the first
    rightMax[i] = std::max(line[i], rightMax[i + 1]); // We set the right maximum to the maximum of the current element and the right maximum of the next element
}
```
Explanation of the code above:
```
line = [9, 1, 9, 4, 6] 
1. We set rightMax[4] = 6 as there are no more elements to the right of it. So rightMax = [_, _, _, _, 6]
2. We set rightMax[3] to the max between line[3] (4) and rightMax[4] (6) -> rightMax[3] = 6. So rightMax = [_, _, _, 6, 6]
3. We set rightMax[2] to the max between line[2] (9) and rightMax[3] (6) -> rightMax[2] = 9. So rightMax = [_, _, 9, 6, 6]
4. We set rightMax[1] to the max between line[1] (1) and rightMax[2] (9) -> rightMax[1] = 9. So rightMax = [_, 9, 9, 6, 6]
5. We set rightMax[0] to the max between line[0] (9) and rightMax[1] (9) -> rightMax[0] = 9. So rightMax = [9, 9, 9, 6, 6]
```

Then we just check for each position if (line[i] * 10 + rightMax[i + 1]) is greater than the current maximum voltage found. If it is, we update the maximum voltage.
```cpp
int max_number = -1; // We initialize the maximum number to -1
for (int i = 0; i < n - 1; i++) { //
    int number = (line[i] - '0') * 10 + (rightMax[i + 1] - '0'); // We calculate the number formed by line[i] and rightMax[i + 1]
    if (number > max_number) { // If the number is greater than the current maximum
        max_number = number; // We update the maximum number
    }
}
```

Finally, we sum up the maximum numbers found for each line and print the result.

## File: AoC3_P2.cpp

The part 2 of the challenge consists in finding the 12 highest digits in each line and summing them up. For this we use a similar approach as in part 1, but instead of storing only the right maximum, we store the 12 highest digits to the right of each position.

We read the input file line by line. For each line, we greedily pick the highest digit possible while ensuring that we can still pick enough digits to reach a total of 12.

```cpp
for (int i = 0; i < 12; ++i) { // We iterate 12 times to pick 12 digits
    // We need to leave enough digits for the remaining picks
    size_t end = input.length() - (12 - i - 1); // We set the end position to ensure enough digits remain
    auto max_it = max_element(input.begin() + start, input.begin() + end); // We find the maximum element in the range [start, end)
    result += *max_it; // We add the maximum digit to the result
    start = distance(input.begin(), max_it) + 1; // We update the start position to the next position after the picked digit
}
```
Explanation of the code above:
```
line = [9, 1, 9, 4, 6, 2, 8, 7, 5, 3, 0, 9, 4, 5, 1, 1]
1. In the first iteration (i=0), we can pick from the position 0 to position 3. The maximum is 9 at position 0. We pick it and set start to 1.
- result = [9]
2. In the second iteration (i=1), we can pick from position 1 to position 4. The maximum is 9 at position 2. We pick it and set start to 3.
- result = [9, 9]
3. In the third iteration (i=2), we can pick from position 3 to position 5. The maximum is 6 at position 4. We pick it and set start to 5.
- result = [9, 9, 6]
4. In the fourth iteration (i=3), we can pick from position 5 to position 6. The maximum is 8 at position 6. We pick it and set start to 6.
- result = [9, 9, 6, 8]
5. We continue this process until we have picked 12 digits (we add the rest).

+ Extreme example: eX = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 1, 9]
1. In the first iteration (i=0), we can pick from position 0 to position 5. The maximum is 1 at position 0. We pick it and set start to 1.
- result = [1]
2. In the second iteration (i=1), we can pick from position 1 to position 6. The maximum is 1 at position 2. We pick it and set start to 2.
- result = [1, 1]
...
...
3. In the third iteration (i=5), we can pick from position 6 to position 11. The maximum is 9 at position 11. We pick it and set start to 12.
- result = [1, 1, 1, 1, 1, 1, 9]
4. We continue and check that we still get a 12 digit number at the end because of the iterations limit.
```

Finally, we sum up the 12-digit numbers found for each line and print the result.

## Final Notes

Today's problem did not give us a nice pattern for using data structures, nor dynamic programing or divide and conquer techniques. However, we were able to solve it using greedy algorithms that we have not seen in class, which are often efficient and for optimization problems like this one.
So in the end, we learned a new algorithmic technique that can be useful in future problems.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)
