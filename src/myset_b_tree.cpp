#include <iostream>

template <typename T>
class MySet {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(T val) : data(val), left(0), right(0) {}
    };

    Node* root;

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    bool insertHelper(Node*& node, const T& value) {
        if (!node) {
            node = new Node(value);
            return true;
        }

        if (value < node->data) {
            return insertHelper(node->left, value);
        } else if (value > node->data) {
            return insertHelper(node->right, value);
        } else {
            return false;
        }
    }

    bool findHelper(const Node* node, const T& value) const {
        if (!node) return false;

        if (value < node->data) {
            return findHelper(node->left, value);
        } else if (value > node->data) {
            return findHelper(node->right, value);
        } else {
            return true;
        }
    }

    void inOrderTraversal(const Node* node) const {
        if (node) {
            inOrderTraversal(node->left);
            std::cout << node->data << " ";
            inOrderTraversal(node->right);
        }
    }

public:
    MySet() : root(0) {}
    
    ~MySet() {
        destroyTree(root);
    }

    bool insert(const T& value) {
        return insertHelper(root, value);
    }

    bool find(const T& value) const {
        return findHelper(root, value);
    }

    void print() const {
        inOrderTraversal(root);
        std::cout << std::endl;
    }
};

int main() {
    MySet<int> mySet;

    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(8);
    mySet.insert(1);
    mySet.insert(4);

    std::cout << "Find 3: " << mySet.find(3) << std::endl;
    std::cout << "Find 10: " << mySet.find(10) << std::endl;

    std::cout << "Elements in MySet: ";
    mySet.print();

    return 0;
}
