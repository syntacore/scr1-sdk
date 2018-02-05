// Replacement for the newlib's stdio.h.
//

#ifndef SCR_STDIO_H
#define SCR_STDIO_H

#include <stddef.h>

int snprintf(char *buf, size_t size, const char *fmt, ...) __attribute__((format (printf, 3, 4)));
int printf(const char *fmt, ...) __attribute__((format (printf, 1, 2)));
int putchar(int c);
int puts(const char *str);

#endif // SCR_STDIO_H
