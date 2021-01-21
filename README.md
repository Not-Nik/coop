# coop
Coop is an experimental asynchronous multitasking scheduler that tries to
run everywhere. It has literally no run time dependencies except for a
proper x86_64/arm64  machine set to long mode. It's only build-time
dependency is a C compiler supporting GCC inline assembly. Technically, 
coop should run, and work, on everything you can compile to, including
baremetal and even Windows.

Additionally, if you want to use coop while staying sane and stack
overflow-less you will need some memory allocation function like
`malloc()` and if you want to regain some of you precious memory
also one like `free()`.

## Usage example

```c
#include <stdio.h>
#include <stdlib.h>

#include "lib/coop.h"

void my_async_func() {
    puts("Func1: 1");
    yield;
    puts("Func1: 2");
    dump;
}

void my_async_func2() {
    puts("Func2: 1");
    yield;
    puts("Func2: 2");
    dump;
}

int main() {
    coop_context ctx = make_context();
    task_t t1 = {0}, t2 = {0};
    coop_stack s1 = {.size = 4096, .stack_bottom = malloc(4096)};
    coop_stack s2 = {.size = 4096, .stack_bottom = malloc(4096)};
    add_task(&ctx, &t1, s1, my_async_func);
    add_task(&ctx, &t2, s2, my_async_func2);
    start(&ctx);
    free(s1.stack_bottom);
    free(s2.stack_bottom);
    return 0;
}
```

## Pitfalls
> Most importantly coop isn't tested further that what's in `test.c`

coop does have two more semi-runtime dependency. Your compiler has to push
it's rbp value to the stack. If it doesn't, you will get at least the last
variable created before yielding control, or, if you don't have a variable,
the return pointer, overwritten, both of which can cause very bad things to
happen.
Additionally, the ABI you are using can't consider RAX and RDX nonvolatile
(callee-saved), since coop a) uses these registers internally and therefore
b) doesn't  save them. Luckily this is a) true for the two major ABI's and
b) nobody  would create such an ABI because these registers are used by 
both the `div` and `idiv` instruction.