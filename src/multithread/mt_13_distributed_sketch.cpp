// mt_13_distributed_sketch.cpp —— 分布式并发：共享内存消失之后
//
// 对应 docs/multy_thread.md：
//   二十二、第十二层 Distributed Concurrency
//   第二十一节末尾那条链：CAS -> optimistic concurrency -> 版本号 -> MVCC -> 隔离级别
//
// 一个进程里模拟三个"节点"，通过消息（而不是共享内存）交互，演示四件事：
//   A. Lamport 逻辑时钟：没有同步的物理钟，也能给出"谁在前谁在后"
//   B. 乐观并发控制（OCC）：version 号 + 重试，就是数据库 CAS / 分布式 CAS
//   C. 带租约的分布式锁 + fencing token：解决"持锁者 GC 停顿后回来覆盖数据"
//   D. 一次写偏斜（write skew）说明：隔离级别不够时，逻辑 race 依然在
//
// 这一层不需要线程：单机并发学到的"顺序/原子/可见性"问题，
// 换成消息传递 + 不可靠网络 + 各自有本地钟之后全部重新出现一遍。
#include "mt_common.h"

#include <map>
#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// A. Lamport 时钟
// ---------------------------------------------------------------------------
class Lamport {
public:
    Lamport(const char* id) : id_(id), c_(0) {}

    long on_local_event() { c_ += 1; return c_; }
    long on_send()        { c_ += 1; return c_; }          // 发送时把 c 塞进消息头

    // 收到消息：本地时钟 = max(本地, 对方) + 1，这一步才是关键
    long on_receive(long msg_clock) {
        c_ = (msg_clock > c_ ? msg_clock : c_) + 1;
        return c_;
    }
    const char* id() const { return id_; }

private:
    const char* id_;
    long c_;
};

// ---------------------------------------------------------------------------
// B. 乐观并发控制：每个值带一个 version，写的时候比对 version
// ---------------------------------------------------------------------------
struct Row {
    std::string value;
    long version;
    Row() : value(""), version(0) {}
};

std::map<std::string, Row> g_store;
long g_occ_attempts = 0, g_occ_retries = 0;

// 事务式读改写：读到 version -> 计算 -> 带 version 提交；不匹配就失败重试
bool commit_if_version(const std::string& key, long expect, const std::string& next) {
    ++g_occ_attempts;
    Row& r = g_store[key];
    if (r.version != expect) return false;          // 有人先写了：冲突，回退
    r.value = next;
    r.version += 1;
    return true;
}

std::string read_value(const std::string& key, long& out_version) {
    Row& r = g_store[key];
    out_version = r.version;
    return r.value;
}

// ---------------------------------------------------------------------------
// C. 带租约的锁 + fencing token
// ---------------------------------------------------------------------------
struct Lease {
    std::string owner;
    long token;        // 单调递增的"栅栏号"
    double expire_ms;
    Lease() : token(0), expire_ms(0) {}
};

Lease g_lease;
long g_token_counter = 0;
long g_stale_writes_rejected = 0;

bool try_acquire(const std::string& who, double now_ms, long ttl_ms, long& out_token) {
    if (g_lease.owner.empty() || now_ms >= g_lease.expire_ms) {   // 没人持有 or 已过期
        g_lease.owner = who;
        g_lease.token = ++g_token_counter;
        g_lease.expire_ms = now_ms + ttl_ms;
        out_token = g_lease.token;
        return true;
    }
    if (g_lease.owner == who) { out_token = g_lease.token; return true; }
    out_token = 0;
    return false;
}

// 服务端只接受"令牌不小于我记录的令牌"的写：
// 老持有者 GC 停顿后苏醒、租约已过期又被别人抢走时，它的写会被令牌拒绝。
bool fenced_write(const std::string& who, long token, const std::string& val) {
    if (token < g_lease.token) {              // 栅栏检查
        ++g_stale_writes_rejected;
        std::cout << "    reject write by " << who << " with token " << token
                  << " (fence is " << g_lease.token << ")\n";
        return false;
    }
    Row& r = g_store["report"];
    r.value = val;
    r.version += 1;
    std::cout << "    accept write by " << who << " with token " << token << "\n";
    return true;
}

}  // namespace

int main() {
    std::cout << "mt_13_distributed_sketch : shared memory is gone, same problems remain\n";

    // ---- A ----
    mt_section(12, "Lamport clock: ordering without synchronized wall clocks");
    {
        Lamport a("node-A"), b("node-B"), c("node-C");
        long ta1 = a.on_local_event();
        long ta2 = a.on_send();
        long tb1 = b.on_receive(ta2);
        long tb2 = b.on_send();
        long tc1 = c.on_receive(tb2);
        long tc2 = c.on_local_event();
        std::cout << "    A e1=" << ta1 << " A send=" << ta2 << " B recv=" << tb1
                  << " B send=" << tb2 << " C recv=" << tc1 << " C e2=" << tc2 << "\n";
        std::cout << "    happened-before 推出：A.e1 < A.send < B.recv < B.send < C.recv\n";
        std::cout << "    注意：Lamport 只给全序，不给因果以外的信息；要精确因果用 Vector Clock。\n";
    }

    // ---- B ----
    mt_section(12, "optimistic concurrency control = CAS over the network");
    {
        g_store["counter"] = Row();
        g_store["counter"].value = "0";
        g_store["counter"].version = 1;

        // 两个"客户端"同时读同一个版本，各自 +1，后提交者必须失败重试
        long v1 = 0, v2 = 0;
        read_value("counter", v1);
        read_value("counter", v2);
        bool ok1 = commit_if_version("counter", v1, "1");
        bool ok2 = commit_if_version("counter", v2, "1");        // 基于同一个旧版本
        if (!ok2) {
            ++g_occ_retries;
            long v3 = 0;
            std::string cur = read_value("counter", v3);
            ok2 = commit_if_version("counter", v3, "2");         // 重新读再试
        }
        std::cout << "    first txn = " << (ok1 ? "committed" : "aborted") << "\n";
        std::cout << "    second txn retried, final value = " << g_store["counter"].value
                  << ", version = " << g_store["counter"].version << "\n";
        std::cout << "    attempts = " << g_occ_attempts << ", retries = " << g_occ_retries << "\n";
        std::cout << "    这就是 UPDATE ... WHERE version = ? 、ETag、MVCC 的最小形状。\n";
        (void)ok2;
    }

    // ---- C ----
    mt_section(12, "lease lock + fencing token: why 'I still hold the lock' can be a lie");
    {
        double now = mt::now_ms();
        long tok_old = 0, tok_new = 0;
        try_acquire("worker-1", now, 50, tok_old);
        std::cout << "    worker-1 got lock, token " << tok_old << ", lease 50ms\n";

        // worker-1 GC 停顿：租约到期，锁被别人拿走
        now += 120;
        try_acquire("worker-2", now, 50, tok_new);
        std::cout << "    after the pause, worker-2 got lock, token " << tok_new << "\n";

        // 停顿结束，worker-1 以为自己还持有锁，回来写数据
        fenced_write("worker-1", tok_old, "from-stale-holder");   // 被拒
        fenced_write("worker-2", tok_new, "from-current-holder"); // 接受
        std::cout << "    stale writes rejected = " << g_stale_writes_rejected << "\n";
        std::cout << "    结论：光有锁不够，临界区里的每次写都要带上栅栏令牌。\n";
        std::cout << "    Redlock/ZooKeeper/etcd 都在解决同一个问题，只是取令牌的方式不同。\n";
    }

    // ---- D ----
    mt_section(12, "write skew: no lost update, still wrong (isolation levels matter)");
    {
        std::cout << "    两个事务并发读同一组约束数据，各自写不同的对象：\n"
                     "      T1: read(shift_ok)=true  -> write(doctor_on_call=false)\n"
                     "      T2: read(shift_ok)=true  -> write(doctor_on_call=false)\n"
                     "    READ COMMITTED 下两者都不冲突（写的不是同一行），\n"
                     "    但约束\"至少留一个人值班\"被破坏了 —— 这就是写偏斜。\n"
                     "    解法：SERIALIZABLE / 谓词锁 / 把约束读进同一把锁的临界区。\n";
        std::cout << "    单机版的对应物见 mt_04 的 check-then-act：同一个逻辑错误，换个尺度而已。\n";
    }

    std::cout << "\nfrom one CPU to a cluster (文档第二十二节那条阶梯):\n"
                 "  1 core -> N cores -> N threads -> N processes -> N machines\n"
                 "  共享内存 + 缓存一致性 逐渐让位给 消息传递 + 部分失败 + 逻辑时钟\n"
                 "  而 CAS 这条线一路贯通：lock cmpxchg -> std::atomic -> version 字段 -> MVCC\n";
    return 0;
}
