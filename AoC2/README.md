# Implementation of Advent of Code 2025 - Day 2

This code implements the solution for part 1 and 2 of Day 2 of Advent of Code 2025. We decided to use a Knuth-Morris-Pratt (KMP) algorithm approach to efficiently find repetitive sequences in strings.

## File: AoC2_P1.cpp

This file contains the implementation of the solution for part 1 of Day 2 of Advent of Code 2025. The task involves processing a list of strings and counting how many of them contain a specific repetitive sequence.

As the condition for part 1 is just to see if a string contains a repeated sequence repeated TWICE, we can use a simplified approach. The function `hasRepetitiveSequence` checks if a string has any substring that appears at least twice.
We jsut check if the first half of the string contains the same substring as the second half.

### hasRepetitiveSequence Function
```cpp
bool hasRepetitiveSequence(const string& id) {
    int n = id.length();
    
    if (n < 2 || n % 2 != 0) return false;
    
    int half = n / 2;
    
    return id.substr(0, half) == id.substr(half);
}
```
This returns true if the first half of the string matches the second half, indicating a repetitive sequence. If it is true, we add up to our count of valid strings.

## File: AoC2_P2.cpp

This file contains the implementation of the failure table construction and the function to check for repetitive sequences in a given string.

A KMP failure table is built to help identify the longest prefix which is also a suffix for substrings of the input string. This is useful for detecting repetitive patterns.
It works as follows:

1. **buildFailure**: This function constructs the failure table for a given string. It iterates through the string and uses previously computed values to efficiently build the table.

Here is a graphical representation of how the failure table is built:

```
Index:        0 1 2 3 4 5 
String:       a b a b a b 
Failure:      0 0 1 2 3 4 
```

At each index, the failure table indicates the length of the longest proper prefix which is also a suffix for the substring ending at that index.

- `failure[2] = 1`: For "aba", the prefix "a" matches the suffix "a"
- `failure[3] = 2`: For "abab", the prefix "ab" matches the suffix "ab"
- `failure[4] = 3`: For "ababa", the prefix "aba" matches the suffix "aba"
- `failure[5] = 4`: For "ababab", the prefix "abab" matches the suffix "abab"

Now that whe know what a failure table is, we are going to discuss the code:
```cpp
vector<int> buildFailure(const string& s) {
    int n = s.length();
    vector<int> failure(n, 0);
    int j = 0; // Length of the previous longest prefix-suffix

    // Here we use the for loop to build the table
    for (int i = 1; i < n; i++) {
        // Fall back until we find a match or reach position 0
        while (j > 0 && s[i] != s[j]) {
            j = failure[j - 1];
        }
        // If there's a match, extend the current prefix-suffix length
        if (s[i] == s[j]) {
            j++;
        }
        failure[i] = j; // Set the failure value for position i for the current position of the string
    }
    return failure; // We return the built failure table.
}
```

2. **hasRepetitiveSequence**: This function uses the failure table to determine if there is a repetitive sequence in the string. If the last value in the failure table indicates that a prefix matches a suffix, it confirms the presence of a repetitive sequence.

This is calculated as follows:

```
Let n be the length of the string.
Let l be the value of failure[n-1].
If l > 0 and n % (n - l) == 0, then the string has a repetitive sequence.
``` 
Aplying this to the string "ababab":
```
n = 6
l = failure[5] = 4
n - l = 6 - 4 = 2
n % (n - l) = 6 % 2 = 0
```
Thus, "ababab" has a repetitive sequence "ab".

This functtion return true if the string contains a repetitive sequence repeated at least twice. The code is as follows:

```cpp
bool hasRepetitiveSequence(const string& id) {
    int n = id.length();
    if (n < 2) return false; // Strings shorter than 2 cannot have repetitive sequences.
    
    vector<int> failure = buildFailure(id); // Here the failure table is built.
    
    int repetitiveLength = n - failure[n - 1]; // We get the length of the repetitive pattern.
    
    return (n % repetitiveLength == 0) && (n / repetitiveLength >= 2); // Check if the pattern repeats at least twice.
}
```

## Final Notes

Today's `Advent of Code` problem turned out to be `less interesting` that the one from yesteday, `but` the second part was a nice challenge that `made us investigate the KMP algorithm and its failure table construction`. We decided to not implement any data structures because the problem could be solved with simple string manipulations and the KMP algorithm. That's because the first part was very straightforward and didn't require complex data structures. And the second part could be efficiently solved using the KMP algorithm without needing additional data structures (that would only add unnecessary complexity).

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)