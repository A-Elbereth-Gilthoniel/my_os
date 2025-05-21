#include "heap.h"
#include "terminal.h"

#define HEAP_START ((uint32_t)&kernel_end)
#define HEAP_SIZE  (1024 * 1024)

extern uint32_t kernel_end;
block_header* heap_head = NULL;

void heap_init()
{
    heap_head = (block_header*)HEAP_START;
    heap_head->size = HEAP_SIZE - sizeof(block_header);
    heap_head->free = 1;
    heap_head->next = NULL;
}

//=======================================================

void* alloc(size_t size)
{
    block_header* current = heap_head;
    while (current)
    {
        if (current->free && current->size >= size)
        {
            if (current->size > size + sizeof(block_header))
            {
                block_header* new_block = (block_header*)((uint32_t)current + size + sizeof(block_header));
                new_block->free = 1;
                new_block->size = current->size - size - sizeof(block_header);
                new_block->next = NULL;

                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (void*)((uint32_t)current + sizeof(block_header));
        }
        current = current->next;
    }
    return NULL;
}

//==============================================

void free(void* ptr)
{
    if (!ptr) return;

    block_header* block = (block_header*)((uint32_t)ptr - sizeof(block_header));
    block->free = 1;

    block_header* current = heap_head;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += sizeof(block_header) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

//=======================================================

extern char stack_top;
static uint32_t next_stack = 0;

uint32_t* malloc_stack(size_t size)
{
    if (next_stack > MAX_THREADS)
        return NULL;
    return (uint32_t*)(&stack_top - (next_stack++) * STACK_SIZE - STACK_SIZE);
}
