#include <stdio.h>
#include <stdint.h>

#include "nlib.h"
#include "rtc.h"

#ifdef putchar
#undef putchar
#endif

// A simplified implementation of putchar
int putchar(int ch)
{
    if (ch == '\n') {
        console_putc('\r');
    }

    return console_putc(ch);
}

void console_puthex32(unsigned long val)
{
    console_puthex16(val >> 16);
    console_puthex16(val);
}

void console_puthex16(unsigned long val)
{
    console_puthex8(val >> 8);
    console_puthex8(val);
}

void console_puthex8(unsigned long val)
{
    console_puthex4(val >> 4);
    console_puthex4(val);
}

void console_puthex4(unsigned long val)
{
    int c = val & 0xf;
    putchar(c + (c > 9 ? ('A' - 10) : '0'));
}

void console_putstr(const char *str)
{
    while (*str)
        putchar(*str++);
}

static inline void __attribute__((noreturn)) shutdown(void)
{
    while (1);
}


void __attribute__((noreturn)) exit(int status)
{
    console_putstr("\nExit ");
    console_puthex(status);
    putchar('\n');
    shutdown();
}

void __attribute__((noreturn)) abort(void)
{
    console_putstr("\nAbort.");
    shutdown();
}

int main(int argc, char **argv);
void scr_uart_init(void);

void c_start(void)
{
    scr_uart_init();
    scr_rtc_init();

    exit(main(0, 0));
}
