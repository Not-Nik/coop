// coop (c) Nikolas Wipper 2021

#ifndef COOP__JMP_H_
#define COOP__JMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__x86_64__) || defined(_M_AMD64)
typedef unsigned long long int coop_ptr_t;
_Static_assert(sizeof(coop_ptr_t) >= 8, "unsigned long long int needs to be at least 8 bytes wide");
typedef coop_ptr_t coop_jmp_buf[17];
#else
#error Only x86_64/amd64 is supported at the moment
#endif

int coop_setjmp(coop_ptr_t *buf);
void coop_longjmp(coop_ptr_t *buf, int ret);

#ifdef __cplusplus
}
#endif

#endif //COOP__JMP_H_
