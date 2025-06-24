#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h> // for sleep function

#define MAX_ITEMS 10
int buffer[MAX_ITEMS];
int itemCount = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int item;
    while(1) {
        item = rand() % 100; // Generate a random item
        sem_wait(&empty); // Wait for an empty slot in the buffer
        pthread_mutex_lock(&mutex);

        buffer[itemCount++] = item;
        printf("Producer produced: %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Signal that there is now one more full slot

        sleep(1); // Simulate time taken to produce another item
    }
}

void* consumer(void* arg) {
    int item;
    while(1) {
        sem_wait(&full); // Wait for at least one item in the buffer
        pthread_mutex_lock(&mutex);

        item = buffer[--itemCount];
        printf("Consumer consumed: %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Signal that there is now one more empty slot

        sleep(1); // Simulate time taken to consume the item
    }
}

int main() {
    pthread_t tid1, tid2;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, MAX_ITEMS); // Initially all slots are empty
    sem_init(&full, 0, 0);          // Initially no items in the buffer
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);

    // Join threads (this will not return as threads run infinitely)
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destroy semaphores and mutex when done
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}