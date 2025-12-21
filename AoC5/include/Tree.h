#ifndef TREE_H
#define TREE_H

#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <variant>

using namespace std;

/*Structure of an interval, it consist in two variables:
    1. start: It marks the start of the interval.
    2. end: It marks the end of an interval.
*/
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

template <typename T>
class Node {
    public:
        variant<T, Interval<T>> data;  //Can store either T or Interval<T>
        Node* left;
        Node* right;
        T maxRange;  //Maximum end value in the subtree

        //Constructor for simple elements
        Node(T elem, Node* left = nullptr, Node* right = nullptr) : 
            data(elem), left(left), right(right), maxRange(elem) {}
        
        //Constructor for intervals
        Node(T start, T end, Node* left = nullptr, Node* right = nullptr) :
            data(Interval<T>(start, end)), left(left), right(right), maxRange(end) {}
        
        //Constructor with Interval
        Node(const Interval<T>& interval, Node* left = nullptr, Node* right = nullptr) :
            data(interval), left(left), right(right), maxRange(interval.end) {}
        
        //Helper methods to check and access data
        bool isInterval() const {
            return holds_alternative<Interval<T>>(data); // Check if data holds an Interval (method from <variant>)
        }
        
        //Get start value (for both T and Interval)
        T getStart() const {
            if (isInterval()) {
                return get<Interval<T>>(data).start;
            } else {
                return get<T>(data);
            }
        }
        
        //Get end value (for both T and Interval)
        T getEnd() const {
            if (isInterval()) {
                return get<Interval<T>>(data).end;
            } else {
                return get<T>(data);
            }
        }
        
        //Get the interval (if it's a single value, return [val, val])
        Interval<T> getRange() const {
            if (isInterval()) {
                return get<Interval<T>>(data);
            } else {
                T val = get<T>(data);
                return Interval<T>(val, val);
            }
        }
};

template <typename T>
class Tree {
    private:
        int nelem;
        Node<T> *root;

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

        //Search for an element in the tree
        Node<T>* search(Node<T>* n, T e) const {
            if (n == nullptr) {
                throw runtime_error("Element not found");
            } else if(n->getStart() < e ){
                return search(n->right, e);
            } else if(n->getStart() > e ){
                return search(n->left, e);
            } else {
                return n;
            }
        }

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
        
        //Check if two intervals overlap
        bool overlaps(const Interval<T>& a, const Interval<T>& b) const {
            return (a.start <= b.end && b.start <= a.end);
        }
        
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

        //Remove an element from the tree
        Node<T>* remove(Node<T>* n, T e){
            if(n == nullptr){
                throw runtime_error("Element not found");
            } else if(n->getStart() < e){
                n->right = remove(n->right, e);
            } else if(n->getStart() > e){
                n->left = remove(n->left, e);
            } else {
                if (n->left != nullptr && n->right != nullptr) {
                    T maxVal = max(n->left);
                    n->data = maxVal;
                    n->left = remove_max(n->left);
                } else {
                    n = (n->left != nullptr) ? n->left : n->right;
                }
            }
            updateMaxRange(n);
            return n;
        }

        //Remove maximum element in the tree
        Node<T>* remove_max(Node<T>* n){
            if(n->right == nullptr) {
                return n->left;
            } else {
                n->right = remove_max(n->right);
                updateMaxRange(n);
                return n;
            }
        }

        //Remove minimum element in the tree
        Node<T>* remove_min(Node<T>* n){
            if(n->left == nullptr) {
                return n->right;
            } else {
                n->left = remove_min(n->left);
                updateMaxRange(n);
                return n;
            }
        }

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
        //Delete all nodes in the tree
         void deleteAll(Node<T>* n){
            if(n != nullptr){
                deleteAll(n->left);
                deleteAll(n->right);
                delete n;
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
        
        //Inorder printing
        std::string print_inorder(Node<T>* n) const {
            if (n == nullptr) {
                return "";
            }
            std::string result = "";
            result += print_inorder(n->left);
            if (n->isInterval()) {
                Interval<T> range = n->getRange();
                result += "[" + std::to_string(range.start) + "," + std::to_string(range.end) + "] ";
            } else {
                result += std::to_string(n->getStart()) + " ";
            }
            result += print_inorder(n->right);
            return result;
        }
        
        //Preorder printing
        std::string print_preorder(Node<T>* n) const {
            if (n == nullptr) {
                return "";
            }
            std::string result = "";
            if (n->isInterval()) {
                Interval<T> range = n->getRange();
                result += "[" + std::to_string(range.start) + "," + std::to_string(range.end) + "] ";
            } else {
                result += std::to_string(n->getStart()) + " ";
            }
            result += print_preorder(n->left);
            result += print_preorder(n->right);
            return result;
        }

        //Postorder printing
        std::string print_postorder(Node<T>* n) const {
            if (n == nullptr) {
                return "";
            }
            std::string result = "";
            result += print_postorder(n->left);
            result += print_postorder(n->right);
            if (n->isInterval()) {
                Interval<T> range = n->getRange();
                result += "[" + std::to_string(range.start) + "," + std::to_string(range.end) + "] ";
            } else {
                result += std::to_string(n->getStart()) + " ";
            }
            return result;
        }
        
    public:
        //Constructor
        Tree() {
            this->nelem = 0;
            this->root = nullptr;
        }
        
        //Get number of elements
        int size() const {
            return this->nelem;
        }

        //Check if tree is empty
        bool empty() const {
            return this->nelem == 0;
        }

        //Search for an element and return it
        T search(T e) const {
            return search(this->root, e)->getStart();
        }

        //Search if an element exists in a range [start, end]
        bool contains(T start, T end) const {
            if (empty()) return false;
            return searchRange(this->root, start, end);
        }

        //Alternative version: search if a point is in any interval
        bool containsPoint(T point) const {
            if (empty()) return false;
            return searchRange(this->root, point, point);
        }
        
        //IntervalTree style method: check if a point falls in any stored interval
        bool intervalContains(T point) const {
            if (empty()) return false;
            return searchIntervalContains(this->root, point);
        }
        
        //Check if there's an interval that overlaps with [start, end]
        bool hasOverlappingInterval(T start, T end) const {
            if (empty()) return false;
            return searchOverlappingInterval(this->root, start, end);
        }

        //Insert a single element
        void insert(T e) {
            this->root = insert(this->root, e);
            this->nelem++; 
        }
        
        //Insert an interval using the Interval structure
        void insert(const Interval<T>& interval) {
            this->root = insert(this->root, interval);
            this->nelem++;
        }
        
        //Remove an element
        void remove(T e) {
            root = remove(this->root, e);
            this->nelem--;  
        }

        //Maximum element
        T max() const {
            return max(this->root);
        }

        //Minimum element
        T min() const {
            return min(this->root);
        }

        //Height of the tree
        int height() const {
            return height(this->root);
        }

        //Check if the tree is balanced
        bool isBalanced() const {
            int h = 0;
            return isBalanced(this->root, h);
        }

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

        //Destructor
        ~Tree() { 
            deleteAll(root); 
        }
        
        //Inorder printing
        void print_inorder(std::ostream &out) const {
            out << print_inorder(this->root);
        }
        
        //Preorder printing
        void print_preorder(std::ostream &out) const {
            out << print_preorder(this->root);
        }
        
        //Postorder printing
        void print_postorder(std::ostream &out) const {
            out << print_postorder(this->root);
        }
        
        //Inorder printing as string
        std::string tree_inorder() const {
            return print_inorder(this->root);
        }
        
        //Preorder printing as string
        std::string tree_preorder() const {
            return print_preorder(this->root);
        }
        
        //Postorder printing as string
        std::string tree_postorder() const {
            return print_postorder(this->root);
        }
};

#endif
