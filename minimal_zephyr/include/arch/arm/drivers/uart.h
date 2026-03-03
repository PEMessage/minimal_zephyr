/*
 * Copyright (c) 2024
 * Minimal Zephyr - UART Driver for MPS2 AN385
 */

#ifndef MINIMAL_UART_H
#define MINIMAL_UART_H

#include <stdint.h>

/* MPS2 AN385 UART0 base address */
#define UART0_BASE      0x40004000
#define UART1_BASE      0x40005000

/* UART Register offsets */
#define UART_DR         0x00  /* Data Register */
#define UART_RSR        0x04  /* Receive Status Register */
#define UART_ECR        0x04  /* Error Clear Register */
#define UART_FR         0x18  /* Flag Register */
#define UART_ILPR       0x20  /* IrDA Low-Power Counter Register */
#define UART_IBRD       0x24  /* Integer Baud Rate Register */
#define UART_FBRD       0x28  /* Fractional Baud Rate Register */
#define UART_LCRH       0x2C  /* Line Control Register */
#define UART_CR         0x30  /* Control Register */
#define UART_IFLS       0x34  /* Interrupt FIFO Level Select Register */
#define UART_IMSC       0x38  /* Interrupt Mask Set/Clear Register */
#define UART_RIS        0x3C  /* Raw Interrupt Status Register */
#define UART_MIS        0x40  /* Masked Interrupt Status Register */
#define UART_ICR        0x44  /* Interrupt Clear Register */
#define UART_DMACR      0x48  /* DMA Control Register */
#define UART_PeriphID0  0xFE0  /* Peripheral ID Register 0 */
#define UART_PeriphID1  0xFE4  /* Peripheral ID Register 1 */
#define UART_PeriphID2  0xFE8  /* Peripheral ID Register 2 */
#define UART_PeriphID3  0xFEC  /* Peripheral ID Register 3 */
#define UART_PCellID0   0xFF0  /* PrimeCell ID Register 0 */
#define UART_PCellID1   0xFF4  /* PrimeCell ID Register 1 */
#define UART_PCellID2   0xFF8  /* PrimeCell ID Register 2 */
#define UART_PCellID3   0xFFC  /* PrimeCell ID Register 3 */

/* Flag Register bits */
#define UART_FR_TXFF    (1 << 5)  /* Transmit FIFO full */
#define UART_FR_RXFE    (1 << 4)  /* Receive FIFO empty */
#define UART_FR_TXFE    (1 << 7)  /* Transmit FIFO empty */
#define UART_FR_RXFF    (1 << 6)  /* Receive FIFO full */
#define UART_FR_BUSY    (1 << 3)  /* UART busy */

/* Line Control Register bits */
#define UART_LCRH_FEN   (1 << 4)  /* Enable FIFOs */
#define UART_LCRH_WLEN8 (3 << 5)  /* 8 bit word length */
#define UART_LCRH_STP2  (1 << 3)  /* Two stop bits */
#define UART_LCRH_PEN   (1 << 1)  /* Parity enable */

/* Control Register bits */
#define UART_CR_UARTEN  (1 << 0)  /* UART Enable */
#define UART_CR_TXE     (1 << 8)  /* Transmit Enable */
#define UART_CR_RXE     (1 << 9)  /* Receive Enable */

/* UART base addresses */
static inline volatile uint32_t* uart_base(int uart)
{
    switch (uart) {
        case 0: return (volatile uint32_t *)UART0_BASE;
        case 1: return (volatile uint32_t *)UART1_BASE;
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

/* Initialize UART */
static inline void uart_init(int uart, uint32_t baudrate)
{
    volatile uint32_t *base = uart_base(uart);
    
    /* Disable UART during configuration */
    uart_write(uart, UART_CR, 0);
    
    /* Calculate baud rate divisor for 25MHz system clock */
    /* Baud rate divisor = System Clock / (16 * Baud Rate) */
    /* For 115200: 25000000 / (16 * 115200) = 13.55 */
    /* Integer part: 13, Fractional part: 0.55 * 64 = 35 */
    uint32_t ibrd = 25000000 / (16 * baudrate);
    uint32_t fbrd = ((25000000 * 64) / (16 * baudrate)) & 0x3F;
    
    uart_write(uart, UART_IBRD, ibrd);
    uart_write(uart, UART_FBRD, fbrd);
    
    /* Configure line: 8 data bits, 1 stop bit, no parity, enable FIFO */
    uart_write(uart, UART_LCRH, UART_LCRH_WLEN8 | UART_LCRH_FEN);
    
    /* Enable UART, TX, and RX */
    uart_write(uart, UART_CR, UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE);
}

/* Check if transmit FIFO is full */
static inline int uart_tx_full(int uart)
{
    return (uart_read(uart, UART_FR) & UART_FR_TXFF) != 0;
}

/* Check if transmit FIFO is empty */
static inline int uart_tx_empty(int uart)
{
    return (uart_read(uart, UART_FR) & UART_FR_TXFE) != 0;
}

/* Check if receive FIFO is empty */
static inline int uart_rx_empty(int uart)
{
    return (uart_read(uart, UART_FR) & UART_FR_RXFE) != 0;
}

/* Write a character to UART */
static inline void uart_putc(int uart, char c)
{
    /* Wait for transmit FIFO to have space */
    while (uart_tx_full(uart)) {
        __asm__ volatile ("nop");
    }
    
    uart_write(uart, UART_DR, c);
}

/* Read a character from UART */
static inline int uart_getc(int uart)
{
    /* Wait for receive data */
    while (uart_rx_empty(uart)) {
        __asm__ volatile ("nop");
    }
    
    return uart_read(uart, UART_DR) & 0xFF;
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
