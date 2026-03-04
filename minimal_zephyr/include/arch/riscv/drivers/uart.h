/*
 * Copyright (c) 2024
 * Minimal Zephyr - NS16550 UART Driver for RISC-V (QEMU virt machine)
 * 
 * NS16550 UART at 0x10000000 (QEMU virt machine default)
 */

#ifndef MINIMAL_NS16550_UART_H
#define MINIMAL_NS16550_UART_H

#include <stdint.h>

/* UART base address for QEMU virt machine */
#define UART0_BASE      0x10000000

/* NS16550 Register offsets (direct addressing) */
#define UART_RBR        0x00  /* Receiver Buffer Register (R) */
#define UART_THR        0x00  /* Transmitter Holding Register (W) */
#define UART_DLL        0x00  /* Divisor Latch Low (when DLAB=1) */
#define UART_IER        0x01  /* Interrupt Enable Register */
#define UART_DLM        0x01  /* Divisor Latch High (when DLAB=1) */
#define UART_IIR        0x02  /* Interrupt Identity Register (R) */
#define UART_FCR        0x02  /* FIFO Control Register (W) */
#define UART_LCR        0x03  /* Line Control Register */
#define UART_MCR        0x04  /* Modem Control Register */
#define UART_LSR        0x05  /* Line Status Register */
#define UART_MSR        0x06  /* Modem Status Register */
#define UART_SCR        0x07  /* Scratch Register */

/* Line Status Register bits */
#define UART_LSR_DR     0x01  /* Data Ready */
#define UART_LSR_OE     0x02  /* Overrun Error */
#define UART_LSR_PE     0x04  /* Parity Error */
#define UART_LSR_FE     0x08  /* Framing Error */
#define UART_LSR_BI     0x10  /* Break Interrupt */
#define UART_LSR_THRE   0x20  /* Transmitter Holding Register Empty */
#define UART_LSR_TEMT   0x40  /* Transmitter Empty */
#define UART_LSR_RFE    0x80  /* Receiver FIFO Error */

/* Line Control Register bits */
#define UART_LCR_WLS_5  0x00  /* Word Length 5 bits */
#define UART_LCR_WLS_6  0x01  /* Word Length 6 bits */
#define UART_LCR_WLS_7  0x02  /* Word Length 7 bits */
#define UART_LCR_WLS_8  0x03  /* Word Length 8 bits */
#define UART_LCR_STB    0x04  /* Stop Bits (0=1, 1=2) */
#define UART_LCR_PEN    0x08  /* Parity Enable */
#define UART_LCR_EPS    0x10  /* Even Parity Select */
#define UART_LCR_STICK  0x20  /* Stick Parity */
#define UART_LCR_BREAK  0x40  /* Set Break */
#define UART_LCR_DLAB   0x80  /* Divisor Latch Access Bit */

/* FIFO Control Register bits */
#define UART_FCR_FIFOEN 0x01  /* FIFO Enable */
#define UART_FCR_RFIFOR 0x02  /* Receiver FIFO Reset */
#define UART_FCR_XFIFOR 0x04  /* Transmitter FIFO Reset */
#define UART_FCR_DMAMOD 0x08  /* DMA Mode */
#define UART_FCR_RTRIG  0xC0  /* Receiver Trigger (14 chars) */

/* Interrupt Enable Register bits */
#define UART_IER_ERBI   0x01  /* Enable Received Data Available Interrupt */
#define UART_IER_ETBEI  0x02  /* Enable Transmitter Holding Register Empty Interrupt */
#define UART_IER_ELSI   0x04  /* Enable Receiver Line Status Interrupt */
#define UART_IER_EDSSI  0x08  /* Enable Modem Status Interrupt */

/* Modem Control Register bits */
#define UART_MCR_DTR    0x01  /* Data Terminal Ready */
#define UART_MCR_RTS    0x02  /* Request To Send */
#define UART_MCR_OUT1   0x04  /* Output 1 */
#define UART_MCR_OUT2   0x08  /* Output 2 */
#define UART_MCR_LOOP   0x10  /* Loopback Mode */
#define UART_MCR_AFCE   0x20  /* Auto Flow Control Enable */

/* UART register access */
static inline uint8_t uart_read_reg(uint32_t offset)
{
    return *((volatile uint8_t *)(UART0_BASE + offset));
}

static inline void uart_write_reg(uint32_t offset, uint8_t value)
{
    *((volatile uint8_t *)(UART0_BASE + offset)) = value;
}

/* Check if transmit FIFO is full */
static inline int uart_tx_full(void)
{
    return (uart_read_reg(UART_LSR) & UART_LSR_THRE) == 0;
}

/* Check if transmit FIFO is empty (ready for data) */
static inline int uart_tx_ready(void)
{
    return (uart_read_reg(UART_LSR) & UART_LSR_THRE) != 0;
}

/* Check if receive buffer has data */
static inline int uart_rx_ready(void)
{
    return (uart_read_reg(UART_LSR) & UART_LSR_DR) != 0;
}

/* Check if receive buffer is empty */
static inline int uart_rx_empty(void)
{
    return (uart_read_reg(UART_LSR) & UART_LSR_DR) == 0;
}

/* Initialize UART
 * 
 * For QEMU virt machine, the UART is already initialized by QEMU
 * at 115200 baud, 8N1. We just enable FIFOs.
 */
static inline void uart_init(int uart __attribute__((unused)), uint32_t baudrate __attribute__((unused)))
{
    /* Disable interrupts */
    uart_write_reg(UART_IER, 0x00);
    
    /* Enable FIFOs, reset them */
    uart_write_reg(UART_FCR, UART_FCR_FIFOEN | UART_FCR_RFIFOR | UART_FCR_XFIFOR);
    
    /* Set line control: 8 data bits, 1 stop bit, no parity */
    uart_write_reg(UART_LCR, UART_LCR_WLS_8);
    
    /* Set modem control: DTR and RTS enabled */
    uart_write_reg(UART_MCR, UART_MCR_DTR | UART_MCR_RTS);
}

/* Write a character to UART */
static inline void uart_putc(int uart __attribute__((unused)), char c)
{
    /* Wait for transmitter holding register to be empty */
    while (!uart_tx_ready()) {
        __asm__ volatile ("nop");
    }
    
    uart_write_reg(UART_THR, c);
}

/* Read a character from UART (non-blocking) */
static inline int uart_getc_nonblock(int uart __attribute__((unused)))
{
    if (uart_rx_empty()) {
        return -1;
    }
    return uart_read_reg(UART_RBR);
}

/* Read a character from UART (blocking) */
static inline int uart_getc(int uart __attribute__((unused)))
{
    /* Wait for receive data */
    while (uart_rx_empty()) {
        __asm__ volatile ("nop");
    }
    
    return uart_read_reg(UART_RBR);
}

/* Write a string to UART */
static inline void uart_puts(int uart __attribute__((unused)), const char *str)
{
    while (*str) {
        if (*str == '\n') {
            uart_putc(0, '\r');
        }
        uart_putc(0, *str++);
    }
}

#endif /* MINIMAL_NS16550_UART_H */
