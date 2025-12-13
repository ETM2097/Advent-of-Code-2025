#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

using namespace std;

// Union-Find (DSU) data structure
struct UnionFind {
    vector<int> parent;
    vector<int> size;
    
    UnionFind(int n);
    int find(int n);
    void unite(int n1, int n2);
    int getSize(int n);
};

#endif