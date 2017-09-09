/// Syntacore SCR* infra
///
/// @copyright (C) Syntacore 2015-2017. All rights reserved.
/// @author mn-sc
///
/// @brief implementation of UART i/o funcs

#include "uart.h"

enum Uart_consts {
    UART_CLK_FREQ = PLF_SYS_FREQ,
    UART_BAUD_RATE = PLF_UART_BAUDRATE,
#ifdef PLF_UART0_16550
    UART_115200_CLK_DIVISOR = (UART_CLK_FREQ / UART_BAUD_RATE + 7) / 16,
#elif defined(PLF_UART0_SCR_RTL)
#else // PLF_UART0_16550
    UART_115200_CLK_DIVISOR = UART_CLK_FREQ / UART_BAUD_RATE,
#endif // PLF_UART0_16550
};

// uart init
void sc1f_uart_init(void)
{
#ifdef PLF_UART0_16550
    // disable interrupts
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_IER, 0);
    // init MCR
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_MCR, 0);
    // setup baud rate
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_CONTROL, SC1F_UART_LCR_INIT | SC1F_UART_LCR_DIVL);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_DIV_LO, UART_115200_CLK_DIVISOR & 0xff);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_DIV_HI, (UART_115200_CLK_DIVISOR >> 8) & 0xff);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_CONTROL, SC1F_UART_LCR_INIT);
    // init FIFO
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_FCR, SC1F_UART_FCR_R_RST | SC1F_UART_FCR_T_RST);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_FCR, SC1F_UART_FCR_INIT & ~SC1F_UART_FCR_RMASK);
    if (SC1F_UART_FCR_INIT & SC1F_UART_FCR_EN)
        sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_FCR, SC1F_UART_FCR_INIT);
#elif defined(PLF_UART0_SCR_RTL)
#else // PLF_UART0_16550
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_BRATE, UART_115200_CLK_DIVISOR);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_CONTROL, 0);
#endif // PLF_UART0_16550
}

void uart_print_info(void)
{
#ifdef PLF_UART0_BASE
    uart_puthex(PLF_UART0_BASE);
#ifdef PLF_UART0_IRQ
    uart_puts("\tirq ");
    uart_putdec(PLF_UART0_IRQ);
#else
    uart_puts("\t ");
#endif // PLF_UART0_IRQ
#ifdef PLF_UART0_16550
    uart_puts("\tUART16550");
#else
    uart_puts("\tUART");
#endif // PLF_UART0_16550
    uart_putc('\n');
#endif // PLF_UART0_BASE
}

int sc1f_uart_putchar(int c)
{
    if (c == '\n') {
        sc1f_uart_put('\r');
    }
    sc1f_uart_put(c);
    return c;
}

#ifdef putchar
#undef putchar
#endif

int putchar(int ch) __attribute__((weak, alias("sc1f_uart_putchar")));

void uart_puts(const char *s)
{
    while (*s)
        sc1f_uart_putchar(*s++);
}

void uart_puthex64(uint64_t val)
{
    uart_puthex32(val >> 32);
    uart_puthex32(val);
}

void uart_puthex32(uint32_t val)
{
    uart_puthex16(val >> 16);
    uart_puthex16(val);
}

void uart_puthex16(uint16_t val)
{
    uart_puthex8(val >> 8);
    uart_puthex8(val);
}

void uart_puthex8(uint8_t val)
{
    uart_puthex4(val >> 4);
    uart_puthex4(val);
}

void uart_puthex4(uint8_t val)
{
    uint8_t c = val & 0xf;
    sc1f_uart_putchar(c + (c > 9 ? ('A' - 10) : '0'));
}

void uart_putdec(unsigned long v)
{
    unsigned long res0 = 0;
    unsigned long i0 = 0;

    if (!v) {
        sc1f_uart_putchar('0');
        return;
    }

    for (; i0 < 8 && v; ++i0) {
        res0 = (res0 << 4) | v % 10;
        v /= 10;
    }

    if (v) {
        unsigned long res1 = 0;
        unsigned long i1 = 0;
        for (; i1 < 2 && v; ++i1) {
            res1 = (res1 << 4) | v % 10;
            v /= 10;
        }
        while (i1--) {
            sc1f_uart_putchar((res1 & 0xf) + '0');
            res1 >>= 4;
        }
    }
    while (i0--) {
        sc1f_uart_putchar((res0 & 0xf) + '0');
        res0 >>= 4;
    }
}

unsigned long uart_read_hex(void)
{
    unsigned len = 0;
    unsigned long res = 0;
    while (1) {
        int c = uart_getc();
        if ((c >= '0' && c <= '9') || ((c & ~0x20) >= 'A' && (c & ~0x20) <= 'F')) {
            if (len >= sizeof(unsigned long) * 2)
                continue;
            sc1f_uart_putchar(c);
            if (c > '9')
                c = (c & ~0x20) - 'A' + 10;
            else
                c = c - '0';
            ++len;
            res = (res << 4) + (c & 0xf);
        } else if (c == 0x7f || c == 0x8) {
            if (!len)
                continue;
            --len;
            res >>= 4;
            sc1f_uart_putchar(0x8);
            sc1f_uart_putchar(' ');
            sc1f_uart_putchar(0x8);
        } else if (c == '\r') {
            sc1f_uart_putchar('\n');
            break;
        }
    }

    return res;
}

unsigned uart_read_str(char *buf, unsigned size)
{
    unsigned len = 0;

    while (1) {
        int c = uart_getc();
        if (c > 0x20 && c < 0x7f) {
            if (len + 1 >= size)
                continue;
            sc1f_uart_putchar(c);
            buf[len++] = c;
        } else if (c == 0x7f || c == 0x8) {
            if (!len)
                continue;
            --len;
            sc1f_uart_putchar(0x8);
            sc1f_uart_putchar(' ');
            sc1f_uart_putchar(0x8);
        } else if (c == '\r') {
            if (size)
                buf[len] = 0;
            sc1f_uart_putchar('\n');
            break;
        }
    }

    return len;
}
