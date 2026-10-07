// mt_04_correctness.cpp —— 并发正确性远不止 data race
//
// 对应 docs/multy_thread.md 第七节（第六层）那棵分类树：
//   Race            : Data Race / Race Condition / Lost Update
//   Deadlock        : lock-order inversion / resource cycle
//   Livelock / Starvation / Priority Inversion
//   Atomicity Violation / Order Violation / Visibility / Unsafe Publication
//   TOCTOU
//   Memory Reclamation（见 mt_05）
//
// 核心结论（文档里那句"Data Race ≠ Race Condition"）：
//   下面第 1 段全程只用原子操作，一个 data race 都没有，结果照样错。
//
// 输出统一英文（避免控制台编码问题），中文引号一律用「」。
#include "mt_common.h"

#if MT_HAS_11
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#endif

namespace {

// ---------------------------------------------------------------------------
// 1) Atomicity violation / TOCTOU：没有 data race，逻辑仍然错
// ---------------------------------------------------------------------------
long g_balance = 0;              // 账户余额（只用原子操作访问）
const long kCap = 100;           // 上限

// 语义：「如果还没到上限，就把剩下的额度一次刷满」。
// 三步各自都是原子的，但「读—判断—写」之间可以被插队。
// 中间那一下 sleep 是刻意放大的窗口：真实系统里它可能是一次 GC、一次缺页、
// 一次网络往返 —— 微秒级也足够出事，只是不容易复现而已。
void top_up(void*) {
    for (int i = 0; i < 50; ++i) {
        long cur = mt::os_fetch_add(&g_balance, 0);      // 原子读
        if (cur < kCap) {
            mt::sleep_ms(1);
            mt::os_fetch_add(&g_balance, kCap - cur);    // 基于旧值的原子加
        }
    }
}

// 正确写法：把判断一起放进临界区（等价做法是 CAS 循环，见 mt_05）
void top_up_locked(void* p) {
    mt::SpinLock* lk = static_cast<mt::SpinLock*>(p);
    for (int i = 0; i < 50; ++i) {
        mt::Guard<mt::SpinLock> g(*lk);
        long cur = g_balance;
        if (cur < kCap) {
            mt::sleep_ms(1);
            g_balance = kCap;        // 此刻没人能插队
        }
    }
}

// ---------------------------------------------------------------------------
// 2) 饥饿：test-and-set 自旋锁不公平，ticket lock 公平
// ---------------------------------------------------------------------------
long g_ticket_now = 0;    // 正在服务的号
long g_ticket_next = 0;   // 下一个发出去的号

class TicketLock {
public:
    void lock() {
        long my = mt::os_fetch_add(&g_ticket_next, 1);        // 取号
        while (mt::os_fetch_add(&g_ticket_now, 0) != my) { }  // 等叫号
    }
    void unlock() {
        mt::os_fence();
        mt::os_fetch_add(&g_ticket_now, 1);                   // 叫下一个号
    }
};

// 关键：不能统计「每个线程拿到了多少次」。每个线程的循环都会跑完，
// 次数永远相等（都是 20000），完全看不出饥饿 —— 饥饿是「等了多久」的问题。
// 所以这里量的是每次进入临界区之前的等待时间：
//   test-and-set spin lock：谁抢到算谁，等待时间可以差出一个数量级；
//   ticket lock：等待长度 = 前面还有几张号 × 持锁时间，天然有上界（公平）。
const int kWorkers = 8;
long g_hits[kWorkers];              // 临界区里的工作计数（两次实验永远相同）
double g_wait_sum[kWorkers];        // 累计等待时间 (ms)
double g_wait_max[kWorkers];        // 单次最长等待时间 (ms)
volatile int g_unfair = 1;          // 在启动线程前设置，运行期只读
mt::SpinLock g_spin;
TicketLock g_ticket;

// 临界区里做一点事，让锁真的被「持有」一段时间，排队才有意义
void do_critical_work(int id) {
    volatile long sink = 0;
    for (int k = 0; k < 50; ++k) sink += k;
    g_hits[id] += 1;
}

void contended_worker(void* arg) {
    int id = (int)mt::from_ptr(arg);
    for (int i = 0; i < 20000; ++i) {
        double t0 = mt::now_ms();
        if (g_unfair) {
            g_spin.lock();
            double w = mt::now_ms() - t0;
            g_wait_sum[id] += w;
            if (w > g_wait_max[id]) g_wait_max[id] = w;
            do_critical_work(id);
            g_spin.unlock();
        } else {
            g_ticket.lock();
            double w = mt::now_ms() - t0;
            g_wait_sum[id] += w;
            if (w > g_wait_max[id]) g_wait_max[id] = w;
            do_critical_work(id);
            g_ticket.unlock();
        }
    }
}

// ---------------------------------------------------------------------------
// 3) 活锁：经典「两个人各拿一支筷子」
//    规则：先拿自己那支，再拿对方那支；拿不到就把手里的放回去，立刻重来。
//    这样破坏了「持有并等待」，死锁没有了 —— 但如果两个人的节奏完全对称：
//    同时拿起、同时发现对方占着、同时放下、同时再拿起…… 状态一直变，
//    进度一直是 0。这就是活锁：线程都在忙，却没有一件事做完。
//
//    一个重要事实（很多教材不会讲）：真实机器上活锁是「概率性」的。
//    两个线程不可能永远同步，总有一次交错让某边先拿到第二支筷子，于是
//    自己跑就复现不出来。所以这里分三种模式对照：
//      mode 0 自然交错        —— 大概率前进，看不到活锁（复现失败本身就是知识点）
//      mode 1 强制同步 + 各拿各的 —— 每一轮都必然对撞，活锁 100% 复现
//      mode 2 强制同步 + 全局顺序 —— 同样同步，但换成固定拿锁顺序，活锁消失
//    （只用 OS 原子，C++98 分支也能跑）
// ---------------------------------------------------------------------------
const int kLiveRounds = 200;
long g_res[2];
int  g_live_mode = 0;
long g_sync_a = 0, g_sync_b = 0;   // 两方对齐用的计数器
long g_live_ok = 0;                 // 真正前进的次数
long g_live_giveup = 0;             // 让路/重试的次数（状态一直在变的证据）

// 让两个线程在同一时刻进入同一步：活锁需要「节奏一致」才成立
void step_sync(int me) {
    if (g_live_mode == 0) return;
    long* mine = (me == 0) ? &g_sync_a : &g_sync_b;
    long* peer = (me == 1) ? &g_sync_a : &g_sync_b;
    long target = mt::os_fetch_add(mine, 1) + 1;      // 我到了第 target 站
    while (mt::os_fetch_add(peer, 0) < target) { }    // 等对方也到
}

bool grab(int i)    { return mt::os_cas_long(&g_res[i], 0, 1) == 0; }
void release(int i) { mt::os_cas_long(&g_res[i], 1, 0); }

void livelock_worker(void* arg) {
    int me = (int)mt::from_ptr(arg);
    long ok = 0, giveup = 0;

    for (int i = 0; i < kLiveRounds; ++i) {
        step_sync(me);                                    // 对齐 1：本轮开始
        int first  = (g_live_mode == 2) ? 0 : me;         // 各拿各的 vs 全局顺序
        int second = 1 - first;
        bool held = grab(first);
        step_sync(me);                                    // 对齐 2：都站稳了再伸手
        bool got2 = false;
        if (held) got2 = grab(second);
        step_sync(me);                                    // 对齐 3：两次伸手都做完，才准放手
        //   ^ 这一步是关键。少了它，一方「让路」释放的资源会正好被另一方的
        //     伸手捡走，于是每轮总有一边前进 —— 看着像活锁，其实是普通竞争。
        if (held && got2) {
            ++ok;                                         // 两支都在手：前进
            release(second);
            release(first);
        } else {
            ++giveup;
            if (got2) release(second);
            if (held) release(first);                     // 让路：把手里的还回去
        }
    }
    mt::os_fetch_add(&g_live_ok, ok);
    mt::os_fetch_add(&g_live_giveup, giveup);
}

#if MT_HAS_11
// ---------------------------------------------------------------------------
// 4) 死锁：lock-order inversion（用 timed_mutex 探测，不让进程真挂住）
// ---------------------------------------------------------------------------
std::timed_mutex mA, mB;

// 死锁要真的「演示出来」而不是讲出来，就必须让两条线程确实同时卡在循环等待上。
// 但一旦真的循环等待，进程就挂死了 —— 所以这里用 timed_mutex：
// 拿第二把锁时带超时，超时即证明「我在等一个永远不会释放的人」，然后自己松手。
// 中间那次 sleep 是刻意制造「各自已持有一把、正伸手拿对方那把」的窗口。
void ab_order() {
    if (mA.try_lock_for(std::chrono::milliseconds(150))) {
        mt::sleep_ms(100);                       // 拿着 A 伸手去够 B
        if (!mB.try_lock_for(std::chrono::milliseconds(150))) {
            std::cout << "  [cycle] thread AB holds A, waits for B (timeout proves the cycle)\n";
            mA.unlock();
            return;
        }
        mB.unlock(); mA.unlock();
        std::cout << "  [ok   ] thread AB got both\n";
    }
}
void ba_order() {
    if (mB.try_lock_for(std::chrono::milliseconds(150))) {
        mt::sleep_ms(100);                       // 拿着 B 伸手去够 A（顺序相反）
        if (!mA.try_lock_for(std::chrono::milliseconds(150))) {
            std::cout << "  [cycle] thread BA holds B, waits for A (timeout proves the cycle)\n";
            mB.unlock();
            return;
        }
        mA.unlock(); mB.unlock();
        std::cout << "  [ok   ] thread BA got both\n";
    }
}

std::mutex m1, m2;
void fix_std_lock() {
    // 修法一：一次性请求两把锁，顺序由库决定（内部 try + back-off）
    std::lock(m1, m2);
    std::lock_guard<std::mutex> g1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> g2(m2, std::adopt_lock);
}
void fix_global_order() {
    // 修法二：全局锁序，所有线程按同一顺序拿锁 -> 破坏「循环等待」
    std::lock_guard<std::mutex> g1(m1);
    std::lock_guard<std::mutex> g2(m2);
}
#endif  // MT_HAS_11

}  // namespace

int main() {
    std::cout << "mt_04_correctness : the correctness tree is bigger than data race\n";

    // ---- 1. atomicity violation ----
    mt_section(6, "atomicity violation / TOCTOU: every access is atomic, result still wrong");
    g_balance = 0;
    {
        mt::Thread t1, t2;
        t1.start(&top_up, NULL);
        t2.start(&top_up, NULL);
        t1.join(); t2.join();
    }
    std::cout << "  check-then-act balance = " << g_balance
              << ", cap = " << kCap << ", overshoot = " << (g_balance - kCap) << "\n";
    {
        mt::SpinLock lk;
        g_balance = 0;
        mt::Thread t1, t2;
        t1.start(&top_up_locked, &lk);
        t2.start(&top_up_locked, &lk);
        t1.join(); t2.join();
        std::cout << "  with critical section balance = " << g_balance
                  << " (must be exactly " << kCap << ")\n";
    }
    std::cout << "  lesson: protect a piece of logic, not a single variable.\n";

    // ---- 2. starvation ----
    mt_section(6, "starvation: test-and-set spin lock vs ticket lock");
    for (int round = 0; round < 2; ++round) {
        g_unfair = (round == 0) ? 1 : 0;
        g_ticket_now = 0; g_ticket_next = 0;
        for (int i = 0; i < kWorkers; ++i) {
            g_hits[i] = 0; g_wait_sum[i] = 0.0; g_wait_max[i] = 0.0;
        }
        mt::Thread ts[kWorkers];
        for (int i = 0; i < kWorkers; ++i) ts[i].start(&contended_worker, mt::to_ptr(i));
        for (int i = 0; i < kWorkers; ++i) ts[i].join();

        double sum_mx = 0.0, sum_mn = 1e18, single_mx = 0.0;
        std::cout << "  " << (round == 0 ? "spin(test-and-set)" : "ticket(FIFO)      ")
                  << " acquisitions:";
        for (int i = 0; i < kWorkers; ++i) std::cout << " " << g_hits[i];
        std::cout << "\n  total wait (ms):";
        for (int i = 0; i < kWorkers; ++i) {
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%.2f", g_wait_sum[i]);
            std::cout << " " << buf;
            if (g_wait_sum[i] > sum_mx) sum_mx = g_wait_sum[i];
            if (g_wait_sum[i] < sum_mn) sum_mn = g_wait_sum[i];
        }
        std::cout << "\n  longest single wait (ms):";
        for (int i = 0; i < kWorkers; ++i) {
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%.3f", g_wait_max[i]);
            std::cout << " " << buf;
            if (g_wait_max[i] > single_mx) single_mx = g_wait_max[i];
        }
        std::cout << "\n  skew (max/min total wait) = " << (sum_mn > 0 ? sum_mx / sum_mn : 0.0)
                  << ", worst single wait = " << single_mx << " ms\n";
    }
    std::cout << "  note: acquisition counts are identical under both locks -- counts cannot\n"
                 "        reveal starvation. Wait time can: a fair lock bounds how long any\n"
                 "        thread may sit while others keep being served.\n"
                 "  trade-off worth noticing: ticket's total wait is LARGER than the spin\n"
                 "        lock's. Strict rotation forbids the shortcut where the thread that\n"
                 "        is already on-CPU grabs the lock again, so throughput drops.\n"
                 "        Fairness and throughput pull in opposite directions here.\n";

    // ---- 3. livelock ----
    mt_section(6, "livelock: state keeps changing, progress stays at zero");
    const char* mode_name[3] = {
        "natural interleave   ",
        "forced lockstep+private",
        "forced lockstep+ordered"
    };
    for (int mode = 0; mode < 3; ++mode) {
        g_live_mode = mode;
        g_res[0] = 0; g_res[1] = 0;
        g_live_ok = 0; g_live_giveup = 0;
        g_sync_a = 0; g_sync_b = 0;
        {
            mt::Thread t1, t2;
            t1.start(&livelock_worker, mt::to_ptr(0));
            t2.start(&livelock_worker, mt::to_ptr(1));
            t1.join(); t2.join();
        }
        std::cout << "  [" << mode_name[mode] << "] rounds = " << kLiveRounds
                  << " x2 threads, progress = " << g_live_ok
                  << ", give-ups = " << g_live_giveup << "\n";
    }
    std::cout << "  reading the numbers:\n"
                 "    - natural interleave almost always escapes: real livelock is timing-\n"
                 "      dependent, which is exactly why it is so hard to reproduce.\n"
                 "    - forced lockstep reproduces it 100%: every round both threads hold\n"
                 "      one resource, both fail on the second, both step back. Busy, zero\n"
                 "      progress -- that is livelock, not deadlock (nobody is blocked).\n"
                 "    - same lockstep, but a fixed global order: someone always completes,\n"
                 "      so give-ups become ordinary contention instead of mutual retreat.\n";
    std::cout << "  fixes: random backoff, an ordering rule, or a queue that owns the lock.\n";

#if MT_HAS_11
    // ---- 4. deadlock ----
    mt_section(6, "deadlock: lock-order inversion, probed with timed locks");
    {
        std::thread ta(&ab_order), tb(&ba_order);
        ta.join(); tb.join();
    }
    std::cout << "  four conditions: mutual exclusion / hold-and-wait /\n"
                 "                   no preemption / circular wait.\n";
    std::cout << "  break any one of them; practice uses global lock order or one-shot locking.\n";
    {
        // 两种修法各跑一批线程；注意 join 过的 thread 对象不能复用，所以分两个 vector
        std::vector<std::thread> v1;
        for (int i = 0; i < 8; ++i) v1.push_back(std::thread(&fix_std_lock));
        for (int i = 0; i < 8; ++i) v1[i].join();

        std::vector<std::thread> v2;
        for (int i = 0; i < 8; ++i) v2.push_back(std::thread(&fix_global_order));
        for (int i = 0; i < 8; ++i) v2[i].join();

        std::cout << "  std::lock and fixed order both finished without a cycle\n";
    }
#else
    mt_skip(11, "deadlock probe with std::timed_mutex",
            "为了不演示到一半进程真挂死，这里用超时锁；C++11 才有 timed_mutex");
#endif
    return 0;
}
