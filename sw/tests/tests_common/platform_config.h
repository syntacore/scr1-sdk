/// Syntacore SCR* infra
///
/// @copyright (C) Syntacore 2015-2017. All rights reserved.
/// @author mn-sc
///
/// @brief platform config

#ifndef SCR_PLATFORM_CONFIG_H
#define SCR_PLATFORM_CONFIG_H

#define PLF_CPU_NAME "SCR1"
#define PLF_RTC_TIMEBASE 1000000

#ifdef SYS_CLK
#define PLF_SYS_FREQ     SYS_CLK
#else
// sys clk freq, MHz
#define PLF_SYS_FREQ     20000000
#endif
// cpu clk freq
#define PLF_CPU_FREQ     PLF_SYS_FREQ

#define PLF_UART0_16550
//#define PLF_UART0_16550_REG_SHIFT 0

#ifdef RTLSIM
#define PLF_UART0_RTLSIM
#endif // #ifdef RTLSIM
#define PLF_MMIO_BASE        (0xff000000)
#define PLF_MMIO_SIZE        (0x100000)
#define PLF_UART0_BASE        0xff010000


#ifndef PLF_UART_BAUDRATE
#define PLF_UART_BAUDRATE 115200
#endif

#define PLF_UART0_REG_SHIFT 0

#ifndef PLATFORM
#define PLATFORM unknown
#endif

#ifdef PLATFORM_HDR
#include PLATFORM_HDR
#endif

// platform info
#ifndef PLF_CPU_NAME
#define PLF_CPU_NAME "unknown"
#endif
#ifndef PLF_IMPL_STR
#define PLF_IMPL_STR "unknown"
#endif

#define PLF_INTLINE_XLNX_UART 0

#define PLF_IRQ_MAP                      \
        [0 ... 31] = ~0,                 \
        [0] = PLF_INTLINE_XLNX_UART


#endif // SCR_PLATFORM_CONFIG_H
