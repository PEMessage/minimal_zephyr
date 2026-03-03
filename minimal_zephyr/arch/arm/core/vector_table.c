/*
 * Copyright (c) 2024
 * Minimal Zephyr - ARM Cortex-M3 Vector Table
 */

#include <stdint.h>

/* Stack top from linker script */
extern uint32_t __stack_top;

/* Main entry point */
extern int main(void);

/* Reset handler */
void _reset_handler(void);

/* Default handler - infinite loop */
static void __attribute__((used)) _default_handler(void)
{
    while (1) {
        __asm__ volatile ("wfi");
    }
}

/* Weak alias for all exception handlers */
#define WEAK_ALIAS __attribute__((weak, alias("_default_handler")))

/* Exception handlers */
void NMI_Handler(void) WEAK_ALIAS;
void HardFault_Handler(void) WEAK_ALIAS;
void MemManage_Handler(void) WEAK_ALIAS;
void BusFault_Handler(void) WEAK_ALIAS;
void UsageFault_Handler(void) WEAK_ALIAS;
void SVC_Handler(void) WEAK_ALIAS;
void DebugMon_Handler(void) WEAK_ALIAS;
void PendSV_Handler(void) WEAK_ALIAS;
void SysTick_Handler(void) WEAK_ALIAS;

/* IRQ handlers (weak aliases to default handler) */
void WDOG_IRQHandler(void) WEAK_ALIAS;
void RTC_IRQHandler(void) WEAK_ALIAS;
void TIM0_IRQHandler(void) WEAK_ALIAS;
void TIM1_IRQHandler(void) WEAK_ALIAS;
void TIM2_IRQHandler(void) WEAK_ALIAS;
void TIM3_IRQHandler(void) WEAK_ALIAS;
void MCIA_IRQHandler(void) WEAK_ALIAS;
void MCIB_IRQHandler(void) WEAK_ALIAS;
void UART0_IRQHandler(void) WEAK_ALIAS;
void UART1_IRQHandler(void) WEAK_ALIAS;
void UART2_IRQHandler(void) WEAK_ALIAS;
void UART3_IRQHandler(void) WEAK_ALIAS;
void UART4_IRQHandler(void) WEAK_ALIAS;
void UART5_IRQHandler(void) WEAK_ALIAS;
void SPI0_IRQHandler(void) WEAK_ALIAS;
void SPI1_IRQHandler(void) WEAK_ALIAS;
void SPI2_IRQHandler(void) WEAK_ALIAS;
void SPI3_IRQHandler(void) WEAK_ALIAS;
void SPI4_IRQHandler(void) WEAK_ALIAS;
void I2C0_IRQHandler(void) WEAK_ALIAS;
void I2C1_IRQHandler(void) WEAK_ALIAS;
void I2C2_IRQHandler(void) WEAK_ALIAS;
void I2C3_IRQHandler(void) WEAK_ALIAS;
void GPIO0_IRQHandler(void) WEAK_ALIAS;
void GPIO1_IRQHandler(void) WEAK_ALIAS;
void GPIO2_IRQHandler(void) WEAK_ALIAS;
void GPIO3_IRQHandler(void) WEAK_ALIAS;
void GPIO4_IRQHandler(void) WEAK_ALIAS;
void GPIO5_IRQHandler(void) WEAK_ALIAS;
void GPIO6_IRQHandler(void) WEAK_ALIAS;
void GPIO7_IRQHandler(void) WEAK_ALIAS;

/* Vector table */
__attribute__((section(".vectors"), used))
void (* const _vector_table[])(void) = {
    (void (*)(void))&__stack_top,   /*  0: Initial Stack Pointer */
    _reset_handler,                 /*  1: Reset Handler */
    NMI_Handler,                    /*  2: NMI Handler */
    HardFault_Handler,              /*  3: Hard Fault Handler */
    MemManage_Handler,              /*  4: MPU Fault Handler */
    BusFault_Handler,               /*  5: Bus Fault Handler */
    UsageFault_Handler,             /*  6: Usage Fault Handler */
    0,                              /*  7: Reserved */
    0,                              /*  8: Reserved */
    0,                              /*  9: Reserved */
    0,                              /* 10: Reserved */
    SVC_Handler,                    /* 11: SVCall Handler */
    DebugMon_Handler,               /* 12: Debug Monitor Handler */
    0,                              /* 13: Reserved */
    PendSV_Handler,                 /* 14: PendSV Handler */
    SysTick_Handler,                /* 15: SysTick Handler */

    /* External Interrupts (16-47) */
    WDOG_IRQHandler,                /* 16: Watchdog Timer */
    RTC_IRQHandler,                 /* 17: Real Time Clock */
    TIM0_IRQHandler,                /* 18: Timer 0 */
    TIM1_IRQHandler,                /* 19: Timer 1 */
    TIM2_IRQHandler,                /* 20: Timer 2 */
    TIM3_IRQHandler,                /* 21: Timer 3 */
    MCIA_IRQHandler,                /* 22: MCI A */
    MCIB_IRQHandler,                /* 23: MCI B */
    UART0_IRQHandler,               /* 24: UART 0 */
    UART1_IRQHandler,               /* 25: UART 1 */
    UART2_IRQHandler,               /* 26: UART 2 */
    UART3_IRQHandler,               /* 27: UART 3 */
    UART4_IRQHandler,               /* 28: UART 4 */
    UART5_IRQHandler,               /* 29: UART 5 */
    SPI0_IRQHandler,                /* 30: SPI 0 */
    SPI1_IRQHandler,                /* 31: SPI 1 */
    SPI2_IRQHandler,                /* 32: SPI 2 */
    SPI3_IRQHandler,                /* 33: SPI 3 */
    SPI4_IRQHandler,                /* 34: SPI 4 */
    I2C0_IRQHandler,                /* 35: I2C 0 */
    I2C1_IRQHandler,                /* 36: I2C 1 */
    I2C2_IRQHandler,                /* 37: I2C 2 */
    I2C3_IRQHandler,                /* 38: I2C 3 */
    GPIO0_IRQHandler,               /* 39: GPIO 0 */
    GPIO1_IRQHandler,               /* 40: GPIO 1 */
    GPIO2_IRQHandler,               /* 41: GPIO 2 */
    GPIO3_IRQHandler,               /* 42: GPIO 3 */
    GPIO4_IRQHandler,               /* 43: GPIO 4 */
    GPIO5_IRQHandler,               /* 44: GPIO 5 */
    GPIO6_IRQHandler,               /* 45: GPIO 6 */
    GPIO7_IRQHandler,               /* 46: GPIO 7 */
    0,                              /* 47: Reserved */
};
