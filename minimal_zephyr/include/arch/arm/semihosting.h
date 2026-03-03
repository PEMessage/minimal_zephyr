/*
 * Copyright (c) 2024
 * Minimal Zephyr - Semihosting Output for Debugging
 */

#ifndef MINIMAL_SEMIHOSTING_H
#define MINIMAL_SEMIHOSTING_H

#include <stdint.h>

/* Semihosting SWI numbers */
#define SEMIHOSTING_SYS_WRITE0  0x04  /* Write null-terminated string */
#define SEMIHOSTING_SYS_WRITE   0x05  /* Write to file */

/* Perform semihosting call */
static inline int semihosting_call(int operation, void *message)
{
    int result;
    __asm__ volatile (
        "mov r0, %1\n"
        "mov r1, %2\n"
        "bkpt 0xAB\n"  /* Semihosting breakpoint */
        "mov %0, r0\n"
        : "=r" (result)
        : "r" (operation), "r" (message)
        : "r0", "r1", "r2", "r3", "memory"
    );
    return result;
}

/* Write string using semihosting */
static inline void semihosting_puts(const char *str)
{
    semihosting_call(SEMIHOSTING_SYS_WRITE0, (void *)str);
}

/* Simple semihosting-based printk */
static inline int semihosting_printk(const char *fmt, ...)
{
    (void)fmt;
    semihosting_puts("Hello from Minimal Zephyr!\n");
    return 0;
}

#endif /* MINIMAL_SEMIHOSTING_H */
