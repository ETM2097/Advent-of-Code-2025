// This is a full implementation of a HashMap that we ended using in our problems and classes for Advent of Code 2025
// It supports generic key-value pairs, custom hash functions, dynamic resizing, and basic operations like insertion, retrieval, deletion, and checking for existence.
// It also includes an append method specifically for vector types to facilitate adjacency list management in graph structures.

// We worked hard to make this implementation efficient and robust, suitable for any type of scenario encountered in AoC challenges.

#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <utility>
#include <stdexcept>
#include <tuple>
#include <string>
#include <type_traits>

// Default hash function for single keys, we use modulo of the key value
template<typename K>
struct DefaultHash {
    int operator()(const K& key, int hashSize) const {
        return static_cast<unsigned long long>(key) % hashSize;
    }
};

// Specialization for string hashing (Day 11 of AoC 2025)
template<> // Allows us to specialize the template of DefaultHash for strings
struct DefaultHash<std::string> {
    int operator()(const std::string& key, int hashSize) const {
        unsigned long long hash = 0;
        const unsigned long long prime = 31;
        for (char c : key) {
            hash = hash * prime + static_cast<unsigned long long>(c); // Used a prime multiplier to reduce collisions
                                                                    // And used casting to unsigned long long to avoid overflow
        }
        return hash % hashSize;
    }
};

// Hash for pair<A, B>, now we use a multiplication by a prime number to reduce collisions (Day 7 of AoC 2025)
template<typename A, typename B>
struct PairHash {
    int operator()(const std::pair<A, B>& key, int hashSize) const {
        return (static_cast<unsigned long long>(key.first) * 31 + 
                static_cast<unsigned long long>(key.second)) % hashSize;
    }
};

// Hash for tuple<A, B, C> , now we use the same method but for 3D coordinates (Day 8 of AoC 2025)
template<typename A, typename B, typename C>
struct TupleHash3 {
    int operator()(const std::tuple<A, B, C>& key, int hashSize) const {
        return ((static_cast<unsigned long long>(std::get<0>(key)) * 31 + 
                 static_cast<unsigned long long>(std::get<1>(key))) * 31 + 
                 static_cast<unsigned long long>(std::get<2>(key))) % hashSize;
    }
};

// Specialization for tuple<string, bool, bool> (for path tracking in AoC11_P2) (AI improved)
template<>
struct TupleHash3<std::string, bool, bool> {
    int operator()(const std::tuple<std::string, bool, bool>& key, int hashSize) const {
        // Hash the string first
        unsigned long long hash = 0;
        const unsigned long long prime = 31;
        const std::string& str = std::get<0>(key);
        for (char c : str) {
            hash = hash * prime + static_cast<unsigned long long>(c);
        }
        // Combine with boolean flags
        hash = hash * 31 + (std::get<1>(key) ? 1 : 0);
        hash = hash * 31 + (std::get<2>(key) ? 1 : 0);
        return hash % hashSize;
    }
};

template<typename K, typename T, typename Hash = DefaultHash<K>> 
class HashMap {
    private:
        int hashSize; // We opted to make the hashSize dynamic
        std::vector<std::list<std::pair<K, T>>> map;
        Hash hasher;
        int numElements; // Track the number of elements
        const double loadFactorThreshold = 0.75; // Load factor threshold for resizing, have not tested if it is optimal, 
                                                 // but we found that 0.7-0.8 is usually a good value

        // Calculate hash for the key
        int hashFunction(const K& key) const {
            return hasher(key, hashSize);
        }

        // We added a resize method that resizes the hash table when load factor exceeds the threshold
        void resize() {
            int newHashSize = hashSize * 2; // Double the size
            std::vector<std::list<std::pair<K, T>>> newMap(newHashSize); // We create the new hash table

            // We rehash all existing elements into the new table
            for (const auto& bucket : map) {
                for (const auto& pair : bucket) {
                    int newHash = hasher(pair.first, newHashSize);
                    newMap[newHash].emplace_back(pair);
                }
            }

            // Finally we replace the old map with the new one
            map = std::move(newMap); // We use the move function from the standard library to avoid copying
            hashSize = newHashSize; // We update the hash size
        }

    public:
        HashMap() : hashSize(25013), map(hashSize), numElements(0) {} // Now we initialize hashSize and numElements as we need them for resizing as default

        HashMap(int initialSize) : hashSize(initialSize), map(hashSize), numElements(0) {} // Constructor with custom initial size

        // Here we insert or update the key-value pair
        void set(const K& key, const T& value) {
            int hash = hashFunction(key);
            // Now we check if it already exists
            for (auto& pair : map[hash]) {
                if (pair.first == key) {
                    pair.second = value; // Updates existing value
                    return;
                }
            }
            // If not found, we insert new key-value pair
            map[hash].emplace_back(key, value);
            numElements++;

            // We now check load factor and resize if necessary
            if (static_cast<double>(numElements) / hashSize > loadFactorThreshold) { // We use static cast to avoid integer division
                resize();
            }
        }

        // Append for vector types only - enables only when T is a vector (Solution helped with AI as we strugled with the correct enable_if syntax)
        template<typename ElementType>
        typename std::enable_if<std::is_same<T, std::vector<ElementType>>::value, void>::type // This enables the function only if T is vector<ElementType>
        append(const K& key, const ElementType& value) { // Strings are also supported as they are vectors of char
            for (auto& pair : map[hashFunction(key)]) {
                if (pair.first == key) {
                    pair.second.push_back(value); // Appends to existing vector
                    return;
                }
            }
            // If not found, we insert new key-value pair with a vector containing the value
            map[hashFunction(key)].emplace_back(key, T{value});
            numElements++;

            // We now check load factor and resize if necessary
            if (static_cast<double>(numElements) / hashSize > loadFactorThreshold) {
                resize();
            }
        }

        // We will add a size function to get the number of elements
        int size() const {
            return numElements;
        }

        // Now we add a fucntion for checking if a key exists
        bool contains(const K& key) const {
            int hash = hashFunction(key);
            for (const auto& pair : map[hash]) {
                if (pair.first == key) {
                    return true;
                }
            }
            return false;
        }

        // Now we add a function to get the value for a key
        T get(const K& key) const {
            int hash = hashFunction(key);
            for (const auto& pair : map[hash]) {
                if (pair.first == key) {
                    return pair.second;
                }
            }
            // In case the key does not exist, we throw an exception because we do not want to return a default value.
            throw std::runtime_error("Key not found");
        }

        void remove(const K& key) {
            int hash = hashFunction(key);
            auto& bucket = map[hash];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) { // Iteration through the bucket for using erase function
                if (it->first == key) { // We found the key (would only work for structures with defined == operator)
                    bucket.erase(it);
                    numElements--;
                    return;
                }
            }
            throw std::runtime_error("Key not found for removal");
        }

        void clear() { // Function for clearing the hash map
            map.clear();
            map.resize(hashSize);
            numElements = 0;
        }
};

#endif