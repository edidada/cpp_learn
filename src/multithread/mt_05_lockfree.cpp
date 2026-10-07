// mt_05_lockfree.cpp —— Atomic / Lock-Free：CAS 之上能盖什么楼
//
// 对应 docs/multy_thread.md 第八节（第七层）：
//   Read / Write / RMW / CAS / FAA / SWAP
//   Spin Lock -> Lock-Free Stack -> Lock-Free Queue -> Ring Buffer
//   ABA / Hazard Pointer / Epoch / RCU / Tagged Pointer
//   blocking vs non-blocking；lock-free vs wait-free vs obstruction-free
//
// 这一层的重点是：无锁不是"没有锁"，而是一条严格的**系统进展性质**
//   lock-free    ：至少有一个线程能在有限步内前进（整体有进展）
//   wait-free    ：每个线程自己都能在有限步内前进（最硬，实时系统要这个）
//   obstruction-free：只有单独跑时才前进
#include "mt_common.h"

#if MT_HAS_11
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#endif

namespace {

// ---------------------------------------------------------------------------
// 1) Treiber 栈：只用 CAS 的无锁栈（C++98 版，用 OS 级 CAS）
// ---------------------------------------------------------------------------
struct Node {
    long  value;
    Node* next;
};

Node  g_pool[16];            // 固定节点池：地址能被复用，正好用来重现 ABA
void* volatile g_head = 0;   // C++ 原子指针的裸版
long  g_push_ops = 0, g_pop_ops = 0, g_cas_retries = 0;

void treiber_push(void* v) {
    Node* n = static_cast<Node*>(v);
    Node* expect = static_cast<Node*>(g_head);
    for (;;) {
        n->next = expect;
        void* got = mt::os_cas(&g_head, expect, n);
        if (got == expect) { mt::os_fetch_add(&g_push_ops, 1); return; }
        expect = static_cast<Node*>(got);       // 别人插了一次，重读再试
        mt::os_fetch_add(&g_cas_retries, 1);
    }
}

void treiber_pop(void*) {
    Node* expect = static_cast<Node*>(g_head);
    for (;;) {
        if (expect == 0) return;
        void* got = mt::os_cas(&g_head, expect, expect->next);
        if (got == expect) { mt::os_fetch_add(&g_pop_ops, 1); return; }
        expect = static_cast<Node*>(got);
        mt::os_fetch_add(&g_cas_retries, 1);
    }
}

// ---------------------------------------------------------------------------
// 2) SPSC 环形缓冲：真正的 wait-free（单生产单消费，各自只写自己的下标）
// ---------------------------------------------------------------------------
const int kRingSize = 1024;   // 2 的幂，方便取模
long  g_ring[kRingSize];
volatile long g_rb_head = 0;  // 只有消费者写
volatile long g_rb_tail = 0;  // 只有生产者写

bool rb_push(long v) {
    long t = g_rb_tail;
    long n = (t + 1) & (kRingSize - 1);
    if (n == g_rb_head) return false;         // 满
    g_ring[t] = v;
    mt::os_fence();                           // release：先写数据，再发布下标
    g_rb_tail = n;
    return true;
}

bool rb_pop(long& v) {
    long h = g_rb_head;
    if (h == g_rb_tail) return false;         // 空
    v = g_ring[h];
    mt::os_fence();                           // acquire：先读数据，再更新下标
    g_rb_head = (h + 1) & (kRingSize - 1);
    return true;
}

void spsc_producer(void*) {
    for (long i = 0; i < 200000; ) {
        if (rb_push(i)) { ++i; }               // 满了就重试：SPSC 环的 push 是 wait-free
    }
}
void spsc_consumer(void*) {
    long seen = 0, expect = 0;
    while (expect < 200000) {
        long v = 0;
        if (rb_pop(v)) { if (v != expect) ++seen; ++expect; }   // seen: 顺序错乱计数
    }
    std::cout << "  SPSC received " << expect << " items, out-of-order = " << seen << "\n";
}

#if MT_HAS_11
// ---------------------------------------------------------------------------
// 3) C++11 版 CAS：compare_exchange_strong / weak
// ---------------------------------------------------------------------------
std::atomic<long> g_max_value(0);
std::atomic<Node*> g_std_head(0);
std::atomic<long> g_std_pushes(0), g_std_pops(0), g_std_retries(0);

// 同一个 Treiber 栈，换成语言级原子：逻辑一模一样，写法更干净，
// 而且 compare_exchange 失败时会自动把当前值回填给 expected，省一次 load。
void std_push(Node* n) {
    Node* expect = g_std_head.load(std::memory_order_relaxed);
    for (;;) {
        n->next = expect;
        if (g_std_head.compare_exchange_weak(expect, n,
                                            std::memory_order_release,
                                            std::memory_order_relaxed)) {
            g_std_pushes.fetch_add(1, std::memory_order_relaxed);
            return;
        }
        g_std_retries.fetch_add(1, std::memory_order_relaxed);
    }
}

Node* std_pop() {
    Node* expect = g_std_head.load(std::memory_order_relaxed);
    for (;;) {
        if (expect == 0) return 0;
        Node* next = expect->next;
        if (g_std_head.compare_exchange_weak(expect, next,
                                            std::memory_order_acq_rel,
                                            std::memory_order_relaxed)) {
            g_std_pops.fetch_add(1, std::memory_order_relaxed);
            return expect;                 // 注意：这里不敢 delete，见回收问题
        }
        g_std_retries.fetch_add(1, std::memory_order_relaxed);
    }
}

void std_push_worker() { for (int i = 0; i < 1000; ++i) std_push(&g_pool[i & 15]); }
void std_pop_worker()  { for (int i = 0; i < 1000; ++i) { if (std_pop() == 0) break; } }

// CAS 循环实现一个无锁 max()：这是 "RMW 之外的任意原子更新" 的通用套路
void cas_max(long candidate) {
    long old = g_max_value.load(std::memory_order_relaxed);
    while (candidate > old) {
        // weak 允许伪失败，自旋循环里更快（尤其 LL/SC 机器：ARM 的 STXR 本来就会伪失败）
        if (g_max_value.compare_exchange_weak(old, candidate,
                                              std::memory_order_acq_rel,
                                              std::memory_order_relaxed)) {
            break;
        }
        // 失败时 old 已被更新成当前值，直接进下一轮
    }
}

// ABA：把 (index, tag) 打包进一个 64 位句柄，tag 每次修改都 +1，ABA 就被 CAS 识破
struct Packed {
    static unsigned long long pack(int idx, unsigned int tag) {
        return (unsigned long long)((unsigned long long)tag << 32) | (unsigned long long)(unsigned)idx;
    }
    static int idx(unsigned long long h) { return (int)(h & 0xFFFFFFFFull); }
    static unsigned int tag(unsigned long long h) { return (unsigned int)(h >> 32); }
};

std::atomic<unsigned long long> g_free_list(Packed::pack(0, 0));

void aba_demo() {
    // 场景：线程 A 想 pop，读到 head=idx0(tag7)；此时线程 B pop 了 0、又 push 回 0，
    // head 仍指向 idx0，tag 变成 8。裸指针 CAS 会成功（值一模一样）-> ABA。
    unsigned long long snapshot = g_free_list.load();
    std::cout << "  snapshot handle = (idx " << Packed::idx(snapshot)
              << ", tag " << Packed::tag(snapshot) << ")\n";

    // B 的一轮"绕回来"
    unsigned long long h = g_free_list.load();
    g_free_list.store(Packed::pack(Packed::idx(h), Packed::tag(h) + 1));
    h = g_free_list.load();
    g_free_list.store(Packed::pack(0, Packed::tag(h) + 1));   // 同一个 idx 回来

    unsigned long long now = g_free_list.load();
    bool plain_ptr_cas_would_pass = (Packed::idx(now) == Packed::idx(snapshot));
    bool tagged_cas_passes = g_free_list.compare_exchange_strong(snapshot, now);
    std::cout << "  same index again? " << (plain_ptr_cas_would_pass ? "yes" : "no")
              << "  -> raw pointer CAS would accept it\n";
    std::cout << "  tagged CAS accepted? " << (tagged_cas_passes ? "yes" : "no")
              << "  -> tag 让它变成 no，这就是 tagged pointer / versioned CAS\n";
}

// atomic_flag：标准库里最小的自旋锁原料
std::atomic_flag g_flag = ATOMIC_FLAG_INIT;
long g_flag_hits = 0;

void flag_worker() {
    for (int i = 0; i < 100000; ++i) {
        while (g_flag.test_and_set(std::memory_order_acquire)) { }
        ++g_flag_hits;
        g_flag.clear(std::memory_order_release);
    }
}

std::once_flag g_once;
long g_once_runs = 0;
void init_once() { ++g_once_runs; }
void once_worker() { std::call_once(g_once, init_once); }
#endif  // MT_HAS_11

}  // namespace

int main() {
    std::cout << "mt_05_lockfree : CAS is the primitive everything else is built from\n";

    // ---- 1. 无锁栈 ----
    mt_section(7, "Treiber stack: push/pop built from nothing but CAS");
    for (int i = 0; i < 16; ++i) { g_pool[i].value = i; g_pool[i].next = 0; }
    g_head = 0; g_push_ops = g_pop_ops = g_cas_retries = 0;
    {
        mt::Thread ts[4];
        for (int i = 0; i < 4; ++i) ts[i].start(&treiber_push, &g_pool[i]);
        for (int i = 0; i < 4; ++i) ts[i].join();
        mt::Thread ps[4];
        for (int i = 0; i < 4; ++i) ps[i].start(&treiber_pop, NULL);
        for (int i = 0; i < 4; ++i) ps[i].join();
    }
    std::cout << "  pushes = " << g_push_ops << " pops = " << g_pop_ops
              << " cas retries = " << g_cas_retries << "\n";
    std::cout << "  retries 就是 lock-free 的代价：整体一定有进展，但单个线程可能反复失败。\n";
    std::cout << "  真正难的是回收：pop 之后立刻 delete，别的线程可能还拿着这个指针。\n"
                 "  所以本演示用的是固定节点池，不做释放（Hazard Pointer / Epoch / RCU 是这一节的续集）。\n";

    // ---- 2. wait-free SPSC ----
    mt_section(7, "SPSC ring buffer: wait-free, each index has exactly one writer");
    g_rb_head = 0; g_rb_tail = 0;
    {
        double t0 = mt::now_ms();
        mt::Thread p, c;
        p.start(&spsc_producer, NULL);
        c.start(&spsc_consumer, NULL);
        p.join(); c.join();
        std::cout << "  200000 items in " << (mt::now_ms() - t0) << " ms\n";
    }
    std::cout << "  lock-free vs wait-free：Treiber 栈是前者，SPSC 环是后者。\n";
    std::cout << "  无锁 != 更快：竞争不激烈时 mutex 常常更划算（少了重试和缓存行来回）。\n";

#if MT_HAS_11
    mt_section(7, "the same Treiber stack, now with std::atomic<Node*>");
    g_std_head.store(0);
    g_std_pushes.store(0); g_std_pops.store(0); g_std_retries.store(0);
    {
        std::vector<std::thread> v;
        for (int i = 0; i < 4; ++i) v.push_back(std::thread(&std_push_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        v.clear();
        for (int i = 0; i < 4; ++i) v.push_back(std::thread(&std_pop_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  pushes = " << g_std_pushes.load() << " pops = " << g_std_pops.load()
                  << " cas retries = " << g_std_retries.load() << "\n";
    }
#endif

#if MT_HAS_11
    // ---- 3. 标准库 CAS ----
    mt_section(2, "compare_exchange_weak vs compare_exchange_strong");
    g_max_value.store(0);
    {
        std::vector<std::thread> v;
        for (int t = 0; t < 4; ++t) v.push_back(std::thread(&cas_max, t * 10 + 5));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  lock-free max() = " << g_max_value.load() << " (expect 35)\n";
    }
    aba_demo();

    mt_section(7, "atomic_flag: the smallest lock the standard gives you");
    g_flag_hits = 0;
    {
        std::vector<std::thread> v;
        for (int t = 0; t < 4; ++t) v.push_back(std::thread(&flag_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  atomic_flag critical sections = " << g_flag_hits
                  << " (expect 400000)\n";
    }

    mt_section(8, "call_once: lazy initialization with a happens-before edge");
    g_once_runs = 0;
    {
        std::vector<std::thread> v;
        for (int t = 0; t < 8; ++t) v.push_back(std::thread(&once_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  initializer ran " << g_once_runs << " time(s)\n";
    }
#else
    mt_skip(11, "std::atomic CAS / atomic_flag / call_once",
            "语言级原子类型是 C++11 的东西；上面第 1、2 段用编译器内置原子做了同样的事");
#endif

#if MT_HAS_20 && defined(__cpp_lib_atomic_wait)
    mt_section(7, "std::atomic::wait/notify: 标准库把 futex 露出来了（C++20）");
    {
        std::atomic<int> state(0);
        std::thread w([&state] {
            state.wait(0);                     // 值仍是 0 就睡，被唤醒后重查
            std::cout << "  waiter woke with state = " << state.load() << "\n";
        });
        mt::sleep_ms(20);
        state.store(1);
        state.notify_all();
        w.join();
    }
#elif MT_HAS_11
    mt_skip(20, "std::atomic::wait/notify",
            "C++20 (P1135) 才进标准库；在此之前要阻塞在原子变量上只能自己配 condvar");
#endif
    return 0;
}
