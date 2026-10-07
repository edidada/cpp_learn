# multy_thread
对。要建立“**并发 / 异步编程完整知识体系**”，不能只围绕“数据竞争、死锁、吞吐量、延迟”展开。更好的方式是从 **硬件 → ISA/汇编 → 内存模型 → OS → 线程同步 → 无锁 → 异步 I/O → 协程 → Reactive → 分布式并发** 一层层建立模型。

而且应该把 **ASM、C、C++、Rust、Java、Kotlin、C#、Go、Swift、JavaScript/TypeScript、Python** 放在同一张地图里比较。

---

# 一、先建立总地图

可以把整个并发 / 异步体系拆成 **12 层**：

```text
并发 / 异步编程
│
├─ 01. 硬件并行
│    CPU Core / SMT / Cache / Cache Line
│    MESI / Store Buffer / NUMA
│
├─ 02. ISA / 汇编并发
│    atomic instruction
│    CAS / LL-SC
│    fence / barrier
│    acquire / release
│
├─ 03. 内存模型
│    visibility
│    ordering
│    atomicity
│    happens-before
│    sequential consistency
│    relaxed memory
│
├─ 04. OS 并发模型
│    process
│    kernel thread
│    scheduler
│    context switch
│    preemption
│    signal
│
├─ 05. 线程同步
│    mutex / rwlock
│    semaphore
│    condition variable
│    monitor
│    latch / barrier
│
├─ 06. 数据竞争与并发正确性
│    race condition
│    data race
│    lost update
│    ABA
│    deadlock
│    livelock
│    starvation
│    priority inversion
│
├─ 07. Atomic / Lock-Free
│    CAS
│    atomic RMW
│    lock-free
│    wait-free
│    obstruction-free
│    memory reclamation
│
├─ 08. Task / Future
│    Future
│    Promise
│    CompletableFuture
│    async/await
│
├─ 09. Coroutine / Structured Concurrency
│    coroutine
│    suspend
│    continuation
│    cancellation
│    scope
│
├─ 10. Async I/O
│    blocking I/O
│    non-blocking I/O
│    select/poll
│    epoll/kqueue
│    IOCP
│    io_uring
│    event loop
│
├─ 11. Stream / Reactive
│    Iterator
│    async iterator
│    Stream
│    Channel
│    Flow
│    Reactive Streams
│    backpressure
│
└─ 12. Distributed Concurrency
     message passing
     actor
     distributed lock
     consensus
     transaction
     logical clock
```

这里最重要的一点是：

> **并行、并发、异步、非阻塞、协程、Reactive、Lock-Free 是不同维度，不是同义词。**

---

# 二、第一层：硬件——并发问题真正开始的地方

这是很多 Java/Kotlin/Go 教材容易跳过去的一层。

现代 CPU 大致是：

```text
                  Memory
                    │
             Memory Controller
                    │
        ┌───────────┴───────────┐
       CPU                     CPU
        │                       │
    ┌───┴───┐               ┌───┴───┐
   Core    Core             Core    Core
    │       │                │       │
   L1      L1               L1      L1
    │       │
   L2      L2
     \     /
       L3
```

程序员首先面对的问题不是 mutex，而是：

**多个 CPU Core 如何看到同一份内存？**

于是产生：

```text
Cache
Cache Line
Cache Coherence
MESI/MOESI
Store Buffer
Load Buffer
Out-of-order execution
Memory ordering
False Sharing
NUMA
```

例如：

```cpp
int x = 0;

Thread 1: x = 1;
Thread 2: print(x);
```

你直觉上可能认为：

```text
CPU1 写 RAM
CPU2 读 RAM
```

实际更接近：

```text
CPU1
  ↓
Store Buffer
  ↓
L1 Cache
  ↓
Cache coherence
  ↓
CPU2 L1
```

因此：

> **并发编程首先是缓存一致性 + 内存顺序问题，然后才是语言层面的锁。**

---

# 三、第二层：ASM / ISA——并发的机器级基础

你特别要求把 ASM 包括进去，这一层非常重要。

高级语言最终必须依赖 CPU 提供的原子操作。

## 普通指令不一定够

例如：

```cpp
counter++;
```

逻辑上是一条语句。

机器层面可能是：

```asm
mov eax, [counter]
add eax, 1
mov [counter], eax
```

两个线程：

```text
T1 load 10
T2 load 10

T1 add → 11
T2 add → 11

T1 store 11
T2 store 11
```

正确结果应该 12，却得到 11。

这就是 lost update。

---

## x86/x86-64

典型原子机制：

```asm
LOCK XADD
LOCK CMPXCHG
LOCK INC
XCHG
```

例如 CAS：

```asm
lock cmpxchg [memory], ecx
```

高级语言：

```cpp
atomic.compare_exchange_strong(...)
```

最终可能落到这样的 ISA 指令。

内存屏障还有：

```asm
LFENCE
SFENCE
MFENCE
```

不过不能简单理解成“每个 C++ memory_order 都直接对应一条 fence”，因为编译器和 x86 TSO 本身已经提供了一部分排序保证。

---

## ARM / AArch64

ARM 的内存模型比 x86 更弱。

你会遇到：

```asm
LDAR        // acquire load
STLR        // release store

LDXR
STXR        // exclusive load/store

DMB
DSB
ISB
```

新一些 ARM 还提供 LSE atomic：

```text
CAS
SWP
LDADD
...
```

所以：

```cpp
memory_order_acquire
memory_order_release
```

在不同 CPU 上可能生成完全不同的代码。

---

## RISC-V

典型机制：

```text
LR
SC

AMOSWAP
AMOADD
AMOXOR
...

FENCE
```

也就是：

```text
Load Reserved
Store Conditional
Atomic Memory Operation
```

所以学习 ASM 并发真正应该研究的是：

```text
x86 LOCK / CMPXCHG
        ↓
ARM LL/SC / LSE
        ↓
RISC-V LR/SC / AMO
        ↓
Memory Barrier
        ↓
Cache Coherence
        ↓
语言 Atomic
```

而不只是学习 `mov/add/jmp`。

---

# 四、第三层：Memory Model——整个体系的核心

这一层是 **C++ / Rust / Java / Kotlin / ASM** 之间真正的桥梁。

需要理解三个基本问题：

```text
Atomicity
Visibility
Ordering
```

也就是：

### 1. 原子性

操作是否会被其他线程观察到中间状态。

### 2. 可见性

```text
Thread A 写 x
        ↓
Thread B 什么时候能看到？
```

### 3. 顺序性

源码：

```cpp
x = 1;
y = 2;
```

并不意味着所有 CPU、编译器、其他线程都必须按照你想象的顺序观察。

---

然后进入最重要的概念：

```text
Happens-Before
Synchronizes-With
Modification Order
Sequential Consistency

Relaxed
Acquire
Release
AcqRel
SeqCst
```

C++：

```cpp
std::memory_order_relaxed
std::memory_order_acquire
std::memory_order_release
std::memory_order_acq_rel
std::memory_order_seq_cst
```

Rust：

```rust
Ordering::Relaxed
Ordering::Acquire
Ordering::Release
Ordering::AcqRel
Ordering::SeqCst
```

Java 则主要通过：

```java
volatile
synchronized
AtomicInteger
VarHandle
```

体现 Java Memory Model。

Kotlin/JVM 很大程度建立在 JMM 上。

---

# 五、第四层：OS——Thread 到底是什么

然后才真正进入操作系统。

需要掌握：

```text
Process
Thread
Kernel Thread
User Thread
Scheduler
Preemption
Time Slice
Context Switch
Thread State
CPU Affinity
Priority
```

典型：

```text
             OS Scheduler

Thread A ───────→ CPU0
Thread B ───────→ CPU1
Thread C ──┐
Thread D ──┼────→ runnable queue
Thread E ──┘
```

这里产生另外一类性能问题：

```text
Context Switch
Cache pollution
Scheduler overhead
Oversubscription
CPU migration
NUMA remote access
```

所以：

```text
100,000 threads
```

的问题并不只是内存。

还包括：

```text
scheduler
context switch
cache
kernel bookkeeping
```

这也是协程产生的重要背景之一。

---

# 六、第五层：传统同步原语

几乎所有主流语言最终都有这些东西。

```text
Mutex
Spinlock
RWLock
Semaphore
Condition Variable
Monitor
Barrier
Latch
Event
```

对应关系大致是：

| 概念 | C++ | Java | Kotlin | Rust | C# | Go |
|---|---|---|---|---|---|---|
| Mutex | `std::mutex` | `Lock`/synchronized | JVM Mutex / coroutine Mutex | `Mutex` | `lock` | `sync.Mutex` |
| RWLock | `shared_mutex` | `ReadWriteLock` | JVM | `RwLock` | `ReaderWriterLockSlim` | `sync.RWMutex` |
| Semaphore | `counting_semaphore` | `Semaphore` | `Semaphore` | crate/runtime | `SemaphoreSlim` | channel 常代替 |
| Condition | `condition_variable` | `Condition` | JVM | `Condvar` | Monitor | `sync.Cond` |
| Barrier | `barrier` | `CyclicBarrier` | JVM | `Barrier` | `Barrier` | WaitGroup 类似 |
| Atomic | `std::atomic` | `java.util.concurrent.atomic` | `kotlin.concurrent.atomics` | `std::sync::atomic` | Interlocked | `sync/atomic` |

注意 Kotlin coroutine 的 `Mutex` 和 OS/JVM mutex 的语义不完全相同：

```text
线程 Mutex
    ↓ wait
可能阻塞线程

Coroutine Mutex
    ↓ suspend
挂起 coroutine
线程可以继续执行其他 coroutine
```

这是非常重要的分界线。

---

# 七、第六层：并发正确性问题远不止 Data Race

至少应该建立下面这套分类。

```text
Concurrency Correctness
│
├─ Race
│   ├─ Data Race
│   ├─ Race Condition
│   └─ Lost Update
│
├─ Deadlock
│   ├─ lock-order inversion
│   └─ resource cycle
│
├─ Livelock
│
├─ Starvation
│
├─ Priority Inversion
│
├─ ABA Problem
│
├─ Atomicity Violation
│
├─ Order Violation
│
├─ Visibility Problem
│
├─ Unsafe Publication
│
├─ TOCTOU
│
└─ Memory Reclamation
    ├─ use-after-free
    ├─ Hazard Pointer
    ├─ Epoch
    └─ RCU
```

尤其：

```text
Data Race ≠ Race Condition
```

这是学习并发时应该专门区分的。

程序可以 **没有语言定义上的 data race，但仍然存在逻辑 race condition**。

---

# 八、第七层：Lock-Free / Atomic

这一层连接 ASM 与高级语言。

核心：

```text
Read
Write
RMW
CAS
FAA
SWAP
```

例如：

```text
Compare-And-Swap

if memory == expected:
    memory = desired
    success
else:
    expected = memory
    failure
```

然后发展出：

```text
Spin Lock
Lock-Free Stack
Lock-Free Queue
Ring Buffer
Concurrent Hash Table
```

进一步研究：

```text
ABA
Hazard Pointer
Epoch Based Reclamation
RCU
Tagged Pointer
```

以及非常容易混淆的：

```text
blocking
non-blocking

lock-free
wait-free
obstruction-free
```

其中：

> Lock-Free 不等于“完全没有锁的感觉”，它是一个严格的系统进展性质。

---

# 九、第八层：Future / Promise / Task

这里开始从“线程”进入“任务”。

传统：

```text
Thread
    ↓
执行函数
    ↓
join
    ↓
获得结果
```

Future：

```text
Task
   │
   └────→ Future<Result>
```

各语言：

```text
C++          std::future
Java         Future
Java         CompletableFuture
Rust         Future
C#           Task<T>
JavaScript   Promise
Python       Future / Task
Swift        Task
```

Java：

```java
CompletableFuture
    .supplyAsync(...)
    .thenApply(...)
    .thenCompose(...)
```

解决的核心问题是：

> **如何表示一个“未来才产生的结果”，以及如何组合这些未来结果。**

这和 mutex 解决的问题完全不同。

---

# 十、第九层：Coroutine

Coroutine 解决的又是另一层问题：

> 如何让一个任务在等待时挂起，而不是占着 OS Thread。

典型：

```text
Coroutine A
     │
     ├── CPU
     │
     ├── await network
     │
     └── suspend
             ↓
Thread ─────→ Coroutine B
```

所以：

```text
Thread ≠ Coroutine
```

主要语言：

| 语言 | Coroutine / Async |
|---|---|
| Kotlin | `suspend` / Coroutine |
| Rust | `async/await` + Future |
| C++20 | coroutine |
| C# | async/await |
| Python | async/await |
| JavaScript | async/await |
| Swift | async/await |
| Go | goroutine |
| Java | Virtual Thread |

但这里尤其要注意：

```text
C++ coroutine
```

更像**底层 coroutine 机制**，标准库并没有直接给你 Kotlin 那样完整的 coroutine runtime。

而：

```text
Kotlin Coroutine
```

则包含：

```text
suspend
Continuation
CoroutineContext
Dispatcher
Job
Scope
Cancellation
Structured Concurrency
```

是更完整的编程模型。

---

# 十一、第十层：Async I/O——协程背后的另一半

只理解 coroutine 而不理解 I/O multiplexing，会缺掉半个体系。

传统 Blocking：

```text
Thread
   │
   read(socket)
   │
   ↓
 BLOCKED
   │
   ↓
kernel
```

如果有：

```text
100,000 connections
```

简单 thread-per-connection 成本可能很高。

于是出现：

```text
select
poll
epoll       Linux
kqueue      BSD/macOS
IOCP        Windows
io_uring    Linux
```

演化：

```text
Blocking I/O
       ↓
Non-blocking I/O
       ↓
I/O Multiplexing
       ↓
Event Loop
       ↓
Future / Promise
       ↓
async/await
       ↓
Coroutine
```

这条链极其重要。

---

# 十二、第十一层：Event Loop

JavaScript / Node.js 是理解这一模型最经典的例子。

```text
            Event Loop
                │
      ┌─────────┼──────────┐
      ↓         ↓          ↓
   Timer      Socket     Promise
      │         │          │
      └─────────┴──────────┘
                ↓
              Task
```

Java：

```text
Netty
NIO
Selector
```

C++：

```text
Boost.Asio
libuv
io_uring frameworks
```

Rust：

```text
Tokio
mio
```

Python：

```text
asyncio
```

JS：

```text
Node.js
libuv
```

Kotlin：

```text
Coroutines
Ktor
```

所以：

> **Event Loop、Coroutine、Async I/O 是相关但不同的三个概念。**

---

# 十三、第十二层：Stream / Channel / Reactive

这正好接上你前面问的：

```text
CompletableFuture
Reactive Streams
Flux / Mono
suspend
Channel
Flow
```

可以这样定位：

```text
一个未来值
│
├─ Java CompletableFuture
├─ JS Promise
├─ C# Task
└─ Rust Future


挂起一个计算
│
├─ Kotlin suspend
├─ async/await
└─ coroutine


多个异步值
│
├─ Kotlin Flow
├─ Java Stream?      ← 注意 Stream 本身通常同步
├─ Reactor Flux
├─ Rx
└─ AsyncIterator


Coroutine 通信
│
└─ Channel


异步流 + 背压协议
│
└─ Reactive Streams
```

Reactor：

```text
Mono<T>
    ≈ 0..1 个元素

Flux<T>
    ≈ 0..N 个元素
```

而：

```text
Reactive Streams
```

核心问题之一是：

> Producer 比 Consumer 快怎么办？

也就是：

```text
Producer
████████████████████████

Consumer
████
```

如果无限生产：

```text
queue
   ↓
memory
   ↓
OOM
```

于是：

```text
Backpressure
```

成为 Reactive Streams 的核心设计之一。

---

# 十四、Structured Concurrency 是现代体系的重要分水岭

早期：

```text
spawn task
spawn task
spawn task

谁负责它们？
什么时候结束？
异常去哪？
怎么取消？
```

很容易出现：

```text
orphan task
task leak
lost exception
cancellation leak
```

Structured Concurrency 要求：

```text
Scope
│
├── Task A
├── Task B
│    ├── Task B1
│    └── Task B2
└── Task C
```

生命周期形成树。

典型代表：

```text
Kotlin CoroutineScope
Swift TaskGroup
Python TaskGroup
Java StructuredTaskScope
```

Rust async 生态也大量面对类似生命周期与任务管理问题。

这是现代并发体系里非常值得单独学的一章。

---

# 十五、Go 又是一条很有特色的路线

Go 的核心思想不是把锁完全消灭，而是强调：

> Don't communicate by sharing memory; share memory by communicating.

模型：

```text
goroutine
    │
 channel
    │
goroutine
```

背后是：

```text
Goroutine
    ↓
Go runtime scheduler
    ↓
OS Thread
    ↓
CPU
```

经典 G-M-P：

```text
G = Goroutine
M = Machine / OS Thread
P = Processor
```

但 Go 同样提供：

```go
sync.Mutex
sync.RWMutex
sync.WaitGroup
sync.Once
sync.Cond
sync.Map
sync/atomic
```

所以：

> Channel 并没有取代共享内存同步。

---

# 十六、Rust 最大的特点：把部分并发错误推到类型系统

Rust：

```rust
Send
Sync
Arc<T>
Mutex<T>
RwLock<T>
Atomic*
```

最特别的是：

```text
Ownership
Borrowing
Lifetime
       ↓
Concurrency Safety
```

例如多个线程共享：

```rust
Arc<Mutex<T>>
```

Rust 尝试让很多非法共享：

```text
编译期
  ↓
拒绝
```

而不是：

```text
运行
 ↓
data race
 ↓
凌晨 3 点 production 崩溃
```

但 Rust 并没有消灭：

```text
deadlock
livelock
logical race
starvation
async cancellation bug
```

类型系统解决的是其中一部分。

---

# 十七、Java：完整的工业并发体系

Java 特别适合系统学习传统并发。

路线：

```text
Thread
 │
synchronized
 │
wait / notify
 │
volatile
 │
JMM
 │
java.util.concurrent
 ├── Lock
 ├── Condition
 ├── Semaphore
 ├── CountDownLatch
 ├── CyclicBarrier
 ├── Phaser
 ├── BlockingQueue
 ├── ConcurrentHashMap
 └── Atomic*
 │
Executor
ForkJoinPool
 │
CompletableFuture
 │
Flow / Reactive Streams
 │
Virtual Threads
 │
Structured Concurrency
```

如果想理解：

```text
线程池
CAS
AQS
Lock
ConcurrentHashMap
ForkJoin
Future
```

Java/JUC 是非常好的研究对象。

---

# 十八、Kotlin 是在 Java/JVM 之上再建立 Coroutine 世界

可以理解成：

```text
             Kotlin

        ┌──── JVM 世界 ─────┐
        │                   │
      Thread             JMM/JUC
        │
   java.util.concurrent
        │
      Atomic
        │
        └───────────────────┐
                            │
                  Coroutine 世界
                            │
              ┌─────────────┼─────────────┐
            suspend      CoroutineScope    Job
              │              │             │
         Continuation     structured    cancellation
              │
          Dispatcher
              │
       ┌──────┴──────┐
    Channel          Flow
```

所以学 Kotlin 并发，**不能只学 coroutine**。

应该两条线都学：

```text
JMM/JUC/Thread/Atomic

          +

Coroutine/suspend/Flow/Channel
```

---

# 十九、C++：最接近硬件的高级语言路线之一

C++ 的体系特别适合你这种希望从软件工程一路追到 ASM 的学习方式。

```text
CPU / ISA
   ↓
C++ Memory Model
   ↓
std::atomic
   ↓
memory_order
   ↓
mutex
condition_variable
semaphore
latch
barrier
   ↓
thread / jthread
   ↓
future / promise
   ↓
C++20 coroutine
   ↓
Executors / async ecosystem
```

重点应该深入：

```cpp
std::atomic
memory_order
compare_exchange
atomic_flag
atomic_wait
std::jthread
stop_token
condition_variable
semaphore
latch
barrier
coroutine
```

然后拿 Compiler Explorer 看：

```text
C++
 ↓
LLVM/GCC
 ↓
x86-64 ASM

以及

C++
 ↓
ARM64 ASM
```

这样会真正把语言 memory model 和 CPU memory model 连起来。

---

# 二十、Python / JavaScript 的路线又不同

Python：

```text
Thread
Multiprocessing
GIL / free-threaded Python
Future
concurrent.futures
asyncio
Task
async/await
async iterator
TaskGroup
```

JavaScript：

```text
Call Stack
     ↓
Event Loop
     ↓
Task / Microtask
     ↓
Promise
     ↓
async/await
     ↓
Web API / Node APIs
```

JS 特别适合理解：

```text
异步 ≠ 多线程
```

一段程序完全可以：

```text
单线程
+
异步
+
non-blocking I/O
```

---

# 二十一、主流语言放在一张表里

这是整个知识体系最值得保存的一张表：

| 层次 | ASM | C | C++ | Java | Kotlin | Rust | Go | C# | Python | JS/TS |
|---|---|---|---|---|---|---|---|---|---|---|
| OS Thread | syscall/libc | pthread | thread | Thread | Thread | thread | runtime/M | Thread | threading | Worker |
| Mutex | atomic ISA | pthread | mutex | Lock | JVM/JUC | Mutex | Mutex | lock | Lock | — |
| Atomic | LOCK/LLSC | `_Atomic` | atomic | Atomic* | Atomic* | Atomic* | atomic | Interlocked | limited | Atomics |
| Memory Model | ISA | C11 | C++11 | JMM | JMM + K/N | Rust/C++相近 | Go MM | .NET MM | 实现相关 | ECMAScript MM |
| CAS | CMPXCHG等 | atomic CAS | CAS | CAS | CAS | CAS | CAS | CompareExchange | — | Atomics |
| Thread Pool | — | library | library | Executor | Dispatcher/JUC | runtime | runtime | ThreadPool | executor | runtime |
| Future | — | library | future | Future | Deferred | Future | — | Task | Future | Promise |
| Coroutine | 手工状态机 | library | C++20 | Loom侧重虚拟线程 | **suspend** | async | goroutine | async | async | async |
| Channel | — | library | library | queue | **Channel** | channel | **chan** | Channel | Queue | library |
| Async Stream | — | library | evolving | Flow/Reactor | **Flow** | Stream | channel | IAsyncEnumerable | async gen | AsyncIterator |
| Reactive | — | — | RxCpp等 | Reactor/RxJava | Flow/Reactor | ecosystem | RxGo等 | Rx.NET | RxPY | RxJS |
| Structured Concurrency | — | — | evolving | 有 | **核心** | ecosystem | goroutine结构需自行管理 | Task体系 | TaskGroup | 部分 |
| Async I/O | syscall | epoll | Asio/io_uring | NIO/Netty | coroutine | Tokio | netpoll | IOCP | asyncio | libuv/browser |

ASM 那一列的意义尤其重要：

> ASM 并没有 `Future`、`Coroutine`、`Reactive Streams` 这些高级抽象；它提供的是构造这些东西最底层所依赖的 **原子指令、内存屏障、系统调用与上下文状态**。

---

# 二十二、还缺最后一层：分布式并发

单机并发学完之后，问题并没有结束。

```text
一个 CPU
 ↓
多个 Core
 ↓
多个 Thread
 ↓
多个 Process
 ↓
多个 Machine
```

到了分布式环境：

```text
Shared Memory
```

基本消失，变成：

```text
Message Passing
Network
Partial Failure
Clock
Replication
```

于是新的并发问题出现：

```text
Distributed Lock
Optimistic Concurrency Control
MVCC
Transaction Isolation
Linearizability
Serializability

Lamport Clock
Vector Clock

Consensus
Paxos
Raft

2PC
Saga

Actor Model
CSP
```

这实际上和数据库知识体系直接连接起来。

例如：

```text
CAS
 ↓
Optimistic concurrency
 ↓
数据库 CAS/version
 ↓
MVCC
 ↓
Transaction Isolation
 ↓
Distributed Transaction
```

---

# 二十三、性能问题也应该独立成一棵树

不要简单写：

```text
吞吐量
延迟
```

完整一些应该是：

```text
Concurrency Performance
│
├── Throughput
├── Latency
│   ├── average
│   ├── P50
│   ├── P95
│   ├── P99
│   └── P999
│
├── Scalability
│
├── Contention
│
├── Lock Convoy
├── Context Switch
├── Cache Miss
├── False Sharing
├── Cache Line Ping-Pong
├── NUMA
│
├── Scheduling Overhead
├── Work Stealing
├── Load Balancing
│
├── Queueing
├── Backpressure
├── Head-of-Line Blocking
│
└── Amdahl's Law
```

这才是完整的“并发性能”。

---

# 二十四、最终形成四条主线

如果从**软件工程师建立知识体系**的角度，我建议不要按照语言分别学，而按照下面四条纵向主线学。

### 主线 A：CPU → ASM → Memory Model → Atomic

```text
CPU Core
→ Cache
→ MESI
→ Store Buffer
→ x86/ARM/RISC-V memory ordering
→ atomic instruction
→ fence
→ C++ memory model
→ Java Memory Model
→ Rust atomic
→ lock-free
→ RCU / Hazard Pointer
```

这是**并发底层线**。

### 主线 B：OS → Thread → Lock → Scheduler

```text
Process
→ Thread
→ Scheduler
→ Context Switch
→ Mutex
→ Futex
→ Semaphore
→ Condition Variable
→ Thread Pool
→ Fork/Join
→ Work Stealing
```

这是**传统并发线**。

### 主线 C：I/O → Event Loop → Future → Coroutine

```text
blocking I/O
→ non-blocking
→ select/poll
→ epoll/kqueue/IOCP/io_uring
→ event loop
→ callback
→ Future/Promise
→ async/await
→ coroutine
→ structured concurrency
```

这是**现代异步线**。

### 主线 D：Queue → Channel → Stream → Reactive

```text
BlockingQueue
→ Message Passing
→ CSP / Actor
→ Channel
→ Async Iterator
→ Stream
→ Reactive Streams
→ Backpressure
→ Reactor Flux/Mono
→ Kotlin Flow
```

这是**数据流并发线**。

四条最后汇合：

```text
                 CPU / Cache
                      │
                     ASM
                      │
                 Memory Model
                      │
            Atomic / Lock-Free
                      │
OS ── Thread ── Lock ─┼──────────────┐
                      │              │
                      ↓              ↓
                 Thread Pool       Queue
                      │              │
I/O ── epoll ── Event Loop       Channel
                      │              │
                   Future          Stream
                      │              │
                  Coroutine      Reactive
                      │              │
                      └──────┬───────┘
                             ↓
                  Structured Concurrency
                             │
                             ↓
                High-Concurrency System
                             │
              ┌──────────────┴────────────┐
              ↓                           ↓
         单机并发                     分布式并发
              │                           │
       C++/Java/Rust              DB / RPC / Actor
       Kotlin/Go/...              MVCC / Consensus
```

如果目标是把这套知识真正学到能**排查 JVM/C++/Rust/Kotlin 服务的并发 bug、性能问题并能下钻到汇编**，最值得深入的交叉主线其实是：

**C++ Memory Model → x86/ARM ASM → Linux futex/epoll/io_uring → Java JMM/JUC/AQS → Kotlin Coroutine/Flow → Rust async/Tokio → Go goroutine/channel。**

这条路线会把你之前关注的 ABI、汇编、操作系统、网络、数据库和现在的并发/异步知识逐渐连成一个完整的系统软件知识图谱。