// scope_guard03.h —— C++03 RAII ScopeGuard
//
// 库代码经常要保证"离开作用域时执行清理"（关文件、解锁、释放内存），
// 即使中途抛出异常也不能漏。C++03 用 RAII 封装成对象即可做到。
//
// 关键点（C++03 的限制）：
//  - 没有移动语义 -> 用"复制即转移"模拟：复制构造后原对象作废
//  - 没有 lambda   -> 回调必须用函数对象（仿函数）
//  - 没有可变参数模板 -> 无法做 C++17 那样的工厂函数，改用 make_guard
#ifndef CPP03_LIB_SCOPE_GUARD_H
#define CPP03_LIB_SCOPE_GUARD_H

namespace lib03 {

template <typename Fun>
class scope_guard {
public:
    explicit scope_guard(Fun f)
        : fun_(f), active_(true) {}

    // C++03 没有移动构造：复制即转移（转移后原对象失效）
    scope_guard(const scope_guard& o)
        : fun_(o.fun_), active_(o.active_) {
        o.active_ = false;
    }

    ~scope_guard() {
        if (active_) fun_();       // 唯一一次执行清理
    }

    // 主动取消：把清理责任移交出去（例如 return 前交给调用方）
    void dismiss() { active_ = false; }

private:
    Fun    fun_;
    mutable bool active_;

    // C++03 惯用法：声明为私有且不实现 -> 编译期禁止赋值
    void operator=(const scope_guard&);
};

// 工厂：C++03 里避免直接写模板类型名的繁琐
template <typename Fun>
scope_guard<Fun> make_guard(Fun f) {
    return scope_guard<Fun>(f);
}

} // namespace lib03

#endif // CPP03_LIB_SCOPE_GUARD_H
