#include "comparator_examples.h"

int main() {
    demonstrate_sort_with_comparators();
    demonstrate_priority_queue_with_comparators();
    return 0;
}

void demonstrate_sort_with_comparators() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6};
    
    // 使用 std::less 升序排序 (默认)
    std::sort(numbers.begin(), numbers.end(), std::less<int>());
    std::cout << "Ascending order (std::less): ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    
    // 使用 std::greater 降序排序
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());
    std::cout << "Descending order (std::greater): ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n\n";
}

void demonstrate_priority_queue_with_comparators() {
    // 使用 std::greater 创建最小堆 (小顶堆)
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
    min_heap.push(3);
    min_heap.push(1);
    min_heap.push(4);
    min_heap.push(1);
    min_heap.push(5);
    
    std::cout << "Min heap (std::greater) top elements: ";
    while (!min_heap.empty()) {
        std::cout << min_heap.top() << " ";
        min_heap.pop();
    }
    std::cout << "\n";
    
    // 使用 std::less 创建最大堆 (大顶堆, 默认)
    std::priority_queue<int> max_heap; // 等同于 std::priority_queue<int, std::vector<int>, std::less<int>>
    max_heap.push(3);
    max_heap.push(1);
    max_heap.push(4);
    max_heap.push(1);
    max_heap.push(5);
    
    std::cout << "Max heap (std::less) top elements: ";
    while (!max_heap.empty()) {
        std::cout << max_heap.top() << " ";
        max_heap.pop();
    }
    std::cout << "\n";
}