// mt_11_event_loop.cpp —— Event Loop：异步不等于多线程
//
// 对应 docs/multy_thread.md：
//   十一、第十层 Async I/O（blocking -> non-blocking -> select/poll/epoll/kqueue/IOCP/io_uring）
//   十二、第十一层 Event Loop（JS/Node、Netty、Asio、Tokio、asyncio、libuv）
//   二十节 Python/JS：一段程序可以"单线程 + 异步 + non-blocking I/O"
//
// 这里不做真 socket（跨平台代码会淹没重点），只把事件循环的骨架搭出来：
//   一个线程 + 一个就绪队列 + 一个定时器堆 + 一轮轮循环
//   别的线程只负责往队列里塞事件，所有回调都在同一条线程上跑。
// 骨架一旦看懂，epoll_wait / IOCP GetQueuedCompletionStatus 只是"事件从哪来"的区别。
#include "mt_common.h"

namespace {

typedef void (*Handler)(void*);

// ---------------------------------------------------------------------------
// 就绪队列：投递者只负责入队，消费者只有循环自己（单消费者）
// 生产级实现会用 MPSC 无锁队列（CAS 头指针）省掉这把锁，见 mt_05 的 Treiber 栈；
// 这里用锁把注意力留给"循环"本身。
// ---------------------------------------------------------------------------
struct TaskNode { Handler h; void* arg; TaskNode* next; };

TaskNode* g_queue_head = 0;
TaskNode* g_queue_tail = 0;
mt::SpinLock g_queue_lk;

void post(Handler h, void* arg) {
    TaskNode* n = new TaskNode;
    n->h = h; n->arg = arg; n->next = 0;
    g_queue_lk.lock();
    if (g_queue_tail) g_queue_tail->next = n; else g_queue_head = n;
    g_queue_tail = n;
    g_queue_lk.unlock();
}

int drain_tasks();

// ---------------------------------------------------------------------------
// 定时器：小规模就线性扫，真实实现用时间堆/时间轮
// ---------------------------------------------------------------------------
struct Timer {
    double due_ms;
    long   period_ms;   // 0 = 一次性
    Handler h;
    void*  arg;
    bool   active;
};

const int kMaxTimers = 8;
Timer g_timers[kMaxTimers];
int g_timer_n = 0;

void add_timer(long period_ms, Handler h, void* arg) {
    if (g_timer_n >= kMaxTimers) return;
    Timer& t = g_timers[g_timer_n++];
    t.due_ms = mt::now_ms() + period_ms;
    t.period_ms = period_ms;
    t.h = h; t.arg = arg; t.active = true;
}

long g_timer_fires = 0, g_task_runs = 0, g_loop_ticks = 0, g_seq = 0;

void log_line(const char* what) {
    std::cout << "    #" << ++g_seq << " " << what
              << "  (tid " << mt::os_tid() << ")\n";
}

void on_tick_a(void*) { ++g_timer_fires; log_line("timer A fired"); }
void on_tick_b(void*) { ++g_timer_fires; log_line("timer B fired"); }
void on_task(void*)   { ++g_task_runs;   log_line("posted task ran"); }

// ---------------------------------------------------------------------------
// 循环本体：一次 tick = 跑到期定时器 + 排空就绪队列；没活就打个盹
// ---------------------------------------------------------------------------
int run_loop(double until_ms) {
    while (mt::now_ms() < until_ms) {
        ++g_loop_ticks;
        double now = mt::now_ms();
        int worked = 0;

        for (int i = 0; i < g_timer_n; ++i) {                 // 阶段 1：timers
            Timer& t = g_timers[i];
            if (t.active && now >= t.due_ms) {
                t.h(t.arg);
                ++worked;
                if (t.period_ms > 0) t.due_ms = now + t.period_ms;
                else t.active = false;
            }
        }
        worked += drain_tasks();                              // 阶段 2：poll/immediate
        if (worked == 0) mt::sleep_ms(1);                      // 阶段 3：idle/检查退出
    }
    return g_loop_ticks;
}

int drain_tasks() {
    int ran = 0;
    for (;;) {
        g_queue_lk.lock();
        TaskNode* head = g_queue_head;
        g_queue_head = 0;
        g_queue_tail = 0;
        g_queue_lk.unlock();
        if (head == 0) break;
        while (head != 0) {
            TaskNode* next = head->next;
            head->h(head->arg);                  // 回调全部在循环线程上执行
            delete head;
            head = next;
            ++ran;
        }
    }
    return ran;
}

// 外部世界：另一个线程模拟"网络上有数据到达"，它只投递，不执行回调
void network_thread(void* arg) {
    int id = (int)mt::from_ptr(arg);
    for (int i = 0; i < 3; ++i) {
        mt::sleep_ms(15);
        post(&on_task, 0);
        (void)id;
    }
}

}  // namespace

int main() {
    std::cout << "mt_11_event_loop : one thread, many pending operations\n";

    mt_section(12, "single-threaded reactor: timers + posted callbacks");
    double t0 = mt::now_ms();
    add_timer(20, &on_tick_a, 0);            // 周期任务
    add_timer(35, &on_tick_b, 0);
    add_timer(50, &on_task, 0);              // 一次性任务

    mt::Thread net;
    net.start(&network_thread, mt::to_ptr(1));

    run_loop(t0 + 120);                       // 跑 120ms
    net.join();
    drain_tasks();                            // 收尾：把最后投递的干掉

    std::cout << "  ticks = " << g_loop_ticks << ", timer fires = " << g_timer_fires
              << ", posted tasks = " << g_task_runs << "\n";
    std::cout << "  所有回调的 tid 都一样：全程只有一条线程在执行，这就是 JS 的模型。\n";

    std::cout << "\nloop phases (对齐 Node.js 的说法):\n"
                 "  timers  -> 到期定时器\n"
                 "  poll    -> epoll_wait / kqueue / IOCP 取出就绪 I/O 事件\n"
                 "  check   -> setImmediate 一类\n"
                 "  microtask -> Promise.then 在阶段之间清空\n";

    std::cout << "\nI/O 后端怎么选（文档第十一节那条演化链）:\n"
                 "  blocking read  : 1 连接 1 线程，10 万连接 = 10 万线程 -> 撑不住\n"
                 "  select         :  fd 位图，1024 上限，每次全量拷贝 + 线性扫描\n"
                 "  poll           :  去掉 1024 上限，仍然 O(n) 扫描\n"
                 "  epoll/kqueue   :  内核维护就绪链表，回调式，O(就绪数)\n"
                 "  IOCP(Windows)  :  完成端口，工人在内核里等结果，队列长度受控\n"
                 "  io_uring       :  提交/完成双队列，批量 syscall，甚至省掉 syscall\n"
                 "  event loop     :  把上面任何一种包成'单线程 + 就绪回调'的形状\n";

    std::cout << "\nAsync I/O vs Coroutine（第十二节的分工）:\n"
                 "  event loop 管'什么时候有事发生'\n"
                 "  coroutine 管'代码怎么写得像同步但仍能挂起'\n"
                 "  async/await 就是把协程挂到事件循环上：见 mt_10 第三段\n";
    return 0;
}
