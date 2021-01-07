// coop (c) Nikolas Wipper 2021

#ifndef COOP__JMP_H_
#define COOP__JMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__x86_64__) || defined(_M_AMD64)
typedef unsigned long long int coop_ptr_t;
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
