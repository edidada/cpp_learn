#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define ALIGN(size) (((size) + (sizeof(void*) - 1)) & ~(sizeof(void*) - 1))
#define BLOCK_SIZE sizeof(struct block)

struct block {
    size_t size;
    struct block *next;
    bool free;
    char data[1];  // flexible array member, C99 feature
};

static struct block *free_list = NULL;

// 查找合适大小的空闲块
static struct block *find_free_block(struct block **last, size_t size) {
    struct block *current = free_list;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

// 扩展堆空间，分配新的块
static struct block *extend_heap(struct block *last, size_t size) {
    struct block *block;
    block = sbrk(0);
    void *request = sbrk(size + BLOCK_SIZE);
    if (request == (void*) -1) {
        return NULL;  // sbrk 失败
    }

    // 初始化新块
    if (last) {
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = false;
    return block;
}

void *malloc(size_t size) {
    struct block *block;
    if (size <= 0) {
        return NULL;
    }

    // 对齐请求的内存大小
    size = ALIGN(size);

    // 如果有空闲块，查找合适的空闲块
    if (free_list) {
        struct block *last = free_list;
        block = find_free_block(&last, size);
        if (block) {
            block->free = false;
            return block->data;
        }

        // 没有合适的空闲块，扩展堆
        block = extend_heap(last, size);
        if (!block) {
            return NULL;
        }
    } else {
        // 初次分配
        block = extend_heap(NULL, size);
        if (!block) {
            return NULL;
        }
        free_list = block;
    }

    return block->data;
}

void free(void *ptr) {
    if (!ptr) {
        return;
    }

    struct block *block = (struct block*)((char*)ptr - offsetof(struct block, data));

    block->free = true;

    // 合并相邻的空闲块
    struct block *current = free_list;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

int main() {
    // 测试 malloc 和 free
    int *array = (int *)malloc(10 * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);

    return 0;
}
