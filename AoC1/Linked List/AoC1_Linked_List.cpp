// Here we implement the same logic as in AoC1.cpp but using a linked list to store different positions of the dial.

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// We create a simple linked list node structure to hold the position of the dial.
struct Node {
    int position;
    Node* next;
    Node(int pos) : position(pos), next(nullptr) {}
};

class LinkedList {
    public:
        Node* first;
        Node* actual;
        Node* last;
        // Contructor and destructor to manage memory
        LinkedList() : first(nullptr), actual(nullptr) {}
        ~LinkedList() {
            if (first == nullptr) return;
            // We break the circular link
            if (last != nullptr) {
                last->next = nullptr;
            }
            Node* current = first;
            while (current != nullptr) {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
        // Function to add a new position to the linked list
        void addPosition(int pos) {
            Node* newNode = new Node(pos);
            if (first == nullptr) {
                first = newNode;
                actual = first;
                last = first;
            } else {
                last->next = newNode;
            }
            last = newNode;
            // We make the list circular
            last->next = first;
        }
};

int main() {
    ifstream inputFile("../AoC1.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string line;
    int position = 50;
    int zeroCount = 0;
    // Here we create an instance of the linked list to store positions
    LinkedList positionsList;
    // Here we initialize the linked list for having all numbers from 0 to 99
    for (int i = 0; i < 100; i++){
        positionsList.addPosition(i);
    }

    // Here we go to the starting position
    positionsList.actual = positionsList.first;
    for (int i = 0; i < 50; i++) {
        if (positionsList.actual != nullptr) {
            positionsList.actual = positionsList.actual->next;
        }
    }

    while (getline(inputFile, line)) {
        // Here we use the same logic as in AoC1.cpp to move the dial
        char direction = line[0];
        int steps = stoi(line.substr(1));
        
        // Here we initialize a temporary node to traverse the linked list and the variable to_go to know the target position
        int to_go;
        Node* temp = positionsList.actual;

        // If we go left we subtract steps, if right we add them
        if (direction == 'L') {
            to_go = ((positionsList.actual->position - steps) % 100 + 100) % 100;

        } else if (direction == 'R') {
            to_go = (positionsList.actual->position + steps) % 100;
        }
        // Here we traverse the linked list to find the target position
        int steps_taken = 0;
        while (temp->position != to_go && steps_taken < 100) {
            temp = temp->next;
            steps_taken++;
        }
        if (steps_taken == 100) {
            cerr << "Error: Position not found in the list." << endl;
            return 1;
        }
        // We update the actual position in the linked list
        positionsList.actual = temp;
        // Here we check if we are at position 0 and increment the counter.
        if (positionsList.actual->position == 0) {
            zeroCount++;
        }
    }
    inputFile.close();

    cout << "Total times at position 0: " << zeroCount << endl;

    return 0;
}