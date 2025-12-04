# Implementations of Advent of code 2025 - Problem 1

This folder contains different implementations for the first problem of Advent of Code 2025, which involves simulating a dial-like structure in order to find how many times a specific position is visited.

## Folder Structure
- `Linked_List`: Contains the implementation using a circular linked list to represent the dial.
- `No_Data_Structure`: Contains the implementation without using any advanced data structures.

```
AoC1
|
|--- Linked List
|       |--- AoC1_Linked_List.cpp
|       |--- AoC2_Linked_List.cpp
|--- No Data Structure
        |--- AoC1_No_Data_Structure.cpp
        |--- AoC2_No_Data_Structure.cpp
```

## No data structure implementation
- `No_Data_Structure`: This implementation uses basic variables and control structures without any advanced data structures.
- `AoC1_No_Data_Structure.cpp`: The main C++ file containing the implementation of the solution without using any data structures.

### AoC1_No_Data_Structure.cpp
This file contains the complete implementation of the solution without using any data structures. It reads input from a file, processes the movements on the dial, and counts the number of times the target position (0) is visited.

For readind the file we use:

```cpp
ifstream inputFile("input.txt");
```
To get the movements we use:
```cpp
while (getline(inputFile, line));
    char direction = line[0];
    int steps = stoi(line.substr(1));
```
The movements are processed using simple arithmetic to update the current position on the dial (if it is lesss than 0 or greater than the maximum position, it wraps around), and a counter is incremented each time the target position (0) is reached.

Finally, the result is printed to the console:
```cpp
cout << "Total visits to position 0: " << visitCount << endl;
```
### AoC2_No_Data_Structure.cpp
This file contains a similar implementation to `AoC1_No_Data_Structure.cpp`, but it counts each time the dial points to 0 instead of how many times it ends atr position 0 after each movement.

We implemented a change of logic in to count each time the dial points at the target position (0) during movements. This is done by checking how many full rotations the dial makes and finally calculating the remaining steps to see if it lands or crosses the target position (0).

This is achieved by using the following logic:
```cpp
    laps = steps / 100;
    zeroCount += laps;
    int remaining = steps % 100;
    if (remaining > 0 && remaining >= position && position > 0) zeroCount++;
    position = ((position '<+ for R, - for L>' steps) % 100 + 100) % 100;
```

## Linked List implementation
- `Linked_List`: This implementation uses a circular linked list to represent the physical dial.
- `AoC1_Linked_List.cpp`: The main C++ file containing the implementation of the solution using a circular linked list.

### AoC1_Linked_List.cpp
This file contains the complete implementation of the solution using a circular linked list. It defines a `Node` structure to represent each position on the dial and a `LinkedList` class to manage the circular linked list with a method to add nodes correctly.
The linked list is first initialized with positions from 0 to 99, and the starting position is set to 50. Then the movements are processed by traversing the linked list according to the specified directions and steps provided in `AoC1.txt`.

The structure Node is defined as follows:
```cpp
struct Node {
    int position;
    Node* next;
    Node(int pos) : position(pos), next(nullptr) {}
};
```
The LinkedList class is defined as follows:
```cpp
class LinkedList {
    public:
        Node* first;
        Node* last;
        Node* actual;
        LinkedList() {}
        ~LinkedList() {}
        void addNode(int pos) {}
};
```
The movements are processed by traversing the linked list according to the specified directions and steps, and a counter is incremented each time the target position (0) is reached by checking the `actual` node's position with:
```cpp
if (positionsList.actual->position == 0) {
    visitCount++;
}
```

Finally, the result is printed to the console:
```cpp
cout << "Total visits to position 0: " << visitCount << endl;
```
### AoC2_Linked_List.cpp
This file contains a similar implementation to `AoC1_Linked_List.cpp`, but it counts each time the dial points to the target position (0) during movements instead of how many times it ends at the target position after each movement.

We have added a `prev` pointer to the `Node` structure to facilitate backward traversal in the linked list. The `LinkedList` class has been updated accordingly to manage the `prev` pointers when adding nodes. The structure Node is now defined as follows:
```cpp
struct Node {
    int position;
    Node* next;
    Node* prev;
    Node(int pos) : position(pos), next(nullptr), prev(nullptr) {}
};
```
The movements are processed by traversing the linked list according to the specified directions and steps, we check if the `actual` node's position is 0 after each step and increment the counter accordingly.

The logic behind the list traversal and counting is as follows:
```cpp
    for (int step = 0; step < steps; step++) {
        if (direction == 'R') {
            temp = temp->next;
        } else if (direction == 'L') {
            temp = temp->prev;
        }
        if (temp->position == 0) {
            zeroCount++;
        }
    }
```
As you can see, each time we are at position 0 during the traversal, we increment the `zeroCount` counter.

Finally, the result is printed to the console:
```cpp
cout << "Total visits to position 0: " << zeroCount << endl;
``` 


## Final Notes

This problem gave us freedom to use different data structures or none at all. We decided to implement a non data structure first in order to show the ingreasing complexity on the `Advent of Code` problems. Then, we decided to spice things up a bit and implement a circular linked list to represent the physical dial, wich added some complexity to the solution but also made it more interesting from a data structures perspective.

## Authors
- Enric Talens (Group Leader, Implementer and Documentation Writer)
- Josué Félix (Code Reviewer, Implementer and Tester)
- Diego Jimenez (Implementation Reviewer and Challenge Advisor)
- Sergio Real (Information Researcher and Documentation Reviewer)

