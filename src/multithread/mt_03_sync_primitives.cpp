// mt_03_sync_primitives.cpp —— 传统同步原语：从自旋锁到条件变量
//
// 对应 docs/multy_thread.md：
//   六、第五层 传统同步原语（Mutex / Spinlock / RWLock / Condition Variable）
//   五、第四层 OS —— 阻塞的线程会被调度器拿走，自旋的线程一直占着 CPU
//
// 演进链（这才是这一层真正的知识）：
//   spin lock (CAS 自旋)              C++98 就能写，代价是空转烧 CPU
//     -> mutex/condvar (futex)        阻塞线程让出 CPU，代价是上下文切换
//     -> lock_guard/unique_lock       RAII，异常安全
//     -> scoped_lock / shared_mutex   C++17，多锁一次拿到 + 读写分离
#include "mt_common.h"

#if MT_HAS_11
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#endif
#if MT_HAS_17
#include <shared_mutex>
#endif

namespace {

const int kCapacity = 8;
const int kItems    = 20;

// ---------------------------------------------------------------------------
// 1) C++98 版：手写自旋锁 + 忙等缓冲区
// ---------------------------------------------------------------------------
class SpinLock {
public:
    SpinLock() : flag_(0) {}
    void lock() {
        // 拿到锁的人把 1 写回去；没拿到就一直转。这就是"自旋"。
        while (mt::os_cas_long(&flag_, 0, 1) != 0) { /* busy wait */ }
    }
    void unlock() {
        // 释放必须是原子的写，并且要有释放语义屏障，否则临界区里的写可能后于 unlock 被看到
        mt::os_fence();
        flag_ = 0;
    }
private:
    long flag_;
    SpinLock(const SpinLock&);
    SpinLock& operator=(const SpinLock&);
};

long g_ring[kCapacity];
int  g_head = 0;   // 取走位置
int  g_tail = 0;   // 放入位置
int  g_count = 0;
SpinLock g_spin;

void spin_producer(void*) {
    for (int i = 1; i <= kItems; ++i) {
        for (;;) {
            g_spin.lock();
            if (g_count < kCapacity) {
                g_ring[g_tail] = i;
                g_tail = (g_tail + 1) % kCapacity;
                ++g_count;
                g_spin.unlock();
                break;
            }
            g_spin.unlock();
            // 队列满：只能自己再转。真实系统里这是 CPU 浪费，正是 condvar 存在的理由。
        }
    }
    g_spin.lock();
    g_ring[g_tail] = -1;                 // 结束哨兵
    g_tail = (g_tail + 1) % kCapacity;
    ++g_count;
    g_spin.unlock();
}

void spin_consumer(void*) {
    long sum = 0;
    int got = 0;
    for (;;) {
        int have = 0, item = 0;
        g_spin.lock();
        if (g_count > 0) {
            item = (int)g_ring[g_head];
            g_head = (g_head + 1) % kCapacity;
            --g_count;
            have = 1;
        }
        g_spin.unlock();
        if (!have) continue;             // 空了就再转
        if (item < 0) break;
        sum += item;
        ++got;
    }
    std::cout << "  [spinlock + busy wait] consumed " << got
              << " items, sum = " << sum << " (expect 210)\n";
}

#if MT_HAS_11
// ---------------------------------------------------------------------------
// 2) C++11 版：mutex + condition_variable，满/空都挂起线程
// ---------------------------------------------------------------------------
class Buffer {
public:
    Buffer() : head_(0), tail_(0), count_(0), closed_(false) {}

    void put(int v) {
        std::unique_lock<std::mutex> lk(m_);
        not_full_.wait(lk, [this] { return count_ < kCapacity || closed_; });
        if (closed_) return;
        ring_[tail_] = v;
        tail_ = (tail_ + 1) % kCapacity;
        ++count_;
        not_empty_.notify_one();
    }

    bool get(int& out) {
        std::unique_lock<std::mutex> lk(m_);
        not_empty_.wait(lk, [this] { return count_ > 0 || closed_; });
        if (count_ == 0) return false;          // closed 且已排空
        out = ring_[head_];
        head_ = (head_ + 1) % kCapacity;
        --count_;
        not_full_.notify_one();
        return true;
    }

    void close() {
        std::lock_guard<std::mutex> lk(m_);
        closed_ = true;
        not_empty_.notify_all();
        not_full_.notify_all();
    }

private:
    std::mutex m_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    int ring_[kCapacity];
    int head_, tail_, count_;
    bool closed_;
};

// 用 wait(lk, predicate) 而不是 if (empty) wait(lk)：
//   - 虚假唤醒（spurious wakeup）
//   - notify 之后、被唤醒之前，条件可能又被别的消费者抢走（丢失唤醒）
// predicate 版本内部就是 while(!pred) wait()，等价于手写循环但更不容易写错。
Buffer g_buf;

void cv_producer() {
    for (int i = 1; i <= kItems; ++i) g_buf.put(i);
    g_buf.close();
}

void cv_consumer() {
    long sum = 0;
    int got = 0, v = 0;
    while (g_buf.get(v)) { sum += v; ++got; }
    std::cout << "  [mutex + condvar] consumed " << got
              << " items, sum = " << sum << " (expect 210)\n";
}
#endif  // MT_HAS_11

#if MT_HAS_17
// ---------------------------------------------------------------------------
// 3) C++17：scoped_lock 一次锁多个 + shared_mutex 读写并发
// ---------------------------------------------------------------------------
class Account {
public:
    Account(long b) : balance_(b) {}
    long balance() const { return balance_; }
    void add(long d) { balance_ += d; }
    std::mutex& m() { return m_; }
private:
    std::mutex m_;
    long balance_;
};

void transfer(Account& from, Account& to, long amount) {
    // std::lock 内部做死锁避免（先试后回溯），scoped_lock 接管 RAII
    std::scoped_lock lk(from.m(), to.m());
    from.add(-amount);
    to.add(amount);
}

std::shared_mutex rw_;
long read_hits_ = 0;

void reader_loop() {
    for (int i = 0; i < 1000; ++i) {
        std::shared_lock<std::shared_mutex> lk(rw_);   // 共享锁：多个读者可同时进
        read_hits_ += 1;
    }
}
void writer_loop() {
    for (int i = 0; i < 200; ++i) {
        std::unique_lock<std::shared_mutex> lk(rw_);   // 独占锁
        mt::sleep_ms(0);
    }
}
#endif  // MT_HAS_17

}  // namespace

int main() {
    std::cout << "mt_03_sync_primitives : spinlock -> mutex/condvar -> scoped_lock\n";

    mt_section(5, "hand-rolled spin lock (C++98 capable, no OS mutex needed)");
    g_head = g_tail = g_count = 0;
    {
        mt::Thread c, p;
        c.start(&spin_consumer, NULL);
        p.start(&spin_producer, NULL);
        p.join();
        c.join();
    }
    std::cout << "  cost: 空转的线程仍然占着核，调度器不会替你省钱\n";

#if MT_HAS_11
    mt_section(5, "std::mutex + std::condition_variable (block instead of spin)");
    {
        std::thread c(&cv_consumer), p(&cv_producer);
        c.join(); p.join();
    }
    std::cout << "  mutex 底层通常是 futex：无竞争时一条原子指令搞定，\n"
                 "  有竞争才陷入内核挂起线程 —— 这就是 主线 B 里 Mutex -> Futex 那一节。\n";
#else
    mt_skip(11, "std::mutex / std::condition_variable",
            "C++11 才标准化；当时的现实答案是 pthread_mutex_t / PTHREAD mutex");
#endif

#if MT_HAS_17
    mt_section(5, "std::scoped_lock (multi-lock, deadlock-free) + std::shared_mutex");
    {
        Account a(1000), b(0);
        std::vector<std::thread> v;
        for (int i = 0; i < 4; ++i) v.push_back(std::thread(&transfer, std::ref(a), std::ref(b), 100));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  after 4 transfers a=" << a.balance() << " b=" << b.balance()
                  << " sum=" << (a.balance() + b.balance()) << " (must stay 1000)\n";
    }
    {
        std::thread r1(&reader_loop), r2(&reader_loop), w(&writer_loop);
        r1.join(); r2.join(); w.join();
        std::cout << "  shared_mutex: readers ran concurrently, read_hits = "
                  << read_hits_ << " (unsynchronized counter, demo only)\n";
    }
#else
    mt_skip(17, "std::scoped_lock / std::shared_mutex",
            "C++17 提供多锁一次性获取与标准读写锁；在此之前是 pthread_rwlock_t 或 boost");
#endif
    return 0;
}
