// mt_09_thread_pool.cpp —— 从 Thread 到 Thread Pool：主线 B 的终点之一
//
// 对应 docs/multy_thread.md：
//   五、第四层 OS —— Context Switch / Scheduler overhead / Oversubscription
//   六、第五层 —— "线程池" 是把线程当资源管理的工程答案
//   表格：Java Executor / C++ library / Rust runtime / Go runtime
//
// 三个版本按"演进"排：
//   C++98 ：mt::Thread + 自旋锁 + 轮询       —— 能跑，但等待靠 sleep
//   C++11 ：std::thread + condvar + packaged_task -> 提交任务拿 future
//   C++20 ：std::jthread + stop_token          —— 自动 join + 协作式取消
//
// 为什么要有池：thread-per-task 的成本是 创建/销毁 + 调度 + 缓存污染 + 内核记账，
// 10 万连接 thread-per-connection 扛不住，这正是协程/事件循环出现的前提。
#include "mt_common.h"

#if MT_HAS_11
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#endif
#if MT_HAS_20
#include <stop_token>
#include <thread>
#endif

namespace {

const int kPoolSize = 4;
const int kJobs     = 200;

long g_done_98 = 0;

// ---------------------------------------------------------------------------
// 1) C++98 版池：固定线程数 + 数组环形队列 + 自旋锁 + 轮询
// ---------------------------------------------------------------------------
typedef void (*JobFn)(void*);
struct Job { JobFn fn; void* arg; };

class Pool98 {
public:
    Pool98(int n) : head_(0), tail_(0), count_(0), stop_(0), n_(n) {
        workers_ = new mt::Thread[n];
    }
    ~Pool98() { delete[] workers_; }

    void start() {
        for (int i = 0; i < n_; ++i) workers_[i].start(&worker_main, this);
    }

    void submit(JobFn fn, void* arg) {
        for (;;) {
            lk_.lock();
            if (count_ < kQueueCap) {
                ring_[tail_] = make_job(fn, arg);
                tail_ = (tail_ + 1) % kQueueCap;
                ++count_;
                lk_.unlock();
                return;
            }
            lk_.unlock();
            mt::sleep_ms(1);        // 队列满：提交方也被限流，这就是最朴素的背压
        }
    }

    void shutdown() {
        stop_ = 1;                  // 先让队列排空，再置停止位
        for (int i = 0; i < n_; ++i) workers_[i].join();
    }

private:
    static const int kQueueCap = 128;

    Job make_job(JobFn fn, void* arg) { Job j; j.fn = fn; j.arg = arg; return j; }

    bool pop(Job& out) {
        lk_.lock();
        if (count_ == 0) { lk_.unlock(); return false; }
        out = ring_[head_];
        head_ = (head_ + 1) % kQueueCap;
        --count_;
        lk_.unlock();
        return true;
    }

    static void worker_main(void* p) {
        Pool98* self = static_cast<Pool98*>(p);
        for (;;) {
            Job j;
            if (self->pop(j)) {
                j.fn(j.arg);
            } else if (self->stop_) {
                if (!self->pop(j)) return;       // 空了就退出
                j.fn(j.arg);
            } else {
                mt::sleep_ms(1);                 // 没活干就睡一会儿：这就是"轮询式"调度
            }
        }
    }

    mt::SpinLock lk_;
    Job ring_[kQueueCap];
    int head_, tail_, count_;
    volatile int stop_;
    mt::Thread* workers_;
    int n_;
    Pool98(const Pool98&);
    Pool98& operator=(const Pool98&);
};

void job_98(void*) { mt::os_fetch_add(&g_done_98, 1); }

#if MT_HAS_11
// ---------------------------------------------------------------------------
// 2) C++11 版池：condvar 阻塞等待（不烧 CPU），任务用 packaged_task -> future
// ---------------------------------------------------------------------------
class ThreadPool {
public:
    explicit ThreadPool(int n) : stop_(false) {
        for (int i = 0; i < n; ++i) workers_.push_back(std::thread(&ThreadPool::loop, this));
    }

    ~ThreadPool() {
        { std::lock_guard<std::mutex> lk(m_); stop_ = true; }
        cv_.notify_all();
        for (size_t i = 0; i < workers_.size(); ++i) workers_[i].join();
    }

    template <typename F>
    std::future<long> submit(F f) {
        auto task = std::make_shared<std::packaged_task<long()>>(f);
        std::future<long> fut = task->get_future();
        {
            std::lock_guard<std::mutex> lk(m_);
            queue_.push([task] { (*task)(); });   // 把任务包成可调用对象塞进队列
        }
        cv_.notify_one();
        return fut;
    }

private:
    void loop() {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lk(m_);
                cv_.wait(lk, [this] { return stop_ || !queue_.empty(); });
                if (stop_ && queue_.empty()) return;
                job = std::move(queue_.front());
                queue_.pop();
            }
            job();
        }
    }

    std::mutex m_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> queue_;
    std::vector<std::thread> workers_;
    bool stop_;
};

long heavy(long x) { long s = 0; for (long i = 0; i < 2000; ++i) s += x; return s; }
#endif  // MT_HAS_11

#if MT_HAS_20
// ---------------------------------------------------------------------------
// 3) C++20 版池：jthread 自动 join + stop_token 协作式取消
// ---------------------------------------------------------------------------
#if MT_HAS_11
struct Pool20 {
    Pool20() : stop_(false) {}
    std::mutex m_;
    std::condition_variable_any cv_;
    std::queue<std::function<void()> > q_;
    std::atomic<bool> stop_;
};

void jworker(std::stop_token st, Pool20* p) {
    // 关键一步：把"取消请求"翻译成"叫醒等待者"。
    // 没有这个回调，request_stop() 之后阻塞在 cv 上的线程永远不会醒。
    std::stop_callback cb(st, [p] { p->stop_.store(true); p->cv_.notify_all(); });

    for (;;) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lk(p->m_);
            p->cv_.wait(lk, [&] { return p->stop_.load() || !p->q_.empty(); });
            if (p->stop_.load() && p->q_.empty()) return;   // 排空后再退出，不丢任务
            job = std::move(p->q_.front());
            p->q_.pop();
        }
        if (job) job();
    }
}
#endif  // MT_HAS_11
#endif  // MT_HAS_20

}  // namespace

int main() {
    std::cout << "mt_09_thread_pool : threads are a resource, manage them\n";

    // ---- 1. C++98 ----
    mt_section(4, "pool with OS threads + spin lock + polling (no std::thread needed)");
    g_done_98 = 0;
    {
        Pool98 pool(kPoolSize);
        pool.start();
        double t0 = mt::now_ms();
        for (int i = 0; i < kJobs; ++i) pool.submit(&job_98, NULL);
        pool.shutdown();
        std::cout << "  jobs done = " << g_done_98 << "/" << kJobs
                  << " in " << (mt::now_ms() - t0) << " ms\n";
    }
    std::cout << "  注意 shutdown 的顺序：先排空队列再退出，否则丢任务。\n";

#if MT_HAS_11
    // ---- 2. C++11 ----
    mt_section(8, "std::thread pool: submit() returns a future (task, not thread)");
    {
        ThreadPool pool(kPoolSize);
        std::vector<std::future<long>> futs;
        double t0 = mt::now_ms();
        for (int i = 0; i < 8; ++i) futs.push_back(pool.submit([i] { return heavy(i + 1); }));
        long total = 0;
        for (size_t i = 0; i < futs.size(); ++i) total += futs[i].get();
        std::cout << "  8 tasks over " << kPoolSize << " workers, sum = " << total
                  << ", " << (mt::now_ms() - t0) << " ms\n";
        std::cout << "  调用方拿到的是 future：它关心结果，不关心是哪个线程算的。\n";
    }
#else
    mt_skip(11, "std::thread + packaged_task thread pool",
            "C++11 才有 thread/future；工程里当时用 boost::thread_pool 或自研，如上面第 1 段");
#endif

#if MT_HAS_20
    // ---- 3. C++20 ----
    mt_section(4, "std::jthread + std::stop_token: 自动 join、可取消");
    {
#if MT_HAS_11
        Pool20 st;
        long done = 0;
        {
            std::vector<std::jthread> v;
            for (int i = 0; i < kPoolSize; ++i) v.push_back(std::jthread(&jworker, &st));
            for (int i = 0; i < kJobs; ++i) {
                { std::lock_guard<std::mutex> lk(st.m_); st.q_.push([&done] { ++done; }); }
                st.cv_.notify_one();
            }
            // 作用域结束：jthread 析构自动 request_stop() + join()，不会忘了 join 而 terminate
        }
        std::cout << "  jthread pool drained " << done << "/" << kJobs
                  << " jobs, no explicit join anywhere\n";
        std::cout << "  stop_token 是协作式取消：任务自己决定在哪个点让路，见第 14 节结构化并发。\n";
#endif
    }
#else
    mt_skip(20, "std::jthread / stop_token",
            "C++20 (P2320/P0660) 才进标准库；在此之前要么手动 join，要么 detach 后失控");
#endif

    std::cout << "\n为什么不用 10 万个线程（文档第五节）：\n"
                 "  - 每个内核线程都要栈 + 内核记账\n"
                 "  - 调度器切换成本、缓存污染、CPU migration\n"
                 "  - oversubscription： runnable 队列长度远超核数\n"
                 "  -> 答案分层：线程池（复用）/ 事件循环（少线程）/ 协程（用户态调度）\n";
    return 0;
}
