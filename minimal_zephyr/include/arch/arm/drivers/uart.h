/*
 * Copyright (c) 2024
 * Minimal Zephyr - UART Driver for MPS2 AN385 (CMSDK APB UART)
 * 
 * Based on ARM CMSDK APB UART specification
 * Compatible with "arm,cmsdk-uart" as used in real Zephyr
 */

#ifndef MINIMAL_UART_H
#define MINIMAL_UART_H

#include <stdint.h>

/* MPS2 AN385 UART base addresses (CMSDK APB UART) */
#define UART0_BASE      0x40004000
#define UART1_BASE      0x40005000
#define UART2_BASE      0x40006000

/* CMSDK APB UART Register offsets */
#define UART_DATA       0x00  /* Data Register (R/W) */
#define UART_STATE      0x04  /* Status Register (R/W) */
#define UART_CTRL       0x08  /* Control Register (R/W) */
#define UART_INTSTATUS  0x0C  /* Interrupt Status Register (R/)/Clear (W) */
#define UART_BAUDDIV    0x10  /* Baudrate Divider Register (R/W) */

/* STATE Register bits */
#define UART_STATE_TX_BF    (1 << 0)  /* Transmit buffer full */
#define UART_STATE_RX_BF    (1 << 1)  /* Receive buffer full */
#define UART_STATE_TX_B_OV  (1 << 2)  /* Transmit buffer overrun */
#define UART_STATE_RX_B_OV  (1 << 3)  /* Receive buffer overrun */

/* CTRL Register bits */
#define UART_CTRL_TX_EN     (1 << 0)  /* Transmit enable */
#define UART_CTRL_RX_EN     (1 << 1)  /* Receive enable */
#define UART_CTRL_TX_IN_EN  (1 << 2)  /* Transmit interrupt enable */
#define UART_CTRL_RX_IN_EN  (1 << 3)  /* Receive interrupt enable */
#define UART_CTRL_TX_OV_EN  (1 << 4)  /* Transmit overrun interrupt enable */
#define UART_CTRL_RX_OV_EN  (1 << 5)  /* Receive overrun interrupt enable */

/* UART base addresses */
static inline volatile uint32_t* uart_base(int uart)
{
    switch (uart) {
        case 0: return (volatile uint32_t *)UART0_BASE;
        case 1: return (volatile uint32_t *)UART1_BASE;
        case 2: return (volatile uint32_t *)UART2_BASE;
        default: return (volatile uint32_t *)UART0_BASE;
    }
}

/* UART register access */
static inline uint32_t uart_read(int uart, uint32_t offset)
{
    return uart_base(uart)[offset / 4];
}

static inline void uart_write(int uart, uint32_t offset, uint32_t value)
{
    uart_base(uart)[offset / 4] = value;
}

/* Check if transmit buffer is full */
static inline int uart_tx_full(int uart)
{
    return (uart_read(uart, UART_STATE) & UART_STATE_TX_BF) != 0;
}

/* Check if transmit buffer is empty (ready for data) */
static inline int uart_tx_ready(int uart)
{
    return (uart_read(uart, UART_STATE) & UART_STATE_TX_BF) == 0;
}

/* Check if receive buffer has data */
static inline int uart_rx_ready(int uart)
{
    return (uart_read(uart, UART_STATE) & UART_STATE_RX_BF) != 0;
}

/* Check if receive buffer is empty */
static inline int uart_rx_empty(int uart)
{
    return (uart_read(uart, UART_STATE) & UART_STATE_RX_BF) == 0;
}

/* Initialize UART */
static inline void uart_init(int uart, uint32_t baudrate)
{
    /* Disable UART during configuration */
    uart_write(uart, UART_CTRL, 0);
    
    /* Calculate baud rate divisor for 25MHz system clock */
    /* Baud rate divisor = System Clock / Baud Rate */
    /* For 115200: 25000000 / 115200 = 217 */
    uint32_t bauddiv = 25000000 / baudrate;
    if (bauddiv == 0) bauddiv = 1;
    
    uart_write(uart, UART_BAUDDIV, bauddiv);
    
    /* Enable transmitter and receiver */
    uart_write(uart, UART_CTRL, UART_CTRL_TX_EN | UART_CTRL_RX_EN);
}

/* Write a character to UART */
static inline void uart_putc(int uart, char c)
{
    /* Wait for transmit buffer to have space */
    while (uart_tx_full(uart)) {
        __asm__ volatile ("nop");
    }
    
    uart_write(uart, UART_DATA, c);
}

/* Read a character from UART (non-blocking) */
static inline int uart_getc_nonblock(int uart)
{
    if (uart_rx_empty(uart)) {
        return -1;
    }
    return uart_read(uart, UART_DATA) & 0xFF;
}

/* Read a character from UART (blocking) */
static inline int uart_getc(int uart)
{
    /* Wait for receive data */
    while (uart_rx_empty(uart)) {
        __asm__ volatile ("nop");
    }
    
    return uart_read(uart, UART_DATA) & 0xFF;
}

/* Write a string to UART */
static inline void uart_puts(int uart, const char *str)
{
    while (*str) {
        if (*str == '\n') {
            uart_putc(uart, '\r');
        }
        uart_putc(uart, *str++);
    }
}

#endif /* MINIMAL_UART_H */
