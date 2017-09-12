
CROSS_PREFIX ?= $(CROSS_PATH)$(if $(CROSS_PATH),/)riscv32-unknown-elf-
CC = $(CROSS_PREFIX)gcc
LD = $(CC)
OBJDUMP = $(CROSS_PREFIX)objdump
OBJCOPY = $(CROSS_PREFIX)objcopy

MARCH ?= rv32im
MABI ?= ilp32
MEM ?= tcm
OPT ?= 2
SYS_CLK ?= 25000000
RTC_HZ ?= 1000000
UART_BAUDRATE ?= 115200

build_siffix := 

ld_script = $(MEM).ld

ifeq ("$(OPT)","3lto")
PORT_CFLAGS=-O3 -funroll-loops -fpeel-loops -fgcse-sm -fgcse-las
XLFLAGS=-flto
build_siffix := $(build_siffix)-o3lto
endif

ifeq ("$(OPT)","3")
PORT_CFLAGS=-O3
XLFLAGS=
build_siffix := $(build_siffix)-o3
endif

ifeq ("$(OPT)","2")
PORT_CFLAGS=-O2
XLFLAGS=
build_siffix := $(build_siffix)-o2
endif

ifeq ("$(OPT)","2lto")
PORT_CFLAGS=-O2 -funroll-loops
XLFLAGS=-flto
build_siffix := $(build_siffix)-o2lto
endif

PORT_CFLAGS+=$(EXT_PORT_CFLAGS)

ifdef RTLSIM
PORT_CFLAGS+=-DRTLSIM
endif

bmarks_defs += -DRTC_HZ=$(RTC_HZ) -DSYS_CLK=$(SYS_CLK) -DUART_BAUDRATE=$(UART_BAUDRATE)

common_c_src = sc_print.c uart.c
common_asm_src = crt-noreloc.S
