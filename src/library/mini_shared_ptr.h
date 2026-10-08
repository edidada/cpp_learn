// mini_shared_ptr.h —— 简易 shared_ptr（C++11 智能指针核心机制演示）
//
// 演示库代码如何管理"共享所有权"：
//  - 引用计数（堆上控制块）记录还有多少个 shared_ptr 指向同一对象
//  - 最后一个所有者析构时释放对象 + 控制块
//  - 移动构造/移动赋值转移所有权，计数不变
//
// 与 std::shared_ptr 的差距：无线程安全、无自定义删除器、
// 无 weak_ptr / aliasing / enable_shared_from_this，仅用于学习。
#ifndef CPP11_LIB_MINI_SHARED_PTR_H
#define CPP11_LIB_MINI_SHARED_PTR_H

#include <cstddef>
#include <utility>

namespace lib11 {

template <typename T>
class mini_shared_ptr {
public:
    typedef T element_type;

    mini_shared_ptr() noexcept : ptr_(nullptr), count_(nullptr) {}

    explicit mini_shared_ptr(T* p) : ptr_(p), count_(p ? new std::size_t(1) : nullptr) {}

    // 拷贝：共享所有权，计数 +1
    mini_shared_ptr(const mini_shared_ptr& o) noexcept
        : ptr_(o.ptr_), count_(o.count_) {
        if (count_) ++(*count_);
    }

    // 移动：转移所有权，计数不变；noexcept 让容器敢于使用
    mini_shared_ptr(mini_shared_ptr&& o) noexcept
        : ptr_(o.ptr_), count_(o.count_) {
        o.ptr_ = nullptr; o.count_ = nullptr;
    }

    // 拷贝/移动赋值二合一：传值 + swap（计数自动正确）
    mini_shared_ptr& operator=(mini_shared_ptr o) noexcept {
        swap(o);
        return *this;
    }

    ~mini_shared_ptr() { release(); }

    T* get() const noexcept { return ptr_; }
    T& operator*()  const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }

    std::size_t use_count() const noexcept { return count_ ? *count_ : 0; }
    bool unique() const noexcept { return use_count() == 1; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    void reset() noexcept { release(); }

    void reset(T* p) {
        release();
        ptr_   = p;
        count_ = p ? new std::size_t(1) : nullptr;
    }

    void swap(mini_shared_ptr& o) noexcept {
        using std::swap;
        swap(ptr_, o.ptr_);
        swap(count_, o.count_);
    }

private:
    void release() noexcept {
        if (count_ && --(*count_) == 0) {
            delete ptr_;
            delete count_;
        }
        ptr_ = nullptr; count_ = nullptr;
    }

    T*           ptr_;
    std::size_t* count_;
};

} // namespace lib11

#endif // CPP11_LIB_MINI_SHARED_PTR_H
