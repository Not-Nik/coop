// coop (c) Nikolas Wipper 2021

#define COOP_IMPL
#include "coop.h"

task_t *current_task = 0;

task_t *get_last(task_t *first) {
    if (first->next == 0 || first->next == first) {
        return first;
    }
    return get_last(first->next);
}

coop_context make_context() {
    return (coop_context){0};
}

void add_task(coop_context *ctx, task_t *buf, coop_stack stack, void (*call)()) {
    buf->call = call;
    buf->stack_base = (coop_ptr_t) (stack.stack_bottom + stack.size);
    if (!ctx->first_task) {
        ctx->first_task = buf;
    }
    task_t *last = get_last(ctx->first_task);
    buf->last = last;
    buf->next = ctx->first_task;
    last->next = buf;
    ctx->first_task->last = buf;
}

void start(coop_context *ctx) {
    while (ctx->first_task) {
        for (current_task = ctx->first_task; current_task; current_task = current_task->next) {
            if (pursue_task() <= 1) continue;

            if (current_task->next == current_task) {
                ctx->first_task = 0;
                break;
            }

            task_t *last = current_task->last;
            task_t *next = current_task->next;

            if (ctx->first_task == current_task) {
                ctx->first_task = next;
            }

            last->next = next;
            next->last = last;
        }
    }
}

int pursue_task() {
    int i = coop_setjmp(current_task->ctx_env);
    if (!i) {
        if (!current_task->was_called) {
            current_task->was_called = true;
            stack_pointer = current_task->stack_base;
            current_task->call();
        } else {
            coop_longjmp(current_task->func_env, 1);
        }
    } else {
        return i;
    }
    return 0;
}
