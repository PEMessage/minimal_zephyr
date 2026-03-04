/*
 * Copyright (c) 2024
 * Minimal Zephyr - RISC-V 32-bit Startup Code
 */

#include <stdint.h>
#include "console.h"

/* Linker script symbols */
extern uint32_t _text_start;
extern uint32_t _text_end;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _data_load;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern uint32_t __stack_top;

/* Main entry point */
extern int main(void);

/* RISC-V CSR read/write macros */
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

/* Hart ID (CPU core number) */
static inline uint32_t mhartid(void)
{
    return csr_read(mhartid);
}

/* System reset using SiFive test finisher (QEMU virt machine) */
#define SIFIVE_TEST_FINISHER_BASE   0x00100000
#define SIFIVE_TEST_FINISHER_EXIT   0x5555
#define SIFIVE_TEST_FINISHER_FAIL   0x3333
#define SIFIVE_TEST_FINISHER_REBOOT 0x7777

static inline void system_reboot(int code)
{
    volatile uint32_t *test_finisher = (volatile uint32_t *)SIFIVE_TEST_FINISHER_BASE;
    *test_finisher = (code << 16) | SIFIVE_TEST_FINISHER_EXIT;
    __asm__ volatile ("fence" ::: "memory");
    while (1) {
        __asm__ volatile ("wfi");
    }
}

/* Simple memset implementation for BSS clearing */
static void *minimal_memset(void *s, int c, uint32_t n)
{
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

/* Copy data section from FLASH to RAM */
static inline void copy_data(void)
{
    uint32_t *src = &_data_load;
    uint32_t *dst = &_data_start;
    uint32_t *end = &_data_end;
    
    while (dst < end) {
        *dst++ = *src++;
    }
}

/* Zero-initialize BSS section */
static inline void zero_bss(void)
{
    minimal_memset(&_bss_start, 0, (uint32_t)&_bss_end - (uint32_t)&_bss_start);
}

/* Early console output for debugging */
static void early_putc(char c)
{
    /* NS16550 UART at 0x10000000 - early output without initialization */
    volatile uint8_t *uart_thr = (volatile uint8_t *)0x10000000;
    volatile uint8_t *uart_lsr = (volatile uint8_t *)0x10000005;
    
    /* Wait for transmitter ready */
    while ((*uart_lsr & 0x20) == 0) {
        __asm__ volatile ("nop");
    }
    *uart_thr = c;
}

static void early_puts(const char *str)
{
    while (*str) {
        if (*str == '\n') {
            early_putc('\r');
        }
        early_putc(*str++);
    }
}

/* Reset handler - called on reset */
__attribute__((naked, noreturn)) void _reset_handler(void)
{
    __asm__ volatile (
        /* Initialize stack pointer - load __stack_top address */
        "lui sp, %hi(__stack_top)\n"
        "addi sp, sp, %lo(__stack_top)\n"
        
        /* Jump to C reset handler */
        "j _c_reset_handler\n"
    );
}

/* C reset handler */
__attribute__((used, noreturn)) void _c_reset_handler(void)
{
    uint32_t hart_id = mhartid();
    
    /* Output something immediately to verify we're running */
    early_puts("RISC-V boot\n");
    
    /* Only boot on hart 0, others go to sleep */
    if (hart_id != 0) {
        while (1) {
            __asm__ volatile ("wfi");
        }
    }
    
    early_puts("Hart 0 running\n");
    
    /* Copy initialized data from FLASH to RAM */
    copy_data();
    
    /* Zero BSS section */
    zero_bss();
    
    /* Initialize console for UART output */
    console_init();
    
    /* Call main */
    main();
    
    /* If main returns, halt */
    system_reboot(0);
}

/* Default trap handler (weak, can be overridden) */
__attribute__((weak)) void _trap_handler(void)
{
    /* Default: just return */
    __asm__ volatile ("mret");
}

/* Machine mode trap entry */
__attribute__((naked)) void _trap_entry(void)
{
    __asm__ volatile (
        /* Save registers */
        "addi sp, sp, -128\n"
        "sw ra, 0(sp)\n"
        "sw t0, 4(sp)\n"
        "sw t1, 8(sp)\n"
        "sw t2, 12(sp)\n"
        "sw a0, 16(sp)\n"
        "sw a1, 20(sp)\n"
        "sw a2, 24(sp)\n"
        "sw a3, 28(sp)\n"
        "sw a4, 32(sp)\n"
        "sw a5, 36(sp)\n"
        "sw a6, 40(sp)\n"
        "sw a7, 44(sp)\n"
        
        /* Call trap handler */
        "call _trap_handler\n"
        
        /* Restore registers */
        "lw ra, 0(sp)\n"
        "lw t0, 4(sp)\n"
        "lw t1, 8(sp)\n"
        "lw t2, 12(sp)\n"
        "lw a0, 16(sp)\n"
        "lw a1, 20(sp)\n"
        "lw a2, 24(sp)\n"
        "lw a3, 28(sp)\n"
        "lw a4, 32(sp)\n"
        "lw a5, 36(sp)\n"
        "lw a6, 40(sp)\n"
        "lw a7, 44(sp)\n"
        "addi sp, sp, 128\n"
        
        /* Return from trap */
        "mret\n"
    );
}

/* Interrupt lock/unlock for RISC-V */
static inline void irq_lock(void)
{
    csr_clear(mstatus, 0x8);  /* Clear MIE bit */
}

static inline void irq_unlock(void)
{
    csr_set(mstatus, 0x8);    /* Set MIE bit */
}

/* Simple delay loop */
static inline void k_busy_wait(uint32_t usec)
{
    volatile uint32_t count = usec * 10;  /* Approximate */
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
