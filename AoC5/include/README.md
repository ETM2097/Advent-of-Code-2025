# Implementation of Tree for Advent of Code 2025: Day 5
This document describes the implementation details of the `HashMap` and `Graph`. Both classes are templated to allow for flexibility in key and value types. The implementations build upon concepts learned in previous days, with specific adaptations to meet the requirements of Day 11, as it was the last day we worked on.

We tried to make our code as reusable as possible. While this made the classes a bit larger than expected, we believe it was worth it. Think of this README as a complete user guide: we’ll cover the main features and explain why we built things the way we did. Grab a coffee—this is going to be a long read!

# Index

As the document is quite long, here is an index to help you navigate through the different sections so you can check easily the parts you are more interested in:
- [Tree Implementation](#tree-implementation)
    - [Key Features](#tree-features)
      - [Tree Template Parameters](#tree-template-parameters)
    - [Tree Class Members](#tree-class-members)
    - [Methods](#tree-methods)

## Tree Implementation
The `Tree` class is a template tree in which we wanted the code to be reusable for future challenges, so we tried to implement a class that was ready for both binary trees of simple elements and intervals. 

It could have been made even more generic by adding other types of trees, but we decided that this was beyond the scope of this challenge. In any case, it is on the list of future updates, as there is still much room for improvement.

<a id="tree-features"></a>

### Key Features

- **Templated Design**: The `Tree` class is template-based to allow any type of data as nodes, whether they are simple elements or ranges.
- **Node Structure**: Each node can store a single value or a range, using `std::variant` to handle both data types.
- **Basic Operations**: The class supports `insertion`, `deletion`, `printing` (inorder, postorder, preorder), `searching`, among others.  
- **Rebalancing and Construction**: We add rebalancing based on the inorder and balanced construction from value or interval vectors (sort, remove duplicates, and rebuild).
- **Error Handling**: We implement error handling to prevent the insertion of duplicate elements.

Now, we will begin to explain all the logic behind the new `Tree` class, but first we will explain two important details: the node class and the interval structure.

### Interval
This structure represents a closed interval defined for two values `start` and `end` of the same type `T`. We also implemented two operators that compare two intervals, one comparing only the start (`operator<`) and the other comparing the start and end of both intervals (`operator==`).

```cpp
template <typename T>
struct Interval {
    T start;
    T end;
    
    Interval() : start(T()), end(T()) {}
    Interval(T s, T e) : start(s), end(e) {}
    
    bool operator<(const Interval& other) const {
        return start < other.start;
    }
    
    bool operator==(const Interval& other) const {
        return start == other.start && end == other.end;
    }
};
```


### Node
This class represents a tree node capable of storing a single value or an interval thanks to `std::variant`. 

In detail: 
- **data**: that stores the value or interval.
- **left / right**: pointers to the left and right children.
- **maxRange**: the maximum value of end in the subtree.
```cpp
    std::variant<T, Interval<T>> data;
    Node* left;
    Node* right;
    T maxRange;  
```
#### Constructors:
We also added three constructors because we needed it to be able to construct from a simple element, an interval, and another that supports intervals.
```cpp
//Constructor for simple elements
Node(T elem, Node* left = nullptr, Node* right = nullptr) : 
    data(elem), left(left), right(right), maxRange(elem) {}

//Constructor for intervals
Node(T start, T end, Node* left = nullptr, Node* right = nullptr) :
    data(Interval<T>(start, end)), left(left), right(right), maxRange(end) {}

//Constructor with Interval
Node(const Interval<T>& interval, Node* left = nullptr, Node* right = nullptr) :
    data(interval), left(left), right(right), maxRange(interval.end) {}
    
```

#### Helpers: 
- **isInterval**: indicates whether data contains an interval.
- **getStart**: return start whether it is a simple value or an interval.
- **getEnd**: return end whether it is a simple value or an interval.
- **getRange**: returns the entire interval; if the node has a simple value, it returns [val, val].

```cpp
//Helper methods to check and access data
bool isInterval() const {
    return std::holds_alternative<Interval<T>>(data);
}

//Get start value (for both T and Interval)
T getStart() const {
    if (isInterval()) {
        return std::get<Interval<T>>(data).start;
    } else {
        return std::get<T>(data);
    }
}

//Get end value (for both T and Interval)
T getEnd() const {
    if (isInterval()) {
        return std::get<Interval<T>>(data).end;
    } else {
        return std::get<T>(data);
    }
}

//Get the interval (if it's a single value, return [val, val])
Interval<T> getRange() const {
    if (isInterval()) {
        return std::get<Interval<T>>(data);
    } else {
        T val = std::get<T>(data);
        return Interval<T>(val, val);
    }
}
```

The `node class` and the `Interval struct` are necessary for the `Tree class` to function. This is how we will now begin to describe the `Tree class`.

### Tree Template Parameters
The `Tree` class is templated with a single parameter:
- `T`: The type of data stored in the tree nodes or interval boundaries (e.g., `int`, `long long`, `double`).

### Tree Class Members
The `Tree` class contains two **private members**:
- `nelem`: This is the current number of stored nodes.
- `root`: This is a pointer to the root node of the tree.

<a id="tree-methods"></a>

### Methods
In this section, we will describe the methods we implement to manage and manipulate the tree.

### Constructor
The `Tree` class has a default constructor that initializes an empty tree:
```cpp
Tree() {
    this->nelem = 0;
    this->root = nullptr;
}
```

#### Insertion
We implemented two insertion methods, one for simple elements and another for ranges, and a method for updating the maximum range.

```cpp
//Unified insert for single elements
Node<T>* insert(Node<T>* n, T e){
    if (n == nullptr) {
        return new Node<T>(e);
    } else if(n->getStart() == e){
        throw runtime_error("Duplicate element");
    } else if(n->getStart() < e){
        n->right = insert(n->right, e);
    } else {
        n->left = insert(n->left, e);
    }
    updateMaxRange(n);
    return n;
}

//Unified insert for intervals (overload)
Node<T>* insert(Node<T>* n, const Interval<T>& interval) {
    // Base case: If the tree is empty, return a new node
    if (n == nullptr) {
        return new Node<T>(interval);
    }
    // Otherwise, recur down the tree
    if (interval.start < n->getStart()) {
        n->left = insert(n->left, interval);
    } else {
        n->right = insert(n->right, interval);
    }
    // Here we update the max value of the ancestor node
    // Here we had a critical bug before, we corrected it by ensuring we consider both left and right children
    updateMaxRange(n);
    // Finally return the node pointer
    return n;
}

    // Here we update the max value of the ancestor node
// Here we had a critical bug before, we corrected it by ensuring we consider both left and right children
void updateMaxRange(Node<T>* n) const {
    if (n == nullptr) return;
    n->maxRange = n->getEnd();
    if (n->left) n->maxRange = std::max(n->maxRange, n->left->maxRange);
    if (n->right) n->maxRange = std::max(n->maxRange, n->right->maxRange);
}
```

#### Balanced build
We needed to implement functions that would build balanced trees from sorted vectors, guaranteeing a minimum height. 
We used the divide and conquer approach: selecting the central element as the root and recursively building the left and right subtrees. That is why we created two balancing functions, one for simple elements(`buildBalanced`) and the other specialized for intervals(`buildBalancedFromIntervals`). We also added a function that checks that the tree is balanced(`isBalanced`) and two collectors to store the elements of the tree and thus balance them(`collectInorder and collectIntervalsInorder`). Finally, we added a public method to balance the tree(`balance`).

```cpp
private:
//Build balanced tree from sorted elements
Node<T>* buildBalanced(std::vector<T>& elements, int start, int end) {
    if (start > end) return nullptr;

    int mid = start + (end - start) / 2;
    Node<T>* node = new Node<T>(elements[mid]);

    node->left = buildBalanced(elements, start, mid - 1);
    node->right = buildBalanced(elements, mid + 1, end);

    updateMaxRange(node);
    return node;
}

//Build balanced tree from intervals
Node<T>* buildBalancedFromIntervals(std::vector<Interval<T>>& intervals, int start, int end) {
    if (start > end) return nullptr;

    int mid = start + (end - start) / 2;
    Node<T>* node = new Node<T>(intervals[mid]);

    node->left = buildBalancedFromIntervals(intervals, start, mid - 1);
    node->right = buildBalancedFromIntervals(intervals, mid + 1, end);

    updateMaxRange(node);
    return node;
}

//Collect elements inorder
void collectInorder(Node<T>* n, std::vector<T>& elements) const {
    if (n != nullptr) {
        collectInorder(n->left, elements);
        elements.push_back(n->getStart());
        collectInorder(n->right, elements);
    }
}

//Collect intervals inorder
void collectIntervalsInorder(Node<T>* n, std::vector<Interval<T>>& intervals) const {
    if (n != nullptr) {
        collectIntervalsInorder(n->left, intervals);
        if (n->isInterval()) {
            intervals.push_back(n->getRange());
        }
        collectIntervalsInorder(n->right, intervals);
    }
}

//Check if the tree is balanced
bool isBalanced(Node<T>* n, int& height) const {
    if (n == nullptr) {
        height = 0;
        return true;
    }

    int leftHeight = 0, rightHeight = 0;
    bool leftBalanced = isBalanced(n->left, leftHeight);
    bool rightBalanced = isBalanced(n->right, rightHeight);

    height = 1 + std::max(leftHeight, rightHeight);

    if (abs(leftHeight - rightHeight) <= 1 && leftBalanced && rightBalanced) {
        return true;
    }

    return false;
}

public: 

//Balance the tree
void balance() {
    if (empty()) return;

    std::vector<T> elements;
    collectInorder(this->root, elements);

    deleteAll(root);
    root = buildBalanced(elements, 0, elements.size() - 1);
}

//Build balanced tree from a vector of elements
void buildFromVector(std::vector<T>& elements) {
    if (elements.empty()) return;

    std::sort(elements.begin(), elements.end());
    
    elements.erase(std::unique(elements.begin(), elements.end()), elements.end());
    
    deleteAll(root);
    root = buildBalanced(elements, 0, elements.size() - 1);
    nelem = elements.size();
}

//Build balanced tree from a vector of intervals
void buildFromIntervals(std::vector<Interval<T>>& intervals) {
    if (intervals.empty()) return;

    //Sort by interval start
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval<T>& a, const Interval<T>& b) {
            return a.start < b.start;
        });

    deleteAll(root);
    root = buildBalancedFromIntervals(intervals, 0, intervals.size() - 1);
    nelem = intervals.size();
}

//Check if the tree is balanced
bool isBalanced() const {
    int h = 0;
    return isBalanced(this->root, h);
}
```

#### Others
We added other methods such as finding the maximum (`max`), the minimum (`min`), the height of the tree (`height`), the size of the tree(`size`), and whether the tree is empty(`empty`).

```cpp
//Find maximum element in the tree
T max(Node<T>* n) const{
    if(n == nullptr){
        throw runtime_error("Empty tree");
    } else if (n->right != nullptr){
        return max(n->right);
    } else {
        return n->getStart();
    }
}

//Find minimum element in the tree
T min(Node<T>* n) const{
    if(n == nullptr){
        throw runtime_error("Empty tree");
    } else if (n->left != nullptr){
        return min(n->left);
    } else {
        return n->getStart();
    }
}

//Calculate height of the tree
int height(Node<T>* n) const {
    if (n == nullptr) {
        return 0;
    }
    int leftHeight = height(n->left);
    int rightHeight = height(n->right);
    return 1 + std::max(leftHeight, rightHeight);
}

//Get number of elements
int size() const {
    return this->nelem;
}

//Check if tree is empty
bool empty() const {
    return this->nelem == 0;
}
```
