/*
 * Copyright (c) 2024
 * Minimal Zephyr - Console Implementation
 */

#include "../include/console.h"

/* Architecture-specific includes */
#if defined(CONFIG_ARCH_ARM) || defined(__ARM_ARCH)
    #include "../include/arch/arm/drivers/uart.h"
    #include "../include/arch/arm/semihosting.h"
    #define ARCH_ARM 1
#elif defined(CONFIG_ARCH_RISCV) || defined(__riscv)
    #include "../include/arch/riscv/drivers/uart.h"
    #define ARCH_RISCV 1
#else
    /* Default to ARM if not specified */
    #include "../include/arch/arm/drivers/uart.h"
    #include "../include/arch/arm/semihosting.h"
    #define ARCH_ARM 1
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

/* Default character output function - starts NULL until init */
static console_char_out_fn_t _char_out = NULL;

/* Forward declarations for output functions */
int uart_putc_out(char c);

/* Console initialization - sets up UART as output */
int console_init(void)
{
    /* Initialize UART0 at 115200 baud */
    uart_init(0, 115200);
    
    /* Install UART output function */
    _char_out = uart_putc_out;
    
    return 0;
}

/* Install a character output function */
void console_set_char_out(console_char_out_fn_t fn)
{
    _char_out = fn;
}

/* Get the current character output function */
console_char_out_fn_t console_get_char_out(void)
{
    return _char_out;
}

/* Wrapper for uart_putc to match signature */
int uart_putc_out(char c)
{
    uart_putc(0, c);
    return 0;
}

/* Write a single character to console */
int console_putchar(char c)
{
    if (_char_out) {
        return _char_out(c);
    }
    return -1;
}

/* Write a string to console */
int console_puts(const char *str)
{
    int count = 0;
    while (*str) {
        if (*str == '\n') {
            console_putchar('\r');
        }
        console_putchar(*str++);
        count++;
    }
    return count;
}

/* Write buffer to console */
int console_write(const char *buf, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            console_putchar('\r');
        }
        console_putchar(buf[i]);
    }
    return len;
}

/* Minimal vsnprintf implementation */
int vsnprintk(char *str, size_t size, const char *fmt, va_list ap)
{
    char *out = str;
    size_t remaining = size;
    
    while (*fmt && remaining > 1) {
        if (*fmt != '%') {
            *out++ = *fmt++;
            remaining--;
            continue;
        }
        
        fmt++; /* Skip '%' */
        
        /* Handle format specifier */
        int width = 0;
        int zero_pad = 0;
        int base = 10;
        int is_signed = 0;
        int long_count = 0;
        
        /* Parse flags */
        if (*fmt == '0') {
            zero_pad = 1;
            fmt++;
        }
        
        /* Parse width */
        while (*fmt >= '0' && *fmt <= '9') {
            width = width * 10 + (*fmt - '0');
            fmt++;
        }
        
        /* Parse length modifiers */
        while (*fmt == 'l') {
            long_count++;
            fmt++;
        }
        
        /* Handle conversion specifier */
        switch (*fmt) {
            case 'c': {
                char c = (char)va_arg(ap, int);
                if (remaining > 1) {
                    *out++ = c;
                    remaining--;
                }
                break;
            }
            
            case 's': {
                const char *s = va_arg(ap, const char *);
                if (!s) s = "(null)";
                while (*s && remaining > 1) {
                    *out++ = *s++;
                    remaining--;
                }
                break;
            }
            
            case 'd':
            case 'i':
                is_signed = 1;
                base = 10;
                goto handle_int;
                
            case 'u':
                is_signed = 0;
                base = 10;
                goto handle_int;
                
            case 'x':
            case 'X':
                is_signed = 0;
                base = 16;
                goto handle_int;
                
            case 'p': {
                /* Pointer - print as hex */
                if (remaining > 3) {
                    *out++ = '0';
                    *out++ = 'x';
                    remaining -= 2;
                }
                uintptr_t p = (uintptr_t)va_arg(ap, void *);
                char buf[17];
                int i = 0;
                do {
                    int digit = p & 0xF;
                    buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
                    p >>= 4;
                } while (p);
                /* Pad to 8 hex digits */
                while (i < 8 && remaining > 1) {
                    *out++ = '0';
                    remaining--;
                }
                while (i > 0 && remaining > 1) {
                    *out++ = buf[--i];
                    remaining--;
                }
                break;
            }
            
            case '%':
                if (remaining > 1) {
                    *out++ = '%';
                    remaining--;
                }
                break;
                
            handle_int: {
                /* Handle integer conversion */
                uint32_t uval;
                int32_t ival = 0;
                
                if (long_count >= 1) {
                    if (is_signed) {
                        int64_t lval = va_arg(ap, long);
                        if (lval < 0) {
                            if (remaining > 1) {
                                *out++ = '-';
                                remaining--;
                            }
                            lval = -lval;
                        }
                        uval = (uint32_t)lval;
                    } else {
                        uval = va_arg(ap, unsigned long);
                    }
                } else {
                    if (is_signed) {
                        ival = va_arg(ap, int);
                        if (ival < 0) {
                            if (remaining > 1) {
                                *out++ = '-';
                                remaining--;
                            }
                            uval = (uint32_t)(-ival);
                        } else {
                            uval = (uint32_t)ival;
                        }
                    } else {
                        uval = va_arg(ap, unsigned int);
                    }
                }
                
                char buf[32];
                int i = 0;
                
                if (uval == 0) {
                    buf[i++] = '0';
                } else {
                    while (uval > 0) {
                        int digit = uval % base;
                        buf[i++] = (digit < 10) ? ('0' + digit) : 
                                   ((*fmt == 'X') ? ('A' + digit - 10) : ('a' + digit - 10));
                        uval /= base;
                    }
                }
                
                /* Pad with zeros if needed */
                while (i < width && remaining > 1) {
                    *out++ = zero_pad ? '0' : ' ';
                    remaining--;
                    width--;
                }
                
                /* Reverse the number into output */
                while (i > 0 && remaining > 1) {
                    *out++ = buf[--i];
                    remaining--;
                }
                break;
            }
            
            default:
                /* Unknown format - just output the character */
                if (remaining > 1) {
                    *out++ = *fmt;
                    remaining--;
                }
                break;
        }
        
        if (*fmt) {
            fmt++;
        }
    }
    
    *out = '\0';
    return size - remaining;
}

/* snprintf implementation */
int snprintk(char *str, size_t size, const char *fmt, ...)
{
    va_list ap;
    int ret;
    
    va_start(ap, fmt);
    ret = vsnprintk(str, size, fmt, ap);
    va_end(ap);
    
    return ret;
}

/* vprintk implementation - outputs directly to console */
int vprintk(const char *fmt, va_list ap)
{
    char buf[256];
    int len;
    
    len = vsnprintk(buf, sizeof(buf), fmt, ap);
    console_write(buf, len);
    
    return len;
}

/* printk implementation */
int printk(const char *fmt, ...)
{
    va_list ap;
    int ret;
    
    va_start(ap, fmt);
    ret = vprintk(fmt, ap);
    va_end(ap);
    
    return ret;
}
