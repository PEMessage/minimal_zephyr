/*
 * Copyright (c) 2024
 * Minimal Zephyr - Default main.c
 */

#include "minimal_zephyr.h"
#include "arch/arm/semihosting.h"

/* Main entry point */
int main(void)
{
    /* Print welcome message using semihosting */
    semihosting_puts("\n");
    semihosting_puts("========================================\n");
    semihosting_puts("  Minimal Zephyr Framework Started\n");
    semihosting_puts("  Board: MPS2 AN385 (Cortex-M3)\n");
    semihosting_puts("========================================\n");
    semihosting_puts("\n");
    
    int counter = 0;
    
    while (1) {
        semihosting_puts("Hello World! Count: ");
        
        /* Print counter as decimal */
        char buf[12];
        int i = 0;
        int n = counter;
        
        if (n == 0) {
            buf[i++] = '0';
        } else {
            while (n > 0) {
                buf[i++] = '0' + (n % 10);
                n /= 10;
            }
        }
        buf[i] = '\0';
        
        /* Reverse the string */
        for (int j = 0; j < i / 2; j++) {
            char tmp = buf[j];
            buf[j] = buf[i - 1 - j];
            buf[i - 1 - j] = tmp;
        }
        
        semihosting_puts(buf);
        semihosting_puts("\n");
        
        counter++;
        
        /* Sleep for 1 second */
        k_msleep(1000);
    }
    
    return 0;
}
