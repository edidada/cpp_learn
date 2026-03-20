#include <pthread.h>
#include <iostream>

// 定义线程特定数据的键
pthread_key_t key;

// 线程启动函数
void* threadFunction(void* arg) {
    // 从线程特定数据中获取数据
    int* data = (int*)pthread_getspecific(key);

    // 检查数据是否为NULL
    if (data == NULL) {
        // 创建新的数据并存储到线程特定数据中
        data = new int(0);
        pthread_setspecific(key, data);
    }

    // 修改数据
    (*data)++;

    // 打印数据
    std::cout << "Thread ID: " << pthread_self() << ", Data: " << *data << std::endl;

    return NULL;
}

int main() {
    // 初始化线程特定数据的键
    pthread_key_create(&key, NULL);

    // 创建多个线程
    pthread_t threads[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    }

    // 等待线程退出
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // 销毁线程特定数据的键
    pthread_key_delete(key);

    return 0;
}