// coop (c) Nikolas Wipper 2021

#define COOP_IMPL
#include "coop.h"

task_t *current_task = 0;

task_t *get_last(task_t *first) {
    task_t *curr = first;
    while (curr->next != first) {
        curr = curr->next;
    }
    return curr;
}

void add_task(coop_context *ctx, task_t *buf, coop_stack stack, void (*call)()) {
    buf->call = call;
    buf->stack_base = (coop_ptr_t) (stack.stack_bottom + stack.size);
    if (!*ctx) {
        *ctx = buf;
    }
    buf->next = *ctx;
    (*ctx)->prev = buf;
    task_t *last = get_last(*ctx);
    buf->prev = last;
    last->next = buf;
}

void start(coop_context *ctx) {
    current_task = *ctx;
    for (;;) {
        // Run the `current_task`
        int i = coop_setjmp(current_task->ctx_env);
        if (!i) {
            if (!current_task->was_called) {
                current_task->was_called = true;
                stack_pointer = current_task->stack_base;
                current_task->call();
            } else {
                coop_longjmp(current_task->func_env, 1);
            }
        }
        // Check whether it wants to dump
        if (i > 1) {
            if (current_task->next == current_task)
                break;

            task_t *prev = current_task->prev;
            task_t *next = current_task->next;

            if (*ctx == current_task)
                *ctx = next; //The iteration part is rather "trivial", we start by setting a global variable

            prev->next = next;
            next->prev = prev;
        }
        current_task = current_task->next;
    }
}
