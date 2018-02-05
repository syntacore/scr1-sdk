#ifndef NLIBC_H
#define NLIBC_H

/* console i/o */

int console_putc(int ch);
int console_getc(void);

void console_puthex32(unsigned long val);
void console_puthex16(unsigned long val);
void console_puthex8(unsigned long val);
void console_puthex4(unsigned long val);
void console_putstr(const char *str);

static inline void console_puthex64(uint64_t val)
{
    console_puthex32(val >> 32);
    console_puthex32(val);
}

static inline void console_puthex(unsigned long val)
{
#ifdef __riscv128
    console_puthex32(val >> 32*3);
    console_puthex32(val >> 32*2);
#endif
#ifdef __riscv64
    console_puthex32(val >> 32);
#endif
    console_puthex32(val);
}

#endif /* NLIBC_H */
