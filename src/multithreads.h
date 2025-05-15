#include "macros.h"

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_TERMINATED
} thread_state_t;

typedef struct thread {
    uint32_t* stack;
    uint32_t* stack_base;
    void (*func)(void);
    thread_state_t state;
    struct thread* next;
} thread_t;

thread_t* create_thread(void (*func)(void));
void schedule();
void thread_start();
