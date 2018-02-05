#include <sys/stat.h>

#include "nlib.h"

int __attribute__((used)) close(int file)
{
    return -1;
}

int __attribute__((used)) fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int __attribute__((used)) isatty(int file)
{
    return 1;
}

int __attribute__((used)) lseek(int file, int ptr, int dir)
{
    return 0;
}

int __attribute__((used)) open(const char *name, int flags, int mode)
{
    return -1;
}

int __attribute__((used,optimize("no-unroll-loops"))) read(int file, char *ptr, int len)
{
    int res = 0;

    int c;

    while ((res < len) && ((c = console_getc()) >= 0))
        ptr[res++] = (char)c;

    return res;
}

int __attribute__((used,optimize("no-unroll-loops"))) write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; ++i)
        console_putc(*ptr++);

    return len;
}

caddr_t __attribute__((used)) sbrk(int incr)
{
    static char *heap_end = 0;

    extern char _end; /* Defined by the linker */
    extern char __STACK_START__; /* Defined by the linker */

    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;

    if (heap_end + incr > &__STACK_START__) {
        /* Heap and stack collision */
        return (caddr_t)0;
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}
