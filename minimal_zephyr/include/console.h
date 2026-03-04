/*
 * Copyright (c) 2024
 * Minimal Zephyr - Console Support
 */

#ifndef MINIMAL_CONSOLE_H
#define MINIMAL_CONSOLE_H

#include <stddef.h>
#include <stdarg.h>

/* Console initialization - called automatically at startup */
int console_init(void);

/* Character output function type */
typedef int (*console_char_out_fn_t)(char c);

/* Install a character output function */
void console_set_char_out(console_char_out_fn_t fn);

/* Get the current character output function */
console_char_out_fn_t console_get_char_out(void);

/* Write a single character to console */
int console_putchar(char c);

/* Write a string to console */
int console_puts(const char *str);

/* Write buffer to console */
int console_write(const char *buf, size_t len);

/* Minimal printk with format support */
int printk(const char *fmt, ...);
int vprintk(const char *fmt, va_list ap);

/* Printf to buffer */
int snprintk(char *str, size_t size, const char *fmt, ...);
int vsnprintk(char *str, size_t size, const char *fmt, va_list ap);

/* Early console (for use before main) - uses semihosting */
int early_console_init(void);
void early_printk(const char *fmt, ...);

#endif /* MINIMAL_CONSOLE_H */
