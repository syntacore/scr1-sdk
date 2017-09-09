/// Syntacore SCR* infra
///
/// @copyright (C) Syntacore 2015-2017. All rights reserved.
/// @author mn-sc
///
/// @brief UART defs and inline funcs

#ifndef SC1_FPGA_UART_H
#define SC1_FPGA_UART_H

#include "platform_config.h"

#ifdef PLF_UART0_BASE

// FPGA UART ports
#define SC1F_UART0_PORT PLF_UART0_BASE

#ifdef PLF_UART0_16550
#define SC1F_UART_RXD       (0x00) // receive data
#define SC1F_UART_TXD       (0x00) // transmit data
#define SC1F_UART_IER       (0x04) // interrupt enable register
#define SC1F_UART_FCR       (0x08) // FIFO control register
#define SC1F_UART_CONTROL   (0x0c) // line control register
#define SC1F_UART_MCR       (0x10) // modem control register
#define SC1F_UART_STATUS    (0x14) // status register
#define SC1F_UART_DIV_LO    (0x00) // baud rate divisor register, low
#define SC1F_UART_DIV_HI    (0x04) // baud rate divisor register, low

// UART FIFO control register bits
#define SC1F_UART_FCR_RT_1  (0 << 6) // RX FIFO trigger level: 1 byte
#define SC1F_UART_FCR_RT_4  (1 << 6) // RX FIFO trigger level: 4 bytes
#define SC1F_UART_FCR_RT_8  (2 << 6) // RX FIFO trigger level: 8 bytes
#define SC1F_UART_FCR_RT_14 (3 << 6) // RX FIFO trigger level: 14 bytes
#define SC1F_UART_FCR_RMASK (3 << 6) // RX FIFO trigger level mask bits
#define SC1F_UART_FCR_T_RST (1 << 2) // reset TX FIFO
#define SC1F_UART_FCR_R_RST (1 << 1) // reset RX FIFO
#define SC1F_UART_FCR_EN    (1 << 0) // FIFO enable
// FCR initial value: enabled
#define SC1F_UART_FCR_INIT  (SC1F_UART_FCR_RT_1 | SC1F_UART_FCR_EN)
// UART line control register bits
#define SC1F_UART_LCR_DIVL  (1 << 7) // divisor latch access
#define SC1F_UART_LCR_SP    (1 << 5) // sticky parity
#define SC1F_UART_LCR_EPS   (1 << 4) // even parity select
#define SC1F_UART_LCR_PE    (1 << 3) // parity enable
#define SC1F_UART_LCR_SBN   (1 << 2) // number of stop bits (0 - 1, 1 - 1.5/2)
#define SC1F_UART_LCR_CL8   (3 << 0) // character length: 8
#define SC1F_UART_LCR_CL7   (2 << 0) // character length: 7
#define SC1F_UART_LCR_CL6   (1 << 0) // character length: 6
#define SC1F_UART_LCR_CL5   (0 << 0) // character length: 5
#define SC1F_UART_LCR_INIT  SC1F_UART_LCR_CL8 // LCR initial value: 8n1
// UART status register bits
#define SC1F_UART_ST_TEMPTY (1 << 6) // tx empty
#define SC1F_UART_ST_TRDY   (1 << 5) // tx not full
#define SC1F_UART_ST_RRDY   (1 << 0) // rx not empty
#elif defined(PLF_UART0_SCR_RTL)
#define SC1F_UART_TXD       (0x00) // transmit data
#define SC1F_UART_RXD       (0x00) // receive data
#else // PLF_UART0_16550
// UART regs
#define SC1F_UART_RXD       (0x00) // receive data
#define SC1F_UART_TXD       (0x04) // transmit data
#define SC1F_UART_STATUS    (0x08) // status register
#define SC1F_UART_CONTROL   (0x0c) // control register
#define SC1F_UART_BRATE     (0x10) // baud rate divisor register

// UART status register bits
#define SC1F_UART_ST_TEMPTY (0x20) // tx empty
#define SC1F_UART_ST_TRDY   (0x40) // tx not full
#define SC1F_UART_ST_RRDY   (0x80) // rx not empty
#endif // PLF_UART0_16550


#ifndef __ASSEMBLER__

#include <stdint.h>

// uart low level i/o
static inline void sc1f_uart_write(uintptr_t uart_base, unsigned reg, uint16_t val)
{
    *(volatile uint32_t *)(uart_base + reg) = val;
}

static inline uint16_t sc1f_uart_read(uintptr_t uart_base, unsigned reg)
{
    return *(volatile uint32_t *)(uart_base + reg);
}

// inlines

static inline int sc1f_uart_rx_ready(void)
{
    return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_RRDY;
}

static inline int sc1f_uart_tx_ready(void)
{
    return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_TRDY;
}

static inline void sc1f_uart_tx_flush(void)
{
    while ((sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_TEMPTY) == 0);
}

static inline void sc1f_uart_put(uint8_t v)
{
    while (!sc1f_uart_tx_ready());
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_TXD, v);
}

static inline int sc1f_uart_getch_nowait(void)
{
    if (sc1f_uart_rx_ready())
        return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_RXD);
    return -1; // no input
}

static inline int sc1f_uart_getch(void)
{
    int c;

    do {
        c = sc1f_uart_getch_nowait();
    } while (c == -1);

    return c;
}

// complex uart init/io
void sc1f_uart_init(void);
void uart_print_info(void);
int sc1f_uart_putchar(int v);
void uart_puthex4(uint8_t val);
void uart_puthex8(uint8_t val);
void uart_puthex16(uint16_t val);
void uart_puthex32(uint32_t val);
void uart_puthex64(uint64_t val);
void uart_putdec(uint32_t v);
void uart_puts(const char *s);
unsigned long uart_read_hex(void);
unsigned uart_read_str(char *buf, unsigned size);
// inlines
static inline void uart_puthex(unsigned long val)
{
#ifdef __riscv128
    uart_puthex32(val >> 32*3);
    uart_puthex32(val >> 32*2);
#endif // __riscv128
#ifdef __riscv64
    uart_puthex32(val >> 32);
#endif // __riscv64
    uart_puthex32(val);
}
static inline int uart_putc(int c)
{
    return sc1f_uart_putchar(c);
}

static inline int uart_getc(void)
{
    return sc1f_uart_getch();
}

#endif // __ASSEMBLER__

#endif // PLF_UART0_BASE

#endif // SC1_FPGA_UART_H
