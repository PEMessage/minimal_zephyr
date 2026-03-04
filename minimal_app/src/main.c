/*
 * Copyright (c) 2024
 * Minimal Application - Hello World with UART
 */

#include "minimal_zephyr.h"

int main(void)
{
    /* Use printk for output (outputs to UART0) */
    printk("\n");
    printk("========================================\n");
    printk("  Minimal Zephyr App Started!\n");
    printk("  This is a custom application.\n");
    printk("  UART output is now working!\n");
    printk("========================================\n");
    printk("\n");
    
    int counter = 0;
    
    while (1) {
        printk("Custom app running... Count: %d\n", counter);
        
        counter++;
        
        /* Sleep for 1 second */
        k_msleep(1000);
    }
    
    return 0;
}
