#include "macros.h"

typedef struct block_header {
    size_t size;
    uint8_t free;
    struct block_header* next;
} block_header;

void heap_init();
void* alloc(size_t size);
void free(void* ptr);
uint32_t* malloc_stack(size_t size);

#define STACK_SIZE 4096
#define MAX_THREADS 4
