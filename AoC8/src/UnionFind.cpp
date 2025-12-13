#include "../include/UnionFind.h"
#include <algorithm>


// we implemented an algorithm called Union-Find or DSU to manage connected components.
// we used the video below as a reference: https://youtu.be/8uOIs4aXBiA?si=x4nSsaF8_1KiwwLO

// constructor, initialize parent and size vectors with n elements
UnionFind::UnionFind(int n) {
    parent.resize(n);
    size.resize(n, 1);
    // each node is its own parent at the beginning
    for(int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

// find the root of n 
int UnionFind::find(int n) {
    if(parent[n] != n) {
        parent[n] = find(parent[n]);
    }
    return parent[n];
}

// unite the sets that contain n1 and n2
void UnionFind::unite(int n1, int n2) {
    n1 = find(n1);
    n2 = find(n2);
    if(n1 == n2) return;
    if(size[n1] < size[n2]) {
        swap(n1, n2);
    }
    parent[n2] = n1;
    size[n1] += size[n2];
}

// get the size of the set that contains n
int UnionFind::getSize(int n) {
    return size[find(n)];
}