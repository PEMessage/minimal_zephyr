/*
 * Copyright (c) 2024
 * Minimal Zephyr - ARM Cortex-M3 Startup Code
 */

#include <stdint.h>
#include <string.h>
#include "console.h"

/* Linker script symbols */
extern uint32_t _text_start;
extern uint32_t _text_end;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _data_load;
extern uint32_t _bss_start;
extern uint32_t _bss_end;

/* Vector table - defined in vector_table.c */
extern uint32_t _vector_table[];

/* Dummy reference to prevent vector table from being garbage collected */
static volatile uint32_t* __attribute__((used)) _vector_ref = _vector_table;

/* Main entry point */
extern int main(void);

/* System initialization (weak, can be overridden) */
__attribute__((weak)) void SystemInit(void)
{
    /* Disable interrupts during initialization */
    __asm__ volatile ("cpsid i");
    
    /* Ensure 4-byte alignment for stack */
    __asm__ volatile ("msr control, %0" :: "r"(0x0));
    
    /* Return */
}

/* Copy data section from FLASH to SRAM */
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
    uint32_t *dst = &_bss_start;
    uint32_t *end = &_bss_end;
    
    while (dst < end) {
        *dst++ = 0;
    }
}

/* Reset handler - called on reset */
__attribute__((naked, noreturn)) void _reset_handler(void)
{
    __asm__ volatile (
        /* Initialize stack pointer */
        "ldr r0, =__stack_top\n"
        "mov sp, r0\n"
        
        /* Call SystemInit */
        "bl SystemInit\n"
        
        /* Jump to C reset handler */
        "b _c_reset_handler\n"
    );
}

/* C reset handler */
__attribute__((used, noreturn)) void _c_reset_handler(void)
{
    /* Copy initialized data from FLASH to SRAM */
    copy_data();
    
    /* Zero BSS section */
    zero_bss();
    
    /* Initialize console for UART output */
    console_init();
    
    /* Call main */
    main();
    
    /* If main returns, halt */
    while (1) {
        __asm__ volatile ("wfi");
    }
}

/* System tick handler (weak, can be overridden) */
__attribute__((weak)) void SysTick_Handler(void)
{
    /* Default: do nothing */
}

/* SVC handler (weak, can be overridden) */
__attribute__((weak)) void SVC_Handler(void)
{
    /* Default: do nothing */
}

/* PendSV handler (weak, can be overridden) */
__attribute__((weak)) void PendSV_Handler(void)
{
    /* Default: do nothing */
}
