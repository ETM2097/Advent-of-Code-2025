# Implementation of Tree for Advent of Code 2025: Day 5
This document describes the implementation details of the `Tree` class, which is templated to allow for flexibility in key and value types. The implementations build upon concepts learned in practice of PRA, with specific adaptations to meet the requirements of Day 5.

We chose to make this a whole new implementation instead of using existing libraries to ensure that the code is reusable for future challenges. The `Tree` class is designed to handle both simple elements and intervals, making it versatile for various use cases.

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

#### Search Operations
We implemented several search methods for different use cases:

##### searchRange()
Searches if an element exists within a specified range [start, end]:

```cpp
//Search if an element exists in a range [start, end]
bool searchRange(Node<T>* n, T start, T end) const {
    // Base case, node is null
    if (n == nullptr) return false;
    
    //If the current point is in the range, found
    T nodeVal = n->getStart();
    if (nodeVal >= start && nodeVal <= end) {
        return true;
    }
    
    //If the maximum of the left subtree is greater or equal to the range start
    if (n->left != nullptr && n->left->maxRange >= start) {
        if (searchRange(n->left, start, end)) {
            return true;
        }
    }
    
    //If the current element is less than the range end, search right
    if (nodeVal < end) {
        return searchRange(n->right, start, end);
    }
    
    return false;
}
```

##### searchIntervalContains()
Searches if a point falls within any stored interval:

```cpp
//Search if a point falls within any interval (for IntervalTree)
bool searchIntervalContains(Node<T>* n, T point) const {
    if (n == nullptr) return false;
    
    //Check if the current interval contains the point
    if (n->isInterval()) {
        Interval<T> range = n->getRange();
        if (range.start <= point && point <= range.end) {
            return true;
        }
    }
    
    //If the left child exists and its maxRange >= point, search there
    if (n->left != nullptr && n->left->maxRange >= point) {
        if (searchIntervalContains(n->left, point)) {
            return true;
        }
    }
    
    //Search in the right child
    return searchIntervalContains(n->right, point);
}
```

##### overlaps()
Checks if two intervals overlap:

```cpp
//Check if two intervals overlap
bool overlaps(const Interval<T>& a, const Interval<T>& b) const {
    return (a.start <= b.end && b.start <= a.end);
}
```

##### searchOverlappingInterval()
Searches for intervals that overlap with a given interval:

```cpp
//Search for intervals that overlap with the given one
bool searchOverlappingInterval(Node<T>* n, T start, T end) const {
    if (n == nullptr) return false;
    
    //Check if the current interval overlaps
    if (n->isInterval() && overlaps(n->getRange(), Interval<T>(start, end))) {
        return true;
    }
    
    //If the left child exists and its maxRange >= start, search there
    if (n->left != nullptr && n->left->maxRange >= start) {
        if (searchOverlappingInterval(n->left, start, end)) {
            return true;
        }
    }
    
    //Search in the right child if the current node start < end
    if (n->getStart() < end) {
        return searchOverlappingInterval(n->right, start, end);
    }
    
    return false;
}
```
#### Balanced Build
We needed to implement functions that would build balanced trees from sorted vectors, guaranteeing a minimum height. 

We used the **divide and conquer** approach: selecting the central element as the root and recursively building the left and right subtrees. That is why we created two balancing functions, one for simple elements(`buildBalanced`) and the other specialized for intervals(`buildBalancedFromIntervals`). We also added a function that checks that the tree is balanced(`isBalanced`) and two collectors to store the elements of the tree and thus balance them(`collectInorder` and `collectIntervalsInorder`). Finally, we added a public method to balance the tree(`balance`).

##### Why Balancing?
We decided to implement this to make the cost of operations O(log n) on average, because unbalanced trees can degrade to O(n). We had to research how to do this, and we found that the best way was to sort the intervals and then build the tree recursively by choosing the median interval as the root at each step.

This is done in two steps:
1. We perform an in-order traversal of the tree to collect all intervals in a sorted vector (using `collectIntervalsInorder()`).
2. We recursively build a balanced tree from the sorted intervals by choosing the median interval as the root at each step (using `buildBalancedFromIntervals()`).

**Note**: This was done in order to use divide and conquer to fulfill all the assignment requirements.

##### Visual Example
A visual example of balancing a tree:

```
    Unbalanced Tree:                Balanced Tree:
          5                              20
           \                            /  \
            10                        10    30
             \                       /  \   / \
              15                    5  15 25  35
               \
                20
                 \
                  25
                   \
                    30
                     \
                      35
```

Here we can see that we sorted the intervals and chose the `median (20)` as the root, making now a balanced tree that has the same amount of nodes in the left and right subtrees. This balanced tree is optimal; however, the balanced tree can also vary depending on the input intervals and not be perfectly balanced like this example.

##### buildBalancedFromIntervals() Implementation
Here is the code for building the balanced tree using divide and conquer:

```cpp
//Build balanced tree from intervals
Node<T>* buildBalancedFromIntervals(std::vector<Interval<T>>& intervals, int start, int end) {
    // Base case: no elements to add in this range (ended recursion because we are out of bounds)
    if (start > end) return nullptr;

    // 1. Find the middle element (divide step)
    int mid = start + (end - start) / 2;

    // 2. Create a node with the middle interval (this becomes the root of subtree)
    Node<T>* node = new Node<T>(intervals[mid]);

    // 3. Recursively build left subtree from elements smaller than mid (conquer left)
    node->left = buildBalancedFromIntervals(intervals, start, mid - 1);

    // 4. Recursively build right subtree from elements greater than mid (conquer right)
    node->right = buildBalancedFromIntervals(intervals, mid + 1, end);

    // 5. Calculate maxRange for this node (maximum end value in entire subtree)
    updateMaxRange(node);

    // We return the constructed node, because it will replace the previous subtree root
    return node;
}
```

##### buildFromIntervals() and Lambda Functions
This function takes the vector of intervals and builds a balanced tree from them. It first sorts the intervals and then calls `buildBalancedFromIntervals`.

In this function, we use a `lambda function` to sort the intervals based on their start values. We are going to explain it here as it is out of the contents of this course, but it is widely used in modern C++.

###### Lambda Functions in C++
Lambda functions are a way to define anonymous functions in C++. They are useful for short functions that are not going to be used anywhere else. The syntax for a lambda function is:

```cpp
[capture](parameters) -> return_type {
    // function body
}
```
- `capture`: This is where is specified which variables from the surrounding scope are accessible inside the lambda. It can be empty `[]`, or it can capture variables by value `[=]` or by reference `[&]`.
- `parameters`: This is where the parameters of the function are defined, similar to regular functions.
- `return_type`: This is optional and specifies the return type of the function.
- `function body`: This is where the code of the function is written.

Here is the implementation of `buildFromIntervals`:

```cpp
//Build balanced tree from a vector of intervals
void buildFromIntervals(std::vector<Interval<T>>& intervals) {
    if (intervals.empty()) return;

    //Sort by interval start using lambda
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval<T>& a, const Interval<T>& b) {
            return a.start < b.start;
        });

    deleteAll(root);
    root = buildBalancedFromIntervals(intervals, 0, intervals.size() - 1);
    nelem = intervals.size();
}
```

As you can see, we used a lambda function to define the comparison logic for sorting the intervals based on their start values. If we had not used a lambda function, we would have to implement a separate function for that comparison.

This function first sorts all the intervals, then deletes the previous tree and builds a new balanced tree from the sorted intervals using `buildBalancedFromIntervals`, finally assigning the new tree to the `root` pointer of the `Tree` class.

#### Other Utility Methods
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