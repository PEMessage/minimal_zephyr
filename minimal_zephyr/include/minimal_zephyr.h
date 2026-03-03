/*
 * Copyright (c) 2024
 * Minimal Zephyr - Main Header
 */

#ifndef MINIMAL_ZEPHYR_H
#define MINIMAL_ZEPHYR_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Kernel types */
typedef uint32_t k_timeout_t;
typedef uint32_t k_tid_t;

/* Time constants */
#define K_MSEC(ms)     (ms)
#define K_SECONDS(s)   ((s) * 1000)
#define K_MINUTES(m)   ((m) * 1000 * 60)
#define K_HOURS(h)     ((h) * 1000 * 60 * 60)
#define K_FOREVER      ((k_timeout_t) -1)
#define K_NO_WAIT      ((k_timeout_t) 0)

/* Architecture specific */
#ifdef __ARM_ARCH
    #include <arch/arm/cortex_m/cmsis.h>
#endif

/* Interrupt control */
static inline void irq_enable(unsigned int irq)
{
    NVIC_EnableIRQ(irq);
}

static inline void irq_disable(unsigned int irq)
{
    NVIC_DisableIRQ(irq);
}

static inline void irq_lock(void)
{
    __asm__ volatile ("cpsid i" ::: "memory");
}

static inline void irq_unlock(void)
{
    __asm__ volatile ("cpsie i" ::: "memory");
}

/* Sleep function - minimal busy wait */
static inline void k_busy_wait(uint32_t usec)
{
    /* Simple busy wait loop */
    volatile uint32_t count = usec * (SystemCoreClock / 1000000) / 4;
    while (count--) {
        __asm__ volatile ("nop");
    }
}

/* Millisecond sleep */
static inline void k_msleep(uint32_t msec)
{
    k_busy_wait(msec * 1000);
}

/* Microsecond sleep */
static inline void k_usleep(uint32_t usec)
{
    k_busy_wait(usec);
}

/* printk - minimal implementation */
#include <arch/arm/drivers/uart.h>

static inline int printk(const char *fmt, ...)
{
    /* Minimal printk that just outputs to UART0 */
    uart_init(0, 115200);
    
    /* For now, just print the string directly if it has no format specifiers */
    const char *p = fmt;
    while (*p) {
        uart_putc(0, *p++);
    }
    
    return 0;
}

#endif /* MINIMAL_ZEPHYR_H */
