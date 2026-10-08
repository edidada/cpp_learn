#include <pthread.h>
#include <iostream>
#include <cstdlib>  // for exit
using namespace std;

static long long total = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* func(void*)
{
    long long i;
    for(i = 0; i < 100000000LL; i++)
    {
        pthread_mutex_lock(&m);
        total += i;
        pthread_mutex_unlock(&m);
    }
    return nullptr;
}

int main()
{
    pthread_t thread1, thread2;

    int ret1 = pthread_create(&thread1, NULL, &func, NULL);
    if (ret1 != 0)
    {
        cerr << "Error creating thread 1: " << ret1 << endl;
        exit(EXIT_FAILURE);
    }

    int ret2 = pthread_create(&thread2, NULL, &func, NULL);
    if (ret2 != 0)
    {
        cerr << "Error creating thread 2: " << ret2 << endl;
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    cout << "Total: " << total << endl; // 期望值约为 2 * (0 到 99999999 的和)
    return 0;
}