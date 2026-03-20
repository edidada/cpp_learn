#include <iostream>
#include <memory>

template <typename T>
class MySet {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(T val) : data(val), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;

    bool insertHelper(std::unique_ptr<Node>& node, const T& value) {
        if (!node) {
            node = std::unique_ptr<Node>(new Node(value));
            return true;  // 新插入节点
        }

        if (value < node->data) {
            return insertHelper(node->left, value);
        } else if (value > node->data) {
            return insertHelper(node->right, value);
        } else {
            return false;  // 元素已存在，不插入重复元素
        }
    }

    bool findHelper(const Node* node, const T& value) const {
        if (!node) return false;

        if (value < node->data) {
            return findHelper(node->left.get(), value);
        } else if (value > node->data) {
            return findHelper(node->right.get(), value);
        } else {
            return true;
        }
    }

    void inOrderTraversal(const Node* node) const {
        if (node) {
            inOrderTraversal(node->left.get());
            std::cout << node->data << " ";
            inOrderTraversal(node->right.get());
        }
    }

public:
    MySet() : root(nullptr) {}
    
    bool insert(const T& value) {
        return insertHelper(root, value);
    }

    bool find(const T& value) const {
        return findHelper(root.get(), value);
    }

    void print() const {
        inOrderTraversal(root.get());
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
