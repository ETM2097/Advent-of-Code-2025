// This is a short Has Map implementation for the memoization in the DFS algorithm of the second part
// of the 7'th day of the Advent of Code 2025 challenge.
// It just implements the needed methods for this specific case, anyway we opted to use templates to make it reusable
// for the future challenges.

#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <utility>
#include <stdexcept>

template<typename K, typename T> 
class HashMap {
    private:
        static const int hashSize = 25013; // A prime number that is larger than the expected number of entries
        std::vector<std::list<std::pair<K, T>>> map;
    
        // Calculate hash for the key
        int hashFunction(K key) const {
            return key % hashSize;
        }

    public:
        HashMap() : map(hashSize) {}

        // Here we insert or update the key-value pair
        void set(int key, const T& value) {
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
        }

        // Now we add a fucntion for checking if a key exists
        bool contains(K key) const {
            int hash = hashFunction(key);
            for (const auto& pair : map[hash]) {
                if (pair.first == key) {
                    return true;
                }
            }
            return false;
        }

        // Now we add a function to get the value for a key
        T get(K key) const {
            int hash = hashFunction(key);
            for (const auto& pair : map[hash]) {
                if (pair.first == key) {
                    return pair.second;
                }
            }
            // In case the key does not exist, we throw an exception because we do not want to return a default value.
            throw std::runtime_error("Key not found");
        }
};

#endif