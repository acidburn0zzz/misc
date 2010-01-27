/* If something really failed */

#include <stdio.h>
#include <stdarg.h>

void panic(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    printf("Kernel panic: ");
    vprintf(fmt, args);
    printf("\n"); /* Won't show anything if not there */
    va_end(args);

    for (;;);
}
