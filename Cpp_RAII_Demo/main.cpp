#include <cstdio>
#include <iostream>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

namespace demo {

int g_log_counter = 0;

// 统一日志函数：给每条日志加递增序号，便于观察资源获取与释放顺序。
void log(const std::string& message) {
    std::cout << "[" << ++g_log_counter << "] " << message << '\n';
}

void print_section(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

// -----------------------------------------------------------------------------
// 1. RAII 管理动态内存
// -----------------------------------------------------------------------------
//
// UniqueArray<T> 是一个简化版“智能指针”示例，用来演示最核心的 RAII 思路：
// - 构造函数中使用 new[] 获取动态内存
// - 析构函数中使用 delete[] 自动释放动态内存
// - 禁止拷贝，避免两个对象指向同一块内存并重复释放
// - 支持移动，把资源所有权安全转移给另一个对象
//
// 这个类不追求完整功能，只强调教学场景下的“资源拥有者”语义。
template <typename T>
class UniqueArray final {
public:
    explicit UniqueArray(std::size_t size)
        : size_(size), data_(size == 0 ? nullptr : new T[size]{}) {
        log("UniqueArray 构造：申请 " + std::to_string(size_) +
            " 个元素的动态内存，地址 = " + pointer_to_string());
    }

    ~UniqueArray() {
        release();
    }

    UniqueArray(const UniqueArray&) = delete;
    UniqueArray& operator=(const UniqueArray&) = delete;

    UniqueArray(UniqueArray&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        log("UniqueArray 移动构造：资源所有权已转移，新的地址 = " + pointer_to_string());
    }

    UniqueArray& operator=(UniqueArray&& other) noexcept {
        if (this != &other) {
            release();
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            log("UniqueArray 移动赋值：旧资源已清理，新资源地址 = " + pointer_to_string());
        }
        return *this;
    }

    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("UniqueArray 下标越界");
        }
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("UniqueArray 下标越界");
        }
        return data_[index];
    }

    std::size_t size() const noexcept {
        return size_;
    }

private:
    std::string pointer_to_string() const {
        if (data_ == nullptr) {
            return "nullptr";
        }
        return std::to_string(reinterpret_cast<std::uintptr_t>(data_));
    }

    void release() noexcept {
        if (data_ != nullptr) {
            log("UniqueArray 析构：释放动态内存，地址 = " + pointer_to_string());
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
        } else {
            log("UniqueArray 析构：当前对象不拥有资源，无需释放");
        }
    }

    std::size_t size_ = 0;
    T* data_ = nullptr;
};

// -----------------------------------------------------------------------------
// 2. RAII 管理文件
// -----------------------------------------------------------------------------
//
// FileGuard 封装了 C 风格文件句柄 FILE*：
// - 构造时 fopen 获取文件资源
// - 析构时 fclose 自动关闭文件
// - 即使中途抛异常，也能保证文件被关闭
//
// 这里故意不用 std::ofstream，而是直接封装 FILE*，让“获取/释放资源”的
// 逻辑更直观，便于教学观察。
class FileGuard final {
public:
    FileGuard(std::string path, const char* mode)
        : path_(std::move(path)), file_(std::fopen(path_.c_str(), mode)) {
        if (file_ == nullptr) {
            throw std::runtime_error("FileGuard 构造失败：无法打开文件 " + path_);
        }
        log("FileGuard 构造：文件已打开 -> " + path_);
    }

    ~FileGuard() {
        close();
    }

    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    FileGuard(FileGuard&& other) noexcept
        : path_(std::move(other.path_)), file_(other.file_) {
        other.file_ = nullptr;
        log("FileGuard 移动构造：文件所有权已转移 -> " + path_);
    }

    FileGuard& operator=(FileGuard&& other) noexcept {
        if (this != &other) {
            close();
            path_ = std::move(other.path_);
            file_ = other.file_;
            other.file_ = nullptr;
            log("FileGuard 移动赋值：文件所有权已转移 -> " + path_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        if (file_ == nullptr) {
            throw std::runtime_error("FileGuard 写入失败：文件未打开");
        }
        std::fputs(text.c_str(), file_);
        std::fputc('\n', file_);
        std::fflush(file_);
        log("FileGuard 写入：\"" + text + "\"");
    }

private:
    void close() noexcept {
        if (file_ != nullptr) {
            log("FileGuard 析构：自动关闭文件 -> " + path_);
            std::fclose(file_);
            file_ = nullptr;
        } else {
            log("FileGuard 析构：当前对象不拥有文件句柄，无需关闭");
        }
    }

    std::string path_;
    std::FILE* file_ = nullptr;
};

// -----------------------------------------------------------------------------
// 3. RAII 管理互斥锁
// -----------------------------------------------------------------------------
//
// MutexLockGuard 模拟 std::lock_guard / std::unique_lock 的核心思想：
// - 构造函数中 lock()
// - 析构函数中 unlock()
// - 即使临界区抛出异常，也能自动解锁
//
// 对锁这类“必须成对调用”的资源，RAII 能显著降低遗漏 unlock() 的风险。
class MutexLockGuard final {
public:
    explicit MutexLockGuard(std::mutex& mutex, std::string name = "mutex")
        : mutex_(&mutex), name_(std::move(name)), owns_lock_(true) {
        log("MutexLockGuard 构造：准备加锁 -> " + name_);
        mutex_->lock();
        log("MutexLockGuard 构造：已加锁 -> " + name_);
    }

    ~MutexLockGuard() {
        unlock();
    }

    MutexLockGuard(const MutexLockGuard&) = delete;
    MutexLockGuard& operator=(const MutexLockGuard&) = delete;

    MutexLockGuard(MutexLockGuard&& other) noexcept
        : mutex_(other.mutex_), name_(std::move(other.name_)), owns_lock_(other.owns_lock_) {
        other.mutex_ = nullptr;
        other.owns_lock_ = false;
        log("MutexLockGuard 移动构造：锁的释放责任已转移 -> " + name_);
    }

    MutexLockGuard& operator=(MutexLockGuard&& other) noexcept {
        if (this != &other) {
            unlock();
            mutex_ = other.mutex_;
            name_ = std::move(other.name_);
            owns_lock_ = other.owns_lock_;
            other.mutex_ = nullptr;
            other.owns_lock_ = false;
            log("MutexLockGuard 移动赋值：锁的释放责任已转移 -> " + name_);
        }
        return *this;
    }

private:
    void unlock() noexcept {
        if (mutex_ != nullptr && owns_lock_) {
            log("MutexLockGuard 析构：自动解锁 -> " + name_);
            mutex_->unlock();
            owns_lock_ = false;
        } else {
            log("MutexLockGuard 析构：当前对象不负责解锁");
        }
    }

    std::mutex* mutex_ = nullptr;
    std::string name_;
    bool owns_lock_ = false;
};

// -----------------------------------------------------------------------------
// 4. RAII 管理 socket（模拟）
// -----------------------------------------------------------------------------
//
// 真实项目中的 socket 可能来自 socket()/close()、accept()/close() 等系统调用。
// 为了保证示例可跨平台直接编译运行，这里用 FakeSocketSystem 模拟 socket 资源：
// - open() 返回一个“句柄编号”
// - close() 负责释放句柄
// - SocketGuard 在构造/析构中自动管理这个句柄
//
// 重点不在网络通信本身，而在“句柄必须最终被关闭”的 RAII 思想。
class FakeSocketSystem {
public:
    static int open(const std::string& endpoint) {
        const int handle = next_handle_++;
        open_handles_.insert(handle);
        log("FakeSocketSystem：打开 socket，endpoint = " + endpoint +
            "，handle = " + std::to_string(handle));
        return handle;
    }

    static void close(int handle) {
        const auto erased = open_handles_.erase(handle);
        if (erased > 0) {
            log("FakeSocketSystem：关闭 socket，handle = " + std::to_string(handle));
        } else {
            log("FakeSocketSystem：handle = " + std::to_string(handle) +
                " 已关闭，忽略重复关闭请求");
        }
    }

    static void send(int handle, const std::string& payload) {
        if (open_handles_.count(handle) == 0) {
            throw std::runtime_error("FakeSocketSystem 发送失败：socket 已关闭");
        }
        log("FakeSocketSystem：发送数据，handle = " + std::to_string(handle) +
            "，payload = \"" + payload + "\"");
    }

    static std::size_t open_count() {
        return open_handles_.size();
    }

private:
    static inline int next_handle_ = 1000;
    static inline std::set<int> open_handles_;
};

class SocketGuard final {
public:
    explicit SocketGuard(std::string endpoint)
        : endpoint_(std::move(endpoint)), handle_(FakeSocketSystem::open(endpoint_)) {
        log("SocketGuard 构造：已获取 socket 句柄 -> " + std::to_string(handle_));
    }

    ~SocketGuard() {
        close();
    }

    SocketGuard(const SocketGuard&) = delete;
    SocketGuard& operator=(const SocketGuard&) = delete;

    SocketGuard(SocketGuard&& other) noexcept
        : endpoint_(std::move(other.endpoint_)), handle_(other.handle_) {
        other.handle_ = invalid_handle();
        log("SocketGuard 移动构造：socket 所有权已转移，handle = " +
            std::to_string(handle_));
    }

    SocketGuard& operator=(SocketGuard&& other) noexcept {
        if (this != &other) {
            close();
            endpoint_ = std::move(other.endpoint_);
            handle_ = other.handle_;
            other.handle_ = invalid_handle();
            log("SocketGuard 移动赋值：socket 所有权已转移，handle = " +
                std::to_string(handle_));
        }
        return *this;
    }

    void send(const std::string& payload) const {
        if (handle_ == invalid_handle()) {
            throw std::runtime_error("SocketGuard 发送失败：无有效 socket");
        }
        FakeSocketSystem::send(handle_, payload);
    }

private:
    static constexpr int invalid_handle() noexcept {
        return -1;
    }

    void close() noexcept {
        if (handle_ != invalid_handle()) {
            log("SocketGuard 析构：自动关闭 socket，handle = " + std::to_string(handle_));
            FakeSocketSystem::close(handle_);
            handle_ = invalid_handle();
        } else {
            log("SocketGuard 析构：当前对象不拥有 socket，无需关闭");
        }
    }

    std::string endpoint_;
    int handle_ = invalid_handle();
};

void demonstrate_memory_raii() {
    print_section("案例 1：RAII 管理动态内存");

    try {
        UniqueArray<int> numbers(5);
        for (std::size_t i = 0; i < numbers.size(); ++i) {
            numbers[i] = static_cast<int>(i * 10);
            log("numbers[" + std::to_string(i) + "] = " + std::to_string(numbers[i]));
        }

        UniqueArray<int> moved_numbers(std::move(numbers));
        log("已把动态内存所有权移动到 moved_numbers");

        log("准备抛出异常，观察是否仍会自动释放内存");
        throw std::runtime_error("模拟异常：内存使用过程中发生错误");
    } catch (const std::exception& ex) {
        log(std::string("捕获异常：") + ex.what());
    }

    log("离开作用域后，动态内存已经自动释放，没有泄漏");
}

void demonstrate_file_raii() {
    print_section("案例 2：RAII 管理文件自动关闭");

    try {
        FileGuard file("raii_demo_output.txt", "w");
        file.write_line("第一行：RAII 会在析构时自动关闭文件");

        {
            FileGuard moved_file(std::move(file));
            moved_file.write_line("第二行：文件所有权已移动到 moved_file");
            log("内部作用域即将结束，moved_file 会自动关闭文件");
        }

        log("原 file 已失去所有权，离开外层作用域时不会重复关闭");
    } catch (const std::exception& ex) {
        log(std::string("捕获异常：") + ex.what());
    }

    log("文件示例结束：即使忘记手写 fclose()，RAII 也会自动关闭");
}

void demonstrate_mutex_raii() {
    print_section("案例 3：RAII 管理互斥锁自动解锁");

    std::mutex mutex;

    try {
        MutexLockGuard guard(mutex, "demo_mutex");
        log("进入临界区：此处可以安全访问共享资源");
        log("准备抛出异常，观察锁是否仍会自动释放");
        throw std::runtime_error("模拟异常：临界区逻辑失败");
    } catch (const std::exception& ex) {
        log(std::string("捕获异常：") + ex.what());
    }

    log("再次进入临界区，证明上一个作用域结束时已经自动解锁");
    {
        MutexLockGuard guard(mutex, "demo_mutex");
        MutexLockGuard moved_guard(std::move(guard));
        log("锁的释放责任已移动到 moved_guard");
    }

    log("锁示例结束：离开作用域后自动 unlock()");
}

void demonstrate_socket_raii() {
    print_section("案例 4：RAII 管理 socket（模拟）");

    try {
        SocketGuard socket("127.0.0.1:8080");
        socket.send("hello");

        SocketGuard moved_socket(std::move(socket));
        moved_socket.send("ownership moved");

        log("准备抛出异常，观察 socket 句柄是否仍会自动关闭");
        throw std::runtime_error("模拟异常：网络处理失败");
    } catch (const std::exception& ex) {
        log(std::string("捕获异常：") + ex.what());
    }

    log("当前仍打开的 socket 数量 = " + std::to_string(FakeSocketSystem::open_count()));
    log("socket 示例结束：句柄随作用域结束自动释放");
}

}  // namespace demo

int main() {
    using namespace demo;

    print_section("RAII 教学示例开始");
    log("RAII = Resource Acquisition Is Initialization（资源获取即初始化）");
    log("核心思想：构造函数获取资源，析构函数释放资源");
    log("依赖 C++ 栈对象离开作用域时自动析构，实现异常安全、自动释放、无泄漏");
    log("典型资源包括：内存、文件、锁、socket、各种系统句柄");

    demonstrate_memory_raii();
    demonstrate_file_raii();
    demonstrate_mutex_raii();
    demonstrate_socket_raii();

    print_section("RAII 教学示例结束");
    log("所有案例运行完成：可以从日志中观察每个资源的构造/析构时机");
    return 0;
}
