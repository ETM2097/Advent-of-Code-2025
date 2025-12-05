#ifndef INTERVAL_TREE_H
#define INTERVAL_TREE_H

#include <iostream>
#include <vector>
#include <algorithm>

/*Structure of an interval, it consist in two variables:
    1. start: It marks the start of the interval.
    2. end: It marks the end of an interval.
*/
struct Interval {
    long long start;
    long long end;
};

/*Structure of a Node for the IntervalTree, it consists of the following */
struct IntervalNode {
    Interval range;
    IntervalNode* left;
    IntervalNode* right;
    long long maxRange;

    IntervalNode(long long start, long long end) : range{start, end}, left(nullptr), right(nullptr) {}
};

class IntervalTree {
    private:
        IntervalNode* main;

        IntervalNode* insert(IntervalNode* node, long long start, long long end);
        bool overlaps( const Interval& a, long long ID) const;
        bool search(IntervalNode* node, long long ID) const;
        void deleteTree(IntervalNode* node);
        IntervalNode* buildBalanced(std::vector<Interval>& intervals, int start, int end);

    public:
        IntervalTree();
        ~IntervalTree();
        void insert(long long low, long long high);
        bool contains(long long point) const;
        void buildFromIntervals(std::vector<Interval>& intervals);
};

#endif // INTERVAL_TREE_H