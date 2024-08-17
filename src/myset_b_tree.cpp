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

    // 递归插入新元素
    bool insertHelper(std::unique_ptr<Node>& node, const T& value) {
        if (!node) {
            node = std::unique_ptr<Node>(new Node(value));
//            node = std::make_unique<Node>(value);
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

    // 递归查找元素
    bool findHelper(const Node* node, const T& value) const {
        if (!node) return false;

        if (value < node->data) {
            return findHelper(node->left.get(), value);
        } else if (value > node->data) {
            return findHelper(node->right.get(), value);
        } else {
            return true;  // 找到元素
        }
    }

    // 中序遍历并打印所有元素
    void inOrderTraversal(const Node* node) const {
        if (node) {
            inOrderTraversal(node->left.get());
            std::cout << node->data << " ";
            inOrderTraversal(node->right.get());
        }
    }

public:
    // 插入新元素
    bool insert(const T& value) {
        return insertHelper(root, value);
    }

    // 查找元素
    bool find(const T& value) const {
        return findHelper(root.get(), value);
    }

    // 打印集合中的所有元素
    void print() const {
        inOrderTraversal(root.get());
        std::cout << std::endl;
    }
};

int main() {
    MySet<int> mySet;

    // 插入一些元素
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(8);
    mySet.insert(1);
    mySet.insert(4);

    // 查找元素
    std::cout << "Find 3: " << mySet.find(3) << std::endl;  // 输出1（true）
    std::cout << "Find 10: " << mySet.find(10) << std::endl;  // 输出0（false）

    // 打印集合中的元素
    std::cout << "Elements in MySet: ";
    mySet.print();  // 应按顺序输出 1 3 4 5 8

    return 0;
}
