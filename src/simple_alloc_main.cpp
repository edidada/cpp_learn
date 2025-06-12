#include <iostream>
#include <cstddef>
#include <climits>
#include <cstring>
#include <vector>

template<typename T>
class simple_alloc {
private:
    // 内存池，使用vector模拟
    static std::vector<char> memory_pool;
    static std::vector<char*> free_list;

public:
    static size_t round_up(size_t bytes) {
        return (bytes + sizeof(T) - 1) / sizeof(T) * sizeof(T);
    }

    static T* allocate(size_t n) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        T* chunk = nullptr;
        if (free_list.size() > 0) {
            // 如果有可用的内存块，则取出一个用于分配
            chunk = reinterpret_cast<T*>(free_list.back());
            free_list.pop_back();
        } else {
            // 没有可用内存，则分配一块新内存
            size_t bytes = round_up(n * sizeof(T));
            memory_pool.resize(memory_pool.size() + bytes);
            chunk = reinterpret_cast<T*>(&memory_pool.back() - bytes + 1);
        }
        return chunk;
    }

    static void deallocate(T* p, size_t n) {
        size_t bytes = round_up(n * sizeof(T));
        // 将内存块放回内存池，而不是直接释放
        free_list.push_back(reinterpret_cast<char*>(p) - 1);
    }

    static size_t max_size() {
        return size_t(-1) / sizeof(T);
    }
};

template<typename T>
std::vector<char> simple_alloc<T>::memory_pool;

template<typename T>
std::vector<char*> simple_alloc<T>::free_list;

int main() {
    // 使用simple_alloc分配和释放内存
    int* ptr = simple_alloc<int>::allocate(10);
    // 使用ptr...
    simple_alloc<int>::deallocate(ptr, 10);

    return 0;
}