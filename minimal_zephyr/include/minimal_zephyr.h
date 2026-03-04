/*
 * Copyright (c) 2024
 * Minimal Zephyr - Main Header (Architecture Agnostic)
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

/* Architecture-specific includes */
#if defined(CONFIG_ARCH_ARM) || defined(__ARM_ARCH)
    #include <arch/arm/cortex_m/cmsis.h>
    
    /* ARM Interrupt control */
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
    
    /* Sleep function - ARM busy wait */
    static inline void k_busy_wait(uint32_t usec)
    {
        volatile uint32_t count = usec * (SystemCoreClock / 1000000) / 4;
        while (count--) {
            __asm__ volatile ("nop");
        }
    }

#elif defined(CONFIG_ARCH_RISCV) || defined(__riscv)
    /* RISC-V CSR operations */
    #define csr_read(csr)                                           \
        ({                                                          \
            uint32_t __v;                                           \
            __asm__ volatile ("csrr %0, " #csr : "=r"(__v));       \
            __v;                                                    \
        })

    #define csr_write(csr, val)                                     \
        ({                                                          \
            uint32_t __v = (uint32_t)(val);                         \
            __asm__ volatile ("csrw " #csr ", %0" : : "r"(__v));   \
        })

    #define csr_set(csr, val)                                       \
        ({                                                          \
            uint32_t __v = (uint32_t)(val);                         \
            __asm__ volatile ("csrs " #csr ", %0" : : "r"(__v));   \
        })

    #define csr_clear(csr, val)                                     \
        ({                                                          \
            uint32_t __v = (uint32_t)(val);                         \
            __asm__ volatile ("csrc " #csr ", %0" : : "r"(__v));   \
        })

    /* RISC-V Interrupt control */
    static inline void irq_enable(unsigned int irq __attribute__((unused)))
    {
        /* Enable global interrupts (MIE bit in mstatus) */
        csr_set(mstatus, 0x8);
    }
    
    static inline void irq_disable(unsigned int irq __attribute__((unused)))
    {
        /* Disable global interrupts (MIE bit in mstatus) */
        csr_clear(mstatus, 0x8);
    }
    
    static inline void irq_lock(void)
    {
        csr_clear(mstatus, 0x8);  /* Clear MIE bit */
    }
    
    static inline void irq_unlock(void)
    {
        csr_set(mstatus, 0x8);    /* Set MIE bit */
    }
    
    /* Sleep function - RISC-V busy wait */
    static inline void k_busy_wait(uint32_t usec)
    {
        volatile uint32_t count = usec * 10;  /* Approximate for typical RISC-V freq */
        while (count--) {
            __asm__ volatile ("nop");
        }
    }
#else
    #warning "No architecture specified, using generic implementations"
    
    static inline void irq_enable(unsigned int irq) { (void)irq; }
    static inline void irq_disable(unsigned int irq) { (void)irq; }
    static inline void irq_lock(void) {}
    static inline void irq_unlock(void) {}
    static inline void k_busy_wait(uint32_t usec) { (void)usec; }
#endif

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

/* Console and printk - architecture neutral */
#include <console.h>

#endif /* MINIMAL_ZEPHYR_H */
