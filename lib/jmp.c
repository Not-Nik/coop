// coop (c) Nikolas Wipper 2021

#include "jmp.h"
// This is probably one of the worst pieces of code ever written
// If you have any concerns raise them, they can't be dumber than
// the monstrosity that is this code. I'm just going to leave this
// here and pray that everything just works.
// "Everything just works" is a registered trademark of Bethesda Game Studios

int coop_setjmp(coop_ptr_t *buf) {
    coop_ptr_t ret = 0;
    // We just trust that the C compiler knows
    // its ABI well enough to get our arguments
    // into rdi and our return value from rax
    asm volatile("movq %0, %%rax" : : "r" (buf));
    // This is assembly because no one bothered
    // to check what assembly does and its not
    // increasing the stack pointer
    // i.e `call _coop_setjmp_asm` != `coop_setjmp_asm()`
    // Probably black magic, but it "Works on my machine"™
    asm volatile("call _coop_setjmp_asm");
    asm volatile("movq %%rdx, %0" : : "m" (ret));
    return ret;
}

void coop_longjmp(coop_ptr_t *buf, int ret) {
    // We are loading the second argument first
    // because conveniently inline assembly puts
    // every variable it has to load into rax
    // which would overwrite buf
    asm volatile("mov %0, %%edx" : : "r" (ret));
    asm volatile("movq %0, %%rax" : : "r" (buf));
    // Here we don't care about the stack since
    // we're restoring it anyway before we use it
    // therefore a normal would be alright, but
    // you never know what crazy things can be
    // Part of an ABI
    asm volatile("jmp _coop_longjmp_asm");
}
