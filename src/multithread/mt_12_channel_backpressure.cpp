// mt_12_channel_backpressure.cpp —— Queue / Channel / Flow / 背压
//
// 对应 docs/multy_thread.md：
//   十三、第十一层 Stream/Channel/Reactive（Kotlin Flow / Reactor Flux / AsyncIterator）
//   十四节末尾那条 "Producer 比 Consumer 快怎么办" 的图
//   主线 D：BlockingQueue -> Message Passing -> Channel -> Async Iterator -> Reactive
//
// 三个可跑的层次：
//   A. 无界队列        ：生产者永远不阻塞，内存涨到 OOM —— 出事最晚发现的一种
//   B. 有界 Channel    ：队列满时生产者被挂起，压力自动回传到上游（这就是背压）
//   C. 信用式 Reactive ：消费者 request(n)，生产者最多发 n 个（Reactive Streams 协议）
//
// Go 的 `chan` / Kotlin 的 Channel / Java 的 BlockingQueue / Rx 的 request(n)
// 是同一件事在不同语言里的三种写法，本文件用 C++ 手搓出来。
#include "mt_common.h"

#include <deque>          // C++98 就有 deque，A 段要用它做"无界队列"

#if MT_HAS_11
#include <condition_variable>
#include <mutex>
#include <thread>
#endif

namespace {

const int kItems = 200;
const int kCap   = 8;

long g_consumed = 0, g_sum = 0;
long g_unbounded_peak = 0;       // 无界队列的历史最大长度

// ---------------------------------------------------------------------------
// A) 无界队列：只看它会涨到多高
// ---------------------------------------------------------------------------
std::deque<long> g_unbounded;
mt::SpinLock g_ub_lk;

void ub_producer(void*) {
    for (long i = 1; i <= kItems; ++i) {
        g_ub_lk.lock();
        g_unbounded.push_back(i);
        long sz = (long)g_unbounded.size();
        g_ub_lk.unlock();
        if (sz > g_unbounded_peak) g_unbounded_peak = sz;   // 消费者慢，全堆在这儿
    }
}
void ub_consumer(void*) {
    while (g_consumed < kItems) {
        long v = 0; bool got = false;
        g_ub_lk.lock();
        if (!g_unbounded.empty()) { v = g_unbounded.front(); g_unbounded.pop_front(); got = true; }
        g_ub_lk.unlock();
        if (got) { g_sum += v; ++g_consumed; mt::sleep_ms(1); }   // 故意比生产者慢
        else mt::sleep_ms(1);
    }
}

#if MT_HAS_11
// ---------------------------------------------------------------------------
// B) 有界 Channel：满则阻塞生产者（背压），空则阻塞消费者
// ---------------------------------------------------------------------------
template <typename T>
class Channel {
public:
    explicit Channel(size_t cap) : cap_(cap), closed_(false) {}

    bool send(const T& v) {
        std::unique_lock<std::mutex> lk(m_);
        not_full_.wait(lk, [this] { return q_.size() < cap_ || closed_; });
        if (closed_) return false;
        if (q_.size() >= cap_) ++blocked_;
        q_.push_back(v);
        not_empty_.notify_one();
        return true;
    }

    bool recv(T& out) {
        std::unique_lock<std::mutex> lk(m_);
        not_empty_.wait(lk, [this] { return !q_.empty() || closed_; });
        if (q_.empty()) return false;
        out = q_.front();
        q_.pop_front();
        not_full_.notify_one();
        return true;
    }

    void close() {
        std::lock_guard<std::mutex> lk(m_);
        closed_ = true;
        not_empty_.notify_all();
        not_full_.notify_all();
    }

    long blocked() const { return blocked_; }
    size_t high_water() const { return cap_; }

private:
    std::mutex m_;
    std::condition_variable not_full_, not_empty_;
    std::deque<T> q_;
    size_t cap_;
    bool closed_;
    long blocked_ = 0;
};

Channel<long> g_chan(kCap);
long g_produced = 0;             // 只在有界 channel 一段用到

void chan_producer() {
    for (long i = 1; i <= kItems; ++i) {
        if (!g_chan.send(i)) break;
        ++g_produced;
    }
    g_chan.close();
}

void chan_consumer() {
    long v = 0;
    while (g_chan.recv(v)) { g_sum += v; ++g_consumed; mt::sleep_ms(1); }  // 慢消费者
}
#endif  // MT_HAS_11

// ---------------------------------------------------------------------------
// C) 信用式流（Reactive Streams 的最小骨架）
// ---------------------------------------------------------------------------
struct Credits {
    long n;               // 消费者授予的额度：所有访问都在 lk 里面，不需要 volatile
    mt::SpinLock lk;
};
Credits g_credits;
long g_flow_sent = 0, g_flow_requests = 0;

void flow_producer(void*) {
    long sent = 0;
    while (sent < kItems) {
        // 没有额度就等：这就是 request(n) 协议里生产者的义务
        long have = 0;
        for (;;) {
            g_credits.lk.lock();
            have = g_credits.n;
            g_credits.lk.unlock();
            if (have > 0) break;
            mt::sleep_ms(1);
            ++g_flow_requests;                 // 生产者为额度等待的轮次
        }
        g_credits.lk.lock();
        --g_credits.n;
        g_credits.lk.unlock();
        ++sent;
        ++g_flow_sent;
    }
}

void flow_consumer(void*) {
    long got = 0;
    while (got < kItems) {
        // 每处理完一批再补请求：批量 request(n) 比逐个 request(1) 省开销
        const long batch = 5;
        g_credits.lk.lock();
        g_credits.n += batch;
        g_credits.lk.unlock();
        for (long k = 0; k < batch && got < kItems; ++k) { mt::sleep_ms(1); ++got; }
    }
}

}  // namespace

int main() {
    std::cout << "mt_12_channel_backpressure : what happens when the producer is faster\n";

    // ---- A. 无界 ----
    mt_section(13, "unbounded queue: nothing blocks, memory pays");
    g_unbounded_peak = 0; g_consumed = 0; g_sum = 0;
    {
        mt::Thread p, c;
        p.start(&ub_producer, NULL);
        c.start(&ub_consumer, NULL);
        p.join();
        c.join();
    }
    std::cout << "  peak queue depth = " << g_unbounded_peak << " (cap-free)\n";
    std::cout << "  sum = " << g_sum << ", consumed = " << g_consumed << " (expect 20100/200)\n";
    std::cout << "  生产快消费慢时无界队列不会报错，只会把 OOM 推到几周以后。\n";

#if MT_HAS_11
    // ---- B. 有界 channel ----
    mt_section(13, "bounded channel: producer is throttled by the consumer");
    g_produced = 0; g_consumed = 0; g_sum = 0;
    {
        std::thread p(&chan_producer), c(&chan_consumer);
        p.join(); c.join();
        std::cout << "  produced = " << g_produced << ", consumed = " << g_consumed
                  << ", sum = " << g_sum << "\n";
        std::cout << "  high water mark = " << kCap
                  << ", producer blocked ~" << g_produced / kCap << " round(s)\n";
    }
    std::cout << "  这就是 Go 的 chan、Kotlin 的 Channel、Java 的 ArrayBlockingQueue 的语义。\n";
#else
    mt_skip(11, "mutex + condition_variable channel",
            "C++11 才有；C++98 的等价物是 pthread_mutex/pthread_cond 或直接复用上面 A 段结构");
#endif

    // ---- C. 信用协议 ----
    mt_section(13, "credit-based flow: request(n) instead of buffering everything");
    g_credits.n = 0; g_flow_sent = 0; g_flow_requests = 0;
    {
        mt::Thread p, c;
        p.start(&flow_producer, NULL);
        c.start(&flow_consumer, NULL);
        p.join(); c.join();
    }
    std::cout << "  emitted = " << g_flow_sent << ", credit waits = " << g_flow_requests << "\n";
    std::cout << "  Reactive Streams 只有 4 个方法：onSubscribe/request/cancel/onNext+onComplete，\n"
                 "  核心就一句：下游说能接多少，上游才发多少。\n";

    std::cout << "\ncross-language map (文档第十三节那张定位表):\n"
                 "  一个未来值  : std::future / Java Future / C# Task / JS Promise\n"
                 "  挂起一个计算: co_await / suspend / async\n"
                 "  多个异步值  : Kotlin Flow / Reactor Flux / AsyncIterator\n"
                 "  协程间通信  : Kotlin Channel / Go chan / 本文件的 Channel<long>\n"
                 "  异步流+背压 : Reactive Streams / Flow（本文件的 Credits）\n";
    return 0;
}
