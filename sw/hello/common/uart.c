/// Syntacore SCR* infra
///
/// @copyright (C) Syntacore 2015-2017. All rights reserved.
///
/// @brief implementation of UART i/o funcs

#ifndef PLF_SYS_CLK
#error PLF_SYS_CLK
#endif

#ifndef PLF_UART_BAUDRATE
#define PLF_UART_BAUDRATE 115200
#endif

// FPGA UART ports
#define SC1F_UART0_PORT 0xff010000
#define PLF_UART0_16550

#ifdef PLF_UART0_16550
#define SC1F_UART_RXD       (0x00) // receive data
#define SC1F_UART_TXD       (0x00) // transmit data
#define SC1F_UART_IER       (0x01) // interrupt enable register
#define SC1F_UART_FCR       (0x02) // FIFO control register
#define SC1F_UART_CONTROL   (0x03) // line control register
#define SC1F_UART_MCR       (0x04) // modem control register
#define SC1F_UART_STATUS    (0x05) // status register
#define SC1F_UART_DIV_LO    (0x00) // baud rate divisor register, low
#define SC1F_UART_DIV_HI    (0x01) // baud rate divisor register, low

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
#define SC1F_UART_TXD       (0x01) // transmit data
#define SC1F_UART_STATUS    (0x02) // status register
#define SC1F_UART_CONTROL   (0x03) // control register
#define SC1F_UART_BRATE     (0x04) // baud rate divisor register

// UART status register bits
#define SC1F_UART_ST_TEMPTY (0x20) // tx empty
#define SC1F_UART_ST_TRDY   (0x40) // tx not full
#define SC1F_UART_ST_RRDY   (0x80) // rx not empty
#endif // PLF_UART0_16550

#ifndef PLF_UART0_MMIO
#define PLF_UART0_MMIO 32
#endif

#include <stdint.h>

#if PLF_UART0_MMIO == 8
typedef uint8_t sc1f_uart_port_t;
#elif PLF_UART0_MMIO == 32
typedef uint32_t sc1f_uart_port_t;
#else
#error Incorrect PLF UART MMIO width
#endif


// uart low level i/o
static inline void sc1f_uart_write(uintptr_t uart_base, unsigned reg, sc1f_uart_port_t val)
{
    ((volatile sc1f_uart_port_t*)uart_base)[reg] = val;
}

static inline sc1f_uart_port_t sc1f_uart_read(uintptr_t uart_base, unsigned reg)
{
    return ((volatile sc1f_uart_port_t*)uart_base)[reg];
}

// inlines

static inline int sc1f_uart_tx_ready(void)
{
    return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_TRDY;
}

static inline int sc1f_uart_rx_ready(void)
{
    return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_STATUS) & SC1F_UART_ST_RRDY;
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

enum Uart_consts {
    UART_CLK_FREQ = PLF_SYS_CLK,
    UART_BAUD_RATE = PLF_UART_BAUDRATE,
#ifdef PLF_UART0_16550
    UART_115200_CLK_DIVISOR = (UART_CLK_FREQ / UART_BAUD_RATE + 7) / 16,
#else // PLF_UART0_16550
    UART_115200_CLK_DIVISOR = UART_CLK_FREQ / UART_BAUD_RATE,
#endif // PLF_UART0_16550
};

// uart init
void scr_uart_init(void)
{
#ifdef PLF_UART0_16550
    // disable interrupts
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_IER, 0);
    // init MCR
#ifdef PLF_UART0_16550_MCRX
    // enable RxD, OUT1=0, OUT2=0
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_MCR, (1 << 6) | (1 << 3) | (1 << 2));
#else // PLF_UART0_16550_MCRX
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_MCR, 0);
#endif // PLF_UART0_16550_MCRX
    // setup baud rate
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_CONTROL, SC1F_UART_LCR_INIT | SC1F_UART_LCR_DIVL);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_DIV_LO, UART_115200_CLK_DIVISOR & 0xff);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_DIV_HI, (UART_115200_CLK_DIVISOR >> 8) & 0xff);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_CONTROL, SC1F_UART_LCR_INIT);
    // init FIFO
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_FCR, SC1F_UART_FCR_R_RST | SC1F_UART_FCR_T_RST | SC1F_UART_FCR_EN);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_FCR, SC1F_UART_FCR_INIT);
#else // PLF_UART0_16550
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_BRATE, UART_115200_CLK_DIVISOR);
    sc1f_uart_write(SC1F_UART0_PORT, SC1F_UART_CONTROL, 0);
#endif // PLF_UART0_16550
}

int uart_putchar(int c)
{
    sc1f_uart_put(c);
    return c;
}

int uart_getch_nowait(void)
{
    if (sc1f_uart_rx_ready())
        return sc1f_uart_read(SC1F_UART0_PORT, SC1F_UART_RXD);
    return -1; // no input
}

int console_putc(int ch) __attribute__((weak, alias("uart_putchar")));
int console_getc(void) __attribute__((weak, alias("uart_getch_nowait")));
