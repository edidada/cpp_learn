# ---------------------------------------------------------------------------
# src/multithread —— docs/multy_thread.md 对应知识点演示
#
# 这批文件是"同一份代码跨标准复用"的：每个 demo 内部用 #if 做能力探测，
# 低标准下 C++11/17/20 段落自动变成打印 [SKIP] 的惰性分支，代码不删。
# 因此这里不硬编码标准号，而是问编译器"你最高支持到哪一档"，
# 于是 cpp98 分支和 cpp26 分支上 merge 同一份文件都能拿到各自最好的效果。
#
# 想固定某一档做对比实验（比如故意在 C++17 下看哪些段落消失）：
#   cmake -DMT_CXX_STANDARD=17 ...
# ---------------------------------------------------------------------------

include(CheckCXXCompilerFlag)

# 1) 决定用哪个语言级别编译这批 demo
#
# 这个仓库是「一个标准一个分支」的，同一份演示文件从 cpp98 一路 merge 到 cpp26。
# 所以默认档位不是"编译器最高支持多少"，而是"我现在站在哪个分支上"：
#   读当前 git 分支名 -> cpp98 / cpp11 / cpp17 / cpp20_linux / cpp23_win ... 里的数字
# 这样 merge 到 cpp20 分支时，同一份文件里的 C++20 段落自动点亮，
# 而在 cpp98 分支上它们保持 [SKIP]，正好把"这个特性是哪一年进来的"演出来。
# 分支名不含 cppNN（比如 main、或者源码包没有 .git）时，退回"编译器最高档"。
#
# 想手工指定做对比实验：cmake -DMT_CXX_STANDARD=17 ...
if(NOT MT_CXX_STANDARD)
    set(MT_CXX_STANDARD "")
    find_program(MT_GIT_EXECUTABLE git)
    if(MT_GIT_EXECUTABLE)
        execute_process(COMMAND "${MT_GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
                        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                        OUTPUT_VARIABLE MT_BRANCH
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET)
        if(MT_BRANCH MATCHES "cpp(98|03|11|14|17|20|23|26)")
            set(MT_CXX_STANDARD "${CMAKE_MATCH_1}")
            if(MT_CXX_STANDARD STREQUAL "03")
                set(MT_CXX_STANDARD 98)   # CMake 没有 03 这一档，gnu++98 足够代表 C++03
            endif()
            set(MT_STD_REASON "from git branch '${MT_BRANCH}'")
        endif()
    endif()
    if(NOT MT_CXX_STANDARD)
        if(MSVC)
            # MSVC 没有 c++98/c++03 模式，最高给到当前工具链支持的档位
            if(MSVC_VERSION GREATER_EQUAL 1928)
                set(MT_CXX_STANDARD 20)
            else()
                set(MT_CXX_STANDARD 14)
            endif()
            set(MT_STD_REASON "auto: MSVC ${MSVC_VERSION}")
        else()
            set(MT_CXX_STANDARD 98)
            foreach(_std 26 23 20 17 14 11 98)
                check_cxx_compiler_flag("-std=c++${_std}" MT_FLAG_${_std})
                if(MT_FLAG_${_std})
                    set(MT_CXX_STANDARD ${_std})
                    break()
                endif()
            endforeach()
            set(MT_STD_REASON "auto: highest -std=c++NN accepted by ${CMAKE_CXX_COMPILER_ID}")
        endif()
    endif()
endif()

# 2) 线程库：Linux/WSL 要 -pthread，MinGW 要 winpthread，MSVC 不需要
find_package(Threads REQUIRED)

set(MT_DEMOS
    mt_00_map
    mt_01_lost_update
    mt_02_memory_orders
    mt_03_sync_primitives
    mt_04_correctness
    mt_05_lockfree
    mt_06_false_sharing
    mt_07_future_promise
    mt_08_latch_barrier_semaphore
    mt_09_thread_pool
    mt_10_coroutine
    mt_11_event_loop
    mt_12_channel_backpressure
    mt_13_distributed_sketch
)

message(STATUS "multithread demos: -std=c++${MT_CXX_STANDARD} (${MT_STD_REASON})")

foreach(_demo ${MT_DEMOS})
    add_executable(${_demo} ${CMAKE_SOURCE_DIR}/src/multithread/${_demo}.cpp)
    set_target_properties(${_demo} PROPERTIES
        CXX_STANDARD ${MT_CXX_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON            # __sync builtins / long long 需要 GNU 扩展档
    )
    target_include_directories(${_demo} PRIVATE ${CMAKE_SOURCE_DIR}/src/multithread)
    target_link_libraries(${_demo} PRIVATE Threads::Threads)
    if(MSVC)
        # MSVC 的 __cplusplus 默认不更新，演示代码里的能力探测要用 /Zc:__cplusplus
        target_compile_options(${_demo} PRIVATE /Zc:__cplusplus)
        # Debug 档自带 /RTC1，和 /O2 互斥（cl 会直接 D8016 报错），所以只在非 Debug 加优化
        if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(${_demo} PRIVATE /O2)
        endif()
    else()
        # 内存序实验必须开优化：-O0 时 store buffer 的重排窗口被无关指令淹没，
        # Dekker/StoreLoad 现象观察不到（实测 -O0 三万轮 0 次，-O2 约 20% 复现）。
        # 追加在构建类型之后，GCC/Clang 以最后一个 -O 为准。
        target_compile_options(${_demo} PRIVATE -O2)
    endif()
endforeach()

# 跑一遍全部演示（ctest 友好）：ctest -R multithread
if(NOT DEFINED MT_ADD_TESTS)
    set(MT_ADD_TESTS ON)
endif()
if(MT_ADD_TESTS)
    enable_testing()
    foreach(_demo ${MT_DEMOS})
        add_test(NAME multithread_${_demo} COMMAND ${_demo})
    endforeach()
endif()
