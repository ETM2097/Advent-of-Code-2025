# Implementation of HashMap and Graph for Advent of Code 2025: Day 11

## HashMap Implementation
The `HashMap` class we implemented is a templated hash map that supports generic key-value pairs. We decided to use different collision resolution using separate chaining with linked lists. But this is the same as in Day 7. We added new functionalities to the HashMap to support our needs in Day 11 taht will be described below.

### Key Features
- **Templated Design**: The `HashMap` class is templated to allow for any data type as key and value (Updated on AoC7 as well).
- **Custom Hash Functions**: We implemented a default hash function and provided the ability to define custom hash functions for complex key types, such as tuples, strings and pairs. we have the following hash functions:
  - `DefaultHash`: A simple hash function for basic types.
  - `StringHash`: A hash function specifically for strings. We used it as DefaultHash for strings.
  - `PairHash`: A hash function for pairs of values.
  - `TupleHash`: A hash function for tuples of arbitrary size.
- **Collision Resolution**: We used separate chaining with linked lists to handle collisions.
- **Dynamic Resizing**: The hash map now dynamically resizes when the load factor exceeds a threshold.
- **Basic Operations**: The class now supports insertion (`set`), appending to vectors (`append`), retrieval (`get`), deletion (`remove`), and checking for key existence (`contains`).
- **Clear Functionality**: Added a `clear` method to reset the hash map to its initial empty state.

#### Append Method
The `append` method allows adding a value to a vector stored at a specific key. We decided to add this method because it became critical for our graph implementation, but we struggled to implement it correctly, mainly because we wanted to keep the HashMap generic enough to be used in other scenarios. We investigated and found that using `std::is_same` from the `<type_traits>` library allowed us to check if the value type is a vector at compile time. This way, we could ensure that the `append` method is only used when the value type is indeed a vector, preventing potential runtime errors. The correct syntax of the conditional was provided by Copilot. 

The code looks as follows:

```cpp
template<typename K, typename V>
template<typename ElementType>
        typename std::enable_if<std::is_same<T, std::vector<ElementType>>::value, void>::type 
``` 
This ensures that the `append` method is only instantiated when `T` is a vector type. The template parameter `ElementType` represents the type of elements stored in the vector. However, it is important to note that this does not allow appending arbitrary types to the vector. The type of the value being appended must match the `ElementType` of the vector. For example, if `T` is `std::vector<int>`, only `int` values can be appended.

The `append` method code is as follows:

```cpp
    void append(const K& key, const ElementType& value) {
        for (auto& pair : map[hashFunction(key)]) {
            if (pair.first == key) {
                pair.second.push_back(value); // Appends to existing vector
                return;
            }
        }
        // If not found, we insert new key-value pair with a vector containing the value
        map[hashFunction(key)].emplace_back(key, T{value});
        numElements++;
        // This is a resizing check after insertion
        if (static_cast<double>(numElements) / hashSize > loadFactorThreshold) {
            resize();
        }
   }
```

## Graph Implementation
The `Graph` class is a templated directed graph implementation that uses the `HashMap` class for storing adjacency lists and other graph-related data. We decided to implement this graph structure to efficiently handle the directed acyclic graph (DAG) required for Day 11's challenge.

This implementation differs from Day 7's graph implementation in several ways and we will describe them below:

### Key Features
- **Templated Design**: The `Graph` class is templated to allow for any data type as nodes.
- **Adjacency Lists**: The graph uses adjacency lists stored in `HashMap` instances for both forward and backward edges.
- **Weighted Edges**: Although not used in Day 11, we added support for weighted edges by implementing a separate adjacency list for weighted edges.
- **Node Management**: The graph keeps track of all unique nodes and their in-degrees.
- **Graph Operations**: The class supports `