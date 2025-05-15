#include "multithreads.h"
#include "keyboard.h"
#include "heap.h"
#include "terminal.h"
extern terminal monitor;

extern void switch_context(uint32_t* old_sp, uint32_t* new_stack_base);
thread_t* current_thread = NULL;
thread_t* thread_list[MAX_THREADS];
size_t threads = 0;
extern char stack_top;

thread_t* create_thread(void (*func)(void))
{
    terminal_writestring(&monitor, "create_thread():\n");
    // get_esp();
    thread_t* new_thread = (thread_t*) alloc(sizeof(thread_t));
    new_thread->stack = malloc_stack(STACK_SIZE);
    // terminal_writestring(&monitor, uint_to_string(new_thread->stack));
    // terminal_writestring(&monitor, "\n");
    uint32_t* stack_topp = new_thread->stack; //+ STACK_SIZE / sizeof(uint32_t);
    // get_esp();

    *(--stack_topp) = (uint32_t) func;
    for (int i = 0; i < 8; ++i)
        *(--stack_topp) = 0;                     // или --stack_top

    // *(--stack_topp) = (uint32_t) func;

    // *(--stack_top) = (uint32_t) thread_trampoline;

    new_thread->stack_base = stack_topp;
    // terminal_writestring(&monitor, uint_to_string(new_thread->stack));
    // terminal_writestring(&monitor, "\n");

    new_thread->func = func;
    terminal_writestring(&monitor, "adress\n");
    terminal_writestring(&monitor, uint_to_string(new_thread->func));
    terminal_writestring(&monitor, "\n");
    terminal_writestring(&monitor, uint_to_string(func));
    new_thread->state = THREAD_READY;
    new_thread->next = NULL;
    thread_list[threads] = new_thread;
    if (threads > 0)
        thread_list[threads - 1]->next = new_thread;
    threads++;
    // get_esp();
    terminal_writestring(&monitor, "end of create_thread()\n");
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
    // current_thread = thread_list[1];
    if (!prev->next)
        terminal_writestring(&monitor, "porazenie\n");
    if (current_thread)
    {
        current_thread->state = THREAD_RUNNING;
        terminal_writestring(&monitor, "\nTh2\n");
        terminal_writestring(&monitor, uint_to_string(current_thread->func));
        terminal_writestring(&monitor, "\n");
        terminal_writestring(&monitor, uint_to_string(&thread2));
        terminal_writestring(&monitor, "\n");
        terminal_writestring(&monitor, uint_to_string(prev->func));
        terminal_writestring(&monitor, "\n");
        terminal_writestring(&monitor, uint_to_string(&thread1));
        // thread2();
        switch_context(prev->stack, current_thread->stack_base);
        terminal_writestring(&monitor, "Th33\n");
    }
}

//=========================================================

void thread_start()
{
    terminal_writestring(&monitor, "thread_start:\n");
    current_thread = thread_list;
    current_thread->state = THREAD_RUNNING;
    // get_esp();
    // terminal_writestring(&monitor, "attention:\n");
    // terminal_writestring(&monitor, uint_to_string(&thread_list[0]->stack));
    // terminal_writestring(&monitor, "attention:\n");
    // terminal_writestring(&monitor, uint_to_string(thread_list[0]->stack));
    switch_context(get_esp(), thread_list[0]->stack_base);
    // get_esp();
    // terminal_writestring(&monitor, "aaaaa\n");
}
