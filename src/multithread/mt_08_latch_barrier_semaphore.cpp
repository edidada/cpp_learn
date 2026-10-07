// mt_08_latch_barrier_semaphore.cpp —— 会合类同步原语：latch / barrier / semaphore
//
// 对应 docs/multy_thread.md 第六节（第五层）表格里的三行：
//   Semaphore  C++20 counting_semaphore   Java Semaphore   Go 常用 channel 代替
//   Barrier    C++20 std::barrier         Java CyclicBarrier
//   Latch      Java CountDownLatch        C++20 std::latch
//
// 三者很容易混，一句话区分：
//   latch     ：一次性门闸。N 个 count_down，等待者等到 0 就放行，之后不再复用。
//   barrier   ：反复用的集合点。N 个线程都到齐，才一起继续，可循环多轮。
//   semaphore ：资源计数。acquire 减、release 加，控制"同时能有几个在里面向导"。
//
// 先看 C++20 之前的手写版（自旋版 C++98 就能跑，阻塞版需要 mutex+cv），
// 再看标准库版本 —— 语义一样，但标准库把 futex/等待队列这些细节做对了。
#include "mt_common.h"

#if MT_HAS_11
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#endif
#if MT_HAS_20
#include <barrier>
#include <latch>
#include <semaphore>
#endif

namespace {

const int kWorkers = 4;
const int kRounds  = 3;

// ---------------------------------------------------------------------------
// 1) 自旋版三件套：C++98 分支也能跑（只用 OS 原子）
// ---------------------------------------------------------------------------
class SpinLatch {
public:
    explicit SpinLatch(long n) : n_(n) {}
    void count_down() { mt::os_fetch_add(&n_, -1); }
    void wait()       { while (mt::os_fetch_add(&n_, 0) > 0) { /* spin */ } }
private:
    long n_;
};

class SpinBarrier {
public:
    explicit SpinBarrier(long n) : total_(n), pending_(n), gen_(0) {}
    void arrive_and_wait() {
        long my_gen = mt::os_fetch_add(&gen_, 0);
        // 最后一个到的人负责翻generation并叫醒大家
        if (mt::os_fetch_add(&pending_, -1) == 1) {
            pending_ = total_;
            mt::os_fence();
            mt::os_cas_long(&gen_, my_gen, my_gen + 1);
        } else {
            while (mt::os_fetch_add(&gen_, 0) == my_gen) { /* spin */ }
        }
    }
private:
    long total_, pending_, gen_;
};

class SpinSemaphore {
public:
    explicit SpinSemaphore(long permits) : permits_(permits) {}
    void acquire() {
        for (;;) {
            long cur = mt::os_fetch_add(&permits_, 0);
            if (cur > 0 && mt::os_cas_long(&permits_, cur, cur - 1) == cur) return;
        }
    }
    void release() { mt::os_fetch_add(&permits_, 1); }
private:
    long permits_;
};

long g_latch_seen = 0;
// latch 本身要通过参数传进来，否则 worker 只会 +1、没人 count_down，
// 主线程就会永远等在 wait() 里（实测：真的会挂住，这一行就是那个教训）。
void spin_worker(void* p) {
    mt::os_fetch_add(&g_latch_seen, 1);
    static_cast<SpinLatch*>(p)->count_down();
}

SpinBarrier g_barrier(kWorkers);
long g_barrier_round_ok = 0;
void barrier_worker(void*) {
    for (int r = 0; r < kRounds; ++r) {
        mt::os_fetch_add(&g_barrier_round_ok, 1);
        g_barrier.arrive_and_wait();      // 到齐才能进下一轮
    }
}

SpinSemaphore g_semaphore(2);             // 最多 2 个并发
long g_in_flight = 0, g_max_in_flight = 0;
void semaphore_worker(void*) {
    for (int i = 0; i < 50; ++i) {
        g_semaphore.acquire();
        long cur = mt::os_fetch_add(&g_in_flight, 1) + 1;
        // 记录历史最大并发度，用来验证限流确实生效
        long old = mt::os_fetch_add(&g_max_in_flight, 0);
        if (cur > old) mt::os_cas_long(&g_max_in_flight, old, cur);
        mt::sleep_ms(1);
        mt::os_fetch_add(&g_in_flight, -1);
        g_semaphore.release();
    }
}

#if MT_HAS_11
// ---------------------------------------------------------------------------
// 2) 阻塞版（mutex + condvar）：不烧 CPU，这是"生产级"的写法
// ---------------------------------------------------------------------------
class CvLatch {
public:
    explicit CvLatch(long n) : n_(n) {}
    void count_down() {
        std::lock_guard<std::mutex> lk(m_);
        if (--n_ == 0) cv_.notify_all();
    }
    void wait() {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this] { return n_ == 0; });
    }
private:
    std::mutex m_;
    std::condition_variable cv_;
    long n_;
};

class CvSemaphore {
public:
    explicit CvSemaphore(long p) : permits_(p) {}
    void acquire() {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this] { return permits_ > 0; });
        --permits_;
    }
    void release() {
        std::lock_guard<std::mutex> lk(m_);
        ++permits_;
        cv_.notify_one();
    }
private:
    std::mutex m_;
    std::condition_variable cv_;
    long permits_;
};

void cv_latch_worker(CvLatch* l, long* seen) {
    mt::os_fetch_add(seen, 1);
    l->count_down();
}
#endif  // MT_HAS_11

#if MT_HAS_20 && defined(__cpp_lib_latch)
// ---------------------------------------------------------------------------
// 3) C++20 标准库版
// ---------------------------------------------------------------------------
std::counting_semaphore<4> g_std_sem(2);
std::atomic<long> g_std_in(0), g_std_max_in(0);
void std_sem_worker() {
    for (int i = 0; i < 30; ++i) {
        g_std_sem.acquire();
        long cur = g_std_in.fetch_add(1) + 1;
        long old = g_std_max_in.load();
        while (cur > old && !g_std_max_in.compare_exchange_weak(old, cur)) { }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        g_std_in.fetch_sub(1);
        g_std_sem.release();
    }
}
void std_latch_worker(std::latch* l) { l->count_down(); }

// std::barrier 带 completion function：最后一个到达者会在"所有人被释放之前"执行它，
// 这是 Java CyclicBarrier 的 barrierAction，也是做每轮归约的地方。
void std_barrier_worker(std::barrier<std::function<void()>>* b) {
    for (int r = 0; r < kRounds; ++r) b->arrive_and_wait();
}
#endif  // MT_HAS_20

}  // namespace

int main() {
    std::cout << "mt_08_latch_barrier_semaphore : rendezvous primitives\n";

    // ---- 1. 自旋版 ----
    mt_section(5, "hand-rolled spin versions (runnable already at C++98)");
    {
        SpinLatch l(kWorkers);
        mt::Thread ts[kWorkers];
        g_latch_seen = 0;
        for (int i = 0; i < kWorkers; ++i) ts[i].start(&spin_worker, &l);
        l.wait();                        // 等 4 个 worker 都 count_down
        for (int i = 0; i < kWorkers; ++i) ts[i].join();
        std::cout << "  latch opened after " << g_latch_seen << " count_downs\n";
    }
    {
        g_barrier_round_ok = 0;
        mt::Thread ts[kWorkers];
        for (int i = 0; i < kWorkers; ++i) ts[i].start(&barrier_worker, NULL);
        for (int i = 0; i < kWorkers; ++i) ts[i].join();
        std::cout << "  barrier rounds: total arrivals = " << g_barrier_round_ok
                  << " (expect " << (kWorkers * kRounds) << ")\n";
    }
    {
        mt::Thread ts[kWorkers];
        for (int i = 0; i < kWorkers; ++i) ts[i].start(&semaphore_worker, NULL);
        for (int i = 0; i < kWorkers; ++i) ts[i].join();
        std::cout << "  semaphore permits=2, observed max concurrency = "
                  << g_max_in_flight << " (must be <= 2)\n";
    }
    std::cout << "  代价写在脸上：等待期间线程一直占着核。这就是要 condvar/futex 的原因。\n";

#if MT_HAS_11
    // ---- 2. 阻塞版 ----
    mt_section(5, "mutex + condition_variable versions: block instead of burn");
    {
        CvLatch l(kWorkers);
        std::vector<std::thread> v;
        g_latch_seen = 0;
        for (int i = 0; i < kWorkers; ++i) {
            v.push_back(std::thread(&cv_latch_worker, &l, &g_latch_seen));
        }
        l.wait();
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  cv latch opened after " << g_latch_seen << " count_downs\n";

        CvSemaphore s(2);
        long in_now = 0, in_max = 0;
        std::vector<std::thread> w;
        for (int t = 0; t < kWorkers; ++t) {
            w.push_back(std::thread([&] {
                for (int i = 0; i < 30; ++i) {
                    s.acquire();
                    long cur = mt::os_fetch_add(&in_now, 1) + 1;
                    long old = mt::os_fetch_add(&in_max, 0);
                    if (cur > old) mt::os_cas_long(&in_max, old, cur);
                    mt::sleep_ms(1);
                    mt::os_fetch_add(&in_now, -1);
                    s.release();
                }
            }));
        }
        for (int i = 0; i < (int)w.size(); ++i) w[i].join();
        std::cout << "  cv semaphore max concurrency = " << in_max << " (must be <= 2)\n";
    }
#else
    mt_skip(11, "mutex/condition_variable versions",
            "C++11 才有标准 mutex/cv；当时的答案是 pthread_mutex_t + pthread_cond_t");
#endif

#if MT_HAS_20 && defined(__cpp_lib_latch)
    // ---- 3. 标准库版 ----
    mt_section(5, "C++20: std::latch / std::barrier / std::counting_semaphore");
    {
        std::latch l(kWorkers);
        std::vector<std::thread> v;
        for (int i = 0; i < kWorkers; ++i) v.push_back(std::thread(&std_latch_worker, &l));
        l.wait();
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  std::latch opened; try_wait() = " << (l.try_wait() ? "true" : "false") << "\n";
    }
    {
        long reduced = 0;
        std::barrier<std::function<void()>> b(kWorkers, [&reduced] { ++reduced; });
        std::vector<std::thread> v;
        for (int i = 0; i < kWorkers; ++i) v.push_back(std::thread(&std_barrier_worker, &b));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  std::barrier completion fn ran " << reduced
                  << " time(s) (= " << kRounds << " rounds)\n";
    }
    {
        g_std_max_in.store(0); g_std_in.store(0);
        std::vector<std::thread> v;
        for (int i = 0; i < kWorkers; ++i) v.push_back(std::thread(&std_sem_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  std::counting_semaphore max concurrency = "
                  << g_std_max_in.load() << " (must be <= 2)\n";
    }
    std::cout << "  try_acquire_for()/barrier::arrive()（不等待）这些细节，标准库都补齐了。\n";
#else
    mt_skip(20, "std::latch / std::barrier / std::counting_semaphore",
            "P0431/P0883/R0996 到 C++20 才合并进标准库；上面手写版就是它们的语义说明");
#endif
    return 0;
}
