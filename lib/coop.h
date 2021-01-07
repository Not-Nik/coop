// coop (c) Nikolas Wipper 2021

#ifndef COOP_LIB_COOP_H_
#define COOP_LIB_COOP_H_

#ifdef __cplusplus
extern "C" {
#else
#if __bool_true_false_are_defined != 1
typedef enum { false, true } bool;
#endif
#endif

#include "jmp.h"

#if defined(__x86_64__) || defined(_M_AMD64)
register coop_ptr_t stack_pointer asm ("rsp");
#else
#error Only x86_64/amd64 is supported at the moment
#endif

/**
 * @brief Holds all the information to execute one task asynchronously
 */
typedef struct task {
    void (*call)();
    bool was_called;
    coop_ptr_t stack_base;
    coop_jmp_buf func_env;
    coop_jmp_buf ctx_env;
    struct task *next;
    struct task *last;
} task_t;

task_t *get_last(task_t *first);

#ifndef COOP_IMPL
extern task_t *current_task;
#endif

/**
 * @brief Scheduler for several tasks
 */
typedef struct {
    task_t *first_task;
} coop_context;

typedef struct {
    coop_ptr_t size;
    void *stack_bottom; // As acquired by malloc(size)
} coop_stack;

coop_context make_context();

/**
 * @brief Adds a task to the task queue
 */
void add_task(coop_context *ctx, task_t *buf, coop_stack stack, void(*call)());

/**
 * @brief Runs all the task asynchronously
 *
 * @note This function won't return before all the tasks have finished
 */
void start(coop_context *ctx);

/**
 * @brief Runs a task until it yields control
 */
int pursue_task();

#define yield if (!coop_setjmp(current_task->func_env))coop_longjmp(current_task->ctx_env, 1)
#define exit if (!coop_setjmp(current_task->func_env))coop_longjmp(current_task->ctx_env, 2)

#ifdef __cplusplus
}
#endif

#endif //COOP_LIB_COOP_H_
