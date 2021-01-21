// coop (c) Nikolas Wipper 2021

#include <stdio.h>
#include <stdlib.h>

#include "lib/coop.h"

void my_nested_async_func() {
    int i = 21;
    puts("Nested1: 1");
    yield;
    printf("Nested1: Saved %d in stack\n", i);
}

void my_nested_async_func2() {
    int i = 22;
    puts("Nested2: 1");
    yield;
    printf("Nested2: Saved %d in stack\n", i);
}

void my_async_func() {
    int i = 11;
    puts("Func1: 1");
    yield;
    printf("Func1: Saved %d in stack\n", i);
    yield;
    my_nested_async_func();
    yield;
    puts("Func1: Ran nested function");
    dump;
}

void my_async_func2() {
    int i = 12;
    puts("Func2: 1");
    yield;
    printf("Func2: Saved %d in stack\n", i);
    yield;
    my_nested_async_func2();
    yield;
    puts("Func2: Ran nested function");
    dump;
}

int main() {
    coop_context ctx = {0};
    task_t t1 = {0}, t2 = {0};
    coop_stack s1 = {.size = 4096, .stack_bottom = malloc(4096)};
    coop_stack s2 = {.size = 4096, .stack_bottom = malloc(4096)};
    add_task(&ctx, &t1, s1, my_async_func);
    add_task(&ctx, &t2, s2, my_async_func2);
    start(&ctx);
    puts("Ran tasks");
    free(s1.stack_bottom);
    free(s2.stack_bottom);
    return 0;
}
