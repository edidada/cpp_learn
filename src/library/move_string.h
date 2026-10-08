// move_string.h —— C++11 移动语义 String（库代码值类型设计示范）
//
// 库里的"值类型"（std::string/std::vector/std::optional...）在 C++11 后
// 都能做到：拷贝是深拷贝、移动是"偷资源"。扩容/按值返回不再有额外开销。
//
// 关键点：
//  - 移动构造/移动赋值标记 noexcept：容器才能安全地用它做强异常保证
//  - 拷贝赋值用 copy-and-swap：一个函数同时兼容拷贝与移动（传值参数）
//  - swap 惯用法：异常安全 + noexcept
#ifndef CPP11_LIB_MOVE_STRING_H
#define CPP11_LIB_MOVE_STRING_H

#include <cstring>
#include <cstddef>
#include <utility>

namespace lib11 {

class move_string {
public:
    move_string() noexcept : data_(nullptr), size_(0), cap_(0) {}

    move_string(const char* s) : data_(nullptr), size_(0), cap_(0) {
        if (s && *s) {
            size_ = std::strlen(s);
            reserve(size_ + 1);
            std::memcpy(data_, s, size_ + 1);
        }
    }

    // 拷贝构造：深拷贝（安全，代价高）
    move_string(const move_string& o) : data_(nullptr), size_(o.size_), cap_(0) {
        if (o.data_) {
            reserve(size_ + 1);
            std::memcpy(data_, o.data_, size_ + 1);
        }
    }

    // 移动构造：偷走对方的资源；noexcept 是必须的
    move_string(move_string&& o) noexcept
        : data_(o.data_), size_(o.size_), cap_(o.cap_) {
        o.data_ = nullptr; o.size_ = 0; o.cap_ = 0;
    }

    // 拷贝/移动赋值二合一：传值参数（拷贝还是移动由调用方决定）+ swap
    move_string& operator=(move_string o) noexcept {
        swap(o);
        return *this;
    }

    ~move_string() { delete[] data_; }

    void swap(move_string& o) noexcept {
        using std::swap;
        swap(data_, o.data_);
        swap(size_, o.size_);
        swap(cap_, o.cap_);
    }

    const char*  c_str() const noexcept { return data_ ? data_ : ""; }
    std::size_t  size()  const noexcept { return size_; }
    bool         empty() const noexcept { return size_ == 0; }

private:
    void reserve(std::size_t n) {
        if (n <= cap_) return;
        char* nd = new char[n];
        if (data_) std::memcpy(nd, data_, size_);
        delete[] data_;
        data_ = nd;
        cap_ = n;
    }

    char*        data_;
    std::size_t  size_;
    std::size_t  cap_;
};

// 自由 swap：配合容器/算法的 ADL 调用
template <typename T>
void swap(T& a, T& b) noexcept(noexcept(a.swap(b))) { a.swap(b); }

} // namespace lib11

#endif // CPP11_LIB_MOVE_STRING_H
