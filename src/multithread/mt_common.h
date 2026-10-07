// mt_common.h —— 并发/多线程演示公共层
//
// 对应 docs/multy_thread.md：
//   五、第四层 OS —— Thread 到底是什么（这里给出 C++98 也能用的 OS 线程包装）
//   三、第二层 ISA/汇编 —— CAS / atomic RMW（这里给出跨标准的 CAS 封装）
//
// 设计要点（很重要）：
//   所有演示文件只依赖本头文件 + 标准库，并用 #if 做"能力探测"。
//   在 cpp98 / cpp03 这种低标准分支里，C++11/17/20 的段落不会被删掉，
//   而是编译成"打印跳过原因"的惰性分支；同一份文件 merge 到高标准分支后，
//   自动多出可以真正跑起来的部分。
//   这样一份代码可以在 cpp98 → cpp26 全链路上编译、运行、对比。
//
// 输出约定：控制台一律打英文（避免 Windows 控制台 GBK/UTF-8 乱码），
// 知识点的中文解释都写在注释里。
#ifndef MT_COMMON_H
#define MT_COMMON_H

#include <cstddef>   // std::size_t
#include <cstdio>    // std::snprintf
#include <string>
#include <iostream>

// 平台头文件必须在任何 namespace 之外包含
#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#endif

// ---------------------------------------------------------------------------
// 1. 语言级别探测
//
// MSVC 的 __cplusplus 默认恒为 199711L（除非加 /Zc:__cplusplus），
// 真实级别在 _MSVC_LANG 里。所以跨编译器要用这个宏。
// ---------------------------------------------------------------------------
#if defined(_MSVC_LANG)
#define MT_STD_LANG _MSVC_LANG
#else
#define MT_STD_LANG __cplusplus
#endif

#define MT_HAS_11 (MT_STD_LANG >= 201103L)
#define MT_HAS_14 (MT_STD_LANG >= 201402L)
#define MT_HAS_17 (MT_STD_LANG >= 201703L)
#define MT_HAS_20 (MT_STD_LANG >= 202002L)
#define MT_HAS_23 (MT_STD_LANG >= 202302L)
#define MT_HAS_26 (MT_STD_LANG >= 202600L)

inline const char* mt_std_name() {
#if MT_HAS_26
    return "C++26";
#elif MT_HAS_23
    return "C++23";
#elif MT_HAS_20
    return "C++20";
#elif MT_HAS_17
    return "C++17";
#elif MT_HAS_14
    return "C++14";
#elif MT_HAS_11
    return "C++11";
#elif MT_STD_LANG >= 199711L
    return "C++98/03";
#else
    return "pre-C++98";
#endif
}

// 需要的语言级别：3=C++03，其余按年份号
inline const char* mt_std_label(int level) {
    switch (level) {
        case 3:  return "C++03";
        case 11: return "C++11";
        case 14: return "C++14";
        case 17: return "C++17";
        case 20: return "C++20";
        case 23: return "C++23";
        case 26: return "C++26";
        default: return "C++98";
    }
}

// ---------------------------------------------------------------------------
// 2. 统一输出格式：让"跑了什么 / 为什么跳过"一眼可见
// ---------------------------------------------------------------------------
inline void mt_section(int layer, const char* title) {
    std::cout << "\n=== [layer " << layer << "] " << title
              << "   [" << mt_std_name() << "]\n";
}

// 本段落需要更高语言级别才有的能力：不报错，打印原因后继续
inline void mt_skip(int need_level, const char* what, const char* why) {
    std::cout << "  [SKIP] " << what << " needs " << mt_std_label(need_level)
              << ", current " << mt_std_name() << "\n"
              << "         reason: " << why << "\n";
}

inline void mt_info(const char* msg) { std::cout << "  " << msg << "\n"; }

inline void mt_value(const char* key, long long v) {
    std::cout << "  " << key << " = " << v << "\n";
}

inline void mt_value_d(const char* key, double v) {
    std::cout << "  " << key << " = " << v << "\n";
}

namespace mt {

// ---------------------------------------------------------------------------
// 3. OS 线程包装：回答"std::thread 出现之前，线程从哪来"
//
// 对应主线 B：Process -> Thread -> Scheduler -> Context Switch
// C++98 没有 std::thread，但 OS 一直有线程原语：
//   Windows: CreateThread / WaitForSingleObject
//   POSIX  : pthread_create / pthread_join
// ---------------------------------------------------------------------------
typedef void (*TaskFn)(void*);

struct Entrypoint {
    TaskFn fn;
    void*  arg;
};

inline unsigned long os_tid() {
#if defined(_WIN32)
    return (unsigned long)::GetCurrentThreadId();
#else
    // pthread_t 不保证是整数类型，这里只演示"线程身份可区分"
    return (unsigned long)(std::size_t)pthread_self();
#endif
}

class Thread {
public:
    Thread()
#if defined(_WIN32)
        : handle_(NULL)
#else
        : id_(), started_(false)
#endif
    {}

#if defined(_WIN32)
    ~Thread() { if (handle_ != NULL) ::CloseHandle(handle_); }
#else
    ~Thread() { if (started_) pthread_detach(id_); }
#endif

    bool start(TaskFn fn, void* arg) {
        Entrypoint* ep = new Entrypoint;
        ep->fn  = fn;
        ep->arg = arg;
#if defined(_WIN32)
        handle_ = ::CreateThread(NULL, 0, &Thread::trampoline, ep, 0, NULL);
        if (handle_ == NULL) delete ep;
        return handle_ != NULL;
#else
        started_ = (pthread_create(&id_, NULL, &Thread::trampoline, ep) == 0);
        if (!started_) delete ep;
        return started_;
#endif
    }

    void join() {
#if defined(_WIN32)
        if (handle_ != NULL) {
            ::WaitForSingleObject(handle_, INFINITE);
            ::CloseHandle(handle_);
            handle_ = NULL;
        }
#else
        if (started_) { pthread_join(id_, NULL); started_ = false; }
#endif
    }

    void detach() {
#if defined(_WIN32)
        if (handle_ != NULL) { ::CloseHandle(handle_); handle_ = NULL; }
#else
        if (started_) { pthread_detach(id_); started_ = false; }
#endif
    }

private:
#if defined(_WIN32)
    static DWORD WINAPI trampoline(LPVOID p) {
        Entrypoint* ep = static_cast<Entrypoint*>(p);
        ep->fn(ep->arg);
        delete ep;
        return 0;
    }
    void* handle_;
#else
    static void* trampoline(void* p) {
        Entrypoint* ep = static_cast<Entrypoint*>(p);
        ep->fn(ep->arg);
        delete ep;
        return 0;
    }
    pthread_t id_;
    bool started_;
#endif

    // 线程句柄不复制
    Thread(const Thread&);
    Thread& operator=(const Thread&);
};

inline void sleep_ms(unsigned ms) {
#if defined(_WIN32)
    ::Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec  = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)(ms % 1000u) * 1000000L;
    while (nanosleep(&ts, &ts) == -1) { /* 被信号打断则接着睡 */ }
#endif
}

// ---------------------------------------------------------------------------
// 4. OS 级原子：回答"CAS 在 std::atomic 之前长什么样"
//
// 对应第三节：x86 LOCK CMPXCHG / ARM LDXR+STXR / RISC-V LR+SC
// 编译器把这些做成内置函数，C++98 时代的无锁结构就靠它们手写。
// ---------------------------------------------------------------------------
#if defined(__GNUC__) || defined(__clang__)

#define MT_OS_ATOMIC 1
#define MT_OS_ATOMIC_NAME "__sync builtins (GCC/Clang) -> lock cmpxchg / ldaxr+stlxr"

inline long os_fetch_add(long* p, long delta) {
    return (long)__sync_fetch_and_add(p, (int)delta);
}
inline void* os_cas(void* volatile* slot, void* expected, void* desired) {
    return __sync_val_compare_and_swap(slot, expected, desired);
}
inline long os_cas_long(long* slot, long expected, long desired) {
    return (long)__sync_val_compare_and_swap(slot, (int)expected, (int)desired);
}
// 全屏障：用来对照 memory_order_seq_cst / std::atomic_thread_fence
inline void os_fence() { __sync_synchronize(); }

#elif defined(_MSC_VER)

#define MT_OS_ATOMIC 1
#define MT_OS_ATOMIC_NAME "InterlockedXxx (MSVC) -> lock xadd / lock cmpxchg"

inline long os_fetch_add(long* p, long delta) {
    // 语义对齐 __sync_fetch_and_add / std::atomic::fetch_add：返回「加之前」的旧值。
    // InterlockedExchangeAdd 本来就返回旧值，这里不能再 +delta，
    // 否则 ticket lock 取号会整体偏移 1，第一个线程永远等不到自己的号（挂死）。
    return (long)::InterlockedExchangeAdd((volatile LONG*)p, (LONG)delta);
}
inline void* os_cas(void* volatile* slot, void* expected, void* desired) {
    return ::InterlockedCompareExchangePointer((void* volatile*)slot, desired, expected);
}
inline long os_cas_long(long* slot, long expected, long desired) {
    return (long)::InterlockedCompareExchange((volatile LONG*)slot, (LONG)desired, (LONG)expected);
}
inline void os_fence() { ::MemoryBarrier(); }

#else

#define MT_OS_ATOMIC_NAME "no OS-level atomic intrinsics on this platform"
inline long os_fetch_add(long*, long) { return 0; }
inline void* os_cas(void* volatile*, void*, void*) { return 0; }
inline long os_cas_long(long*, long, long) { return 0; }
inline void os_fence() {}

#endif

// ---------------------------------------------------------------------------
// 5. 单调时钟：性能段落（层 23）测吞吐必须用墙钟，std::clock() 量的是 CPU 时间
// ---------------------------------------------------------------------------
inline double now_ms() {
#if defined(_WIN32)
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (double)cnt.QuadPart * 1000.0 / (double)freq.QuadPart;
#elif defined(CLOCK_MONOTONIC)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0;
#endif
}

// ---------------------------------------------------------------------------
// 6. Cache line 常量
//
// 对应第二节硬件层 + 第二十三节性能树：False Sharing / Cache Line Ping-Pong
// C++17 提供 hardware_destructive_interference_size，但实现不一定给 64。
// ---------------------------------------------------------------------------
#if MT_HAS_17
#include <new>
inline std::size_t cache_line() {
#if defined(__cpp_lib_hardware_interference_size)
    return std::hardware_destructive_interference_size;
#else
    return 64;
#endif
}
#else
inline std::size_t cache_line() { return 64; }
#endif

// alignas 是 C++11 关键字；C++98 下退化成编译器扩展
#if MT_HAS_11
#define MT_ALIGNED64 alignas(64)
#elif defined(__GNUC__)
#define MT_ALIGNED64 __attribute__((aligned(64)))
#elif defined(_MSC_VER)
#define MT_ALIGNED64 __declspec(align(64))
#else
#define MT_ALIGNED64
#endif

inline std::string to_str(long long v) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%lld", v);
    return std::string(buf);
}

// 优化档位标签。内存序/StoreBuffer 这类实验对优化级别极其敏感：
// -O0 下 CPU 的写缓冲窗口被一堆无关指令拉长又填满，乱序现象基本观察不到。
// 所以演示输出必须自带这一行，否则"跑了但没看到"会被误以为结论是假的。
inline const char* mt_opt_name() {
#if defined(_MSC_VER)
#  ifdef _DEBUG
    return "MSVC /Od (unoptimized)";
#  else
    return "MSVC optimized";
#  endif
#elif defined(__OPTIMIZE__)
    return "optimized (-O2)";
#else
    return "UNOPTIMIZED (-O0) -- reorder windows may be invisible";
#endif
}

// 线程参数只能带一个 void*：小整数要塞进去再取出来。
// 注意别用 long 中转 —— Windows 上 long 是 32 位，指针是 64 位，会丢精度。
inline void* to_ptr(std::size_t v) { return reinterpret_cast<void*>(v); }
inline std::size_t from_ptr(void* p) { return reinterpret_cast<std::size_t>(p); }

// ---------------------------------------------------------------------------
// 7. 复用版自旋锁 + RAII 包装（C++98 可用）
//
// mt_03 里有一份"逐行讲清楚"的手写版，这里这份给后续文件复用：
// 只要 MT_OS_ATOMIC 在，C++98/03 分支也能写出真正正确的锁。
// ---------------------------------------------------------------------------
class SpinLock {
public:
    SpinLock() : flag_(0) {}

    void lock() {
        while (os_cas_long(&flag_, 0, 1) != 0) { /* 自旋等待 */ }
    }
    bool try_lock() {
        return os_cas_long(&flag_, 0, 1) == 0;
    }
    void unlock() {
        os_fence();          // 释放语义：临界区的写必须早于 flag 清零被看到
        flag_ = 0;
    }

private:
    long flag_;
    SpinLock(const SpinLock&);
    SpinLock& operator=(const SpinLock&);
};

// C++98 没有 std::lock_guard，但 RAII 模板本来就有
template <typename Lock>
class Guard {
public:
    explicit Guard(Lock& l) : lock_(l) { lock_.lock(); }
    ~Guard() { lock_.unlock(); }
private:
    Lock& lock_;
    Guard(const Guard&);
    Guard& operator=(const Guard&);
};

}  // namespace mt

#endif  // MT_COMMON_H
