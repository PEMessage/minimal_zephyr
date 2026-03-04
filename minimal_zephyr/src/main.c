/*
 * Copyright (c) 2024
 * Minimal Zephyr - Default main.c
 */

#include "minimal_zephyr.h"

/* Main entry point */
int main(void)
{
    /* Print welcome message using printk (outputs to UART) */
    printk("\n");
    printk("========================================\n");
    printk("  Minimal Zephyr Framework Started\n");
    printk("  Board: MPS2 AN385 (Cortex-M3)\n");
    printk("========================================\n");
    printk("\n");
    
    int counter = 0;
    
    while (1) {
        printk("Hello World! Count: %d (0x%x)\n", counter, counter);
        
        counter++;
        
        /* Sleep for 1 second */
        k_msleep(1000);
    }
    
    return 0;
}
