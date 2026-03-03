/*
 * Copyright (c) 2024
 * Minimal Application - Hello World
 */

#include "minimal_zephyr.h"
#include "arch/arm/semihosting.h"

int main(void)
{
    /* Use semihosting for output (works with QEMU -semihosting) */
    semihosting_puts("\n");
    semihosting_puts("========================================\n");
    semihosting_puts("  Minimal Zephyr App Started!\n");
    semihosting_puts("  This is a custom application.\n");
    semihosting_puts("========================================\n");
    semihosting_puts("\n");
    
    int counter = 0;
    
    while (1) {
        semihosting_puts("Custom app running... Count: ");
        
        /* Simple counter output */
        char buf[2] = {'0' + (counter % 10), '\n'};
        semihosting_puts(buf);
        
        counter++;
        
        /* Sleep for 1 second */
        k_msleep(1000);
    }
    
    return 0;
}
