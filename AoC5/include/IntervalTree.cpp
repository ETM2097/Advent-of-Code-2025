#include "IntervalTree.h"

IntervalNode* IntervalTree::insert(IntervalNode* node, long long start, long long end) {
    // Base case: If the tree is empty, return a new node
    if (node == nullptr) {
        return new IntervalNode(start, end);
    }
    // Otherwise, recur down the tree
    if (start < node->range.start) {
        node->left = insert(node->left, start, end);
    } 
    else {
        node->right = insert(node->right, start, end);
    }

    // Here we update the max value of the ancestor node
    if (node->maxRange < end) {
        node->maxRange = end;
    }
    // Finally return the node pointer
    return node;
}

// We check if there is an overlap in the tree
bool IntervalTree::overlaps(const Interval& a, long long ID) const {
    return (a.start <= ID && ID <= a.end);
}

bool IntervalTree::search(IntervalNode* node, long long ID) const {
    // Base case, node is null
    if (node == nullptr) {
        return false;
    }
    // We check if the current node overlaps with the given ID
    if (overlaps(node->range, ID)) {
        return true;
    }
    // If the left exists and maxRange is greater or equal to ID, we search left
    if (node->left != nullptr && node->left->maxRange >= ID) {
        if (search(node->left, ID)) {
            return true;
        }
    }
    // Otherwise, we search right
    return search(node->right, ID);
}

void IntervalTree::deleteTree(IntervalNode* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

IntervalNode* IntervalTree::buildBalanced(std::vector<Interval>& intervals, int start, int end) {
    // Base case: no elements to add in this range
    if (start > end) return nullptr;

    // 1. Find the middle element
    int mid = start + (end - start) / 2;
    
    // 2. Create a node with the middle interval (this becomes the main of subtree)
    IntervalNode* node = new IntervalNode(intervals[mid].start, intervals[mid].end);

    // 3. Recursively build left subtree from elements BEFORE mid
    node->left = buildBalanced(intervals, start, mid - 1);
    
    // 4. Recursively build right subtree from elements AFTER mid
    node->right = buildBalanced(intervals, mid + 1, end);

    // 5. Calculate maxRange for this node (maximum end value in entire subtree)
    node->maxRange = node->range.end;
    if (node->left && node->left->maxRange > node->maxRange) {
        node->maxRange = node->left->maxRange;
    }
    if (node->right && node->right->maxRange > node->maxRange) {
        node->maxRange = node->right->maxRange;
    }

    return node;
}

// Public methods implementation

IntervalTree::IntervalTree() : main(nullptr) {}

IntervalTree::~IntervalTree() {
    deleteTree(main);
}

void IntervalTree::insert(long long low, long long high) {
    main = insert(main, low, high);
}

bool IntervalTree::contains(long long point) const {
    return search(main, point);
}

// Build a balanced tree from a vector of intervals
void IntervalTree::buildFromIntervals(std::vector<Interval>& intervals) {
    if (intervals.empty()) return;

    // Sort by start value using lambda
    std::sort(intervals.begin(), intervals.end(),
        [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

    deleteTree(main);
    main = buildBalanced(intervals, 0, intervals.size() - 1);
}