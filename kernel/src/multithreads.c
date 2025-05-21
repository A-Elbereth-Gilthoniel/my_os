#include "multithreads.h"
#include "keyboard.h"
#include "heap.h"
#include "terminal.h"

extern void switch_context(uint32_t* old_sp, uint32_t* new_stack_base);
thread_t* current_thread = NULL;
thread_t* thread_list[MAX_THREADS];
size_t threads = 0;
extern char stack_top;

thread_t* create_thread(void (*func)(void))
{
    thread_t* new_thread = (thread_t*) alloc(sizeof(thread_t));
    new_thread->stack = malloc_stack(STACK_SIZE);
    uint32_t* stack_topp = new_thread->stack;

    *(--stack_topp) = (uint32_t) func;
    for (int i = 0; i < 8; ++i)
        *(--stack_topp) = 0;

    new_thread->stack_base = stack_topp;
    new_thread->func = func;
    new_thread->state = THREAD_READY;
    new_thread->next = NULL;
    thread_list[threads] = new_thread;
    if (threads > 0)
        thread_list[threads - 1]->next = new_thread;
    threads++;
    return new_thread;
}

//========================================================

void thread_trampoline() {
    void (*func)(void);

    __asm__ volatile ("movl %%edi, %0" : "=r"(func));
    func();
    thread_exit();
}

//========================================================

void thread_exit() {
    current_thread->state = THREAD_TERMINATED;
    schedule();
    while (1);
}

//========================================================

void schedule()
{
    thread_t* prev = current_thread;
    current_thread = current_thread->next ? current_thread->next : thread_list[0];
    if (current_thread)
    {
        current_thread->state = THREAD_RUNNING;
        switch_context(&prev->stack_base, current_thread->stack_base);
    }
}

//=========================================================

void thread_start()
{
    current_thread = thread_list[0];
    current_thread->state = THREAD_RUNNING;
    switch_context(&stack_top, thread_list[0]->stack_base);
}
