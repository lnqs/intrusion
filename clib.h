#ifndef CLIB_H
#define CLIB_H

#include <stdlib.h>
#include <sys/syscall.h>
#include <linux/sched.h>

// stdcall saves some bytes at some calls, just use it everywhere for simplicity
#define stdcall __attribute__((stdcall))

#define packed __attribute__((__packed__));

// Implementing these functions here is smaller than jumping to the libc-ones.
// Also, even though the compiler generates some overhead for the input-registers,
// the optimisations possible when it is implemented as static function, is more
// optimal than jumping to these functions implemented in pure asm in another
// object.

static stdcall void clib_exit(int code)
{
    __asm__ volatile ("int $0x80"
                      :
                      : "a" (SYS_exit_group),
                        "b" (code));
}

static stdcall void clib_clone(int (*fn)(void*), void* stack, int flags, void* data)
{
    __asm__ volatile ("subl $4,%2\n"
                      "movl %4,(%2)\n"
                      "int $0x80\n"
                      "testl %0,%0\n"
                      "jne 1f\n"
                      "call *%3\n"
                      "movl %5,%0\n"
                      "int $0x80\n"
                      "1:\n"
                      :
                      : "a" (SYS_clone),
                        "b" (flags),
                        "c" (stack),
                        "r" (fn),
                        "r" (data),
                        "r" (SYS_exit));
}

static stdcall void clib_inaccurate_memcpy(void* dest, const void* src, size_t n)
{
    // To save some instructions, only full words are copied, rest is ignored.
    // Therefore inaccurate -- while this is perfectly fine for us. This
    // function is only used by the sound-callback. It could lead to up to three
    // missing samples, what wouldn't be hearable, but even this cannot happen,
    // since the sound-buffer is divideable by four.
    __asm__ volatile ("cld\n"
                      "rep\n"
                      "movsl\n"
                      :
                      : "S" (src),
                        "D" (dest),
                        "c" (n / 4));
}

#endif

