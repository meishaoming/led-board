#include "stm32f0xx_hal.h"
#include <SEGGER_RTT.h>

int _write(int fd, char *ptr, int len)
{
#if 1
    SEGGER_RTT_Write(0, ptr, len);
#else
    char *p = ptr;
    while (len--) {
        if (*p == '\n') {
            console_putchar('\r');
        }
        console_putchar(*p++);
    }
#endif
    return len;
}

void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
    __disable_irq();

    for (;;) {

        printf("assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
               failedexpr, file, line, func ? ", function: " : "", func ? func : "");


        __BKPT(0);

        for (int i = 0; i < 0xFFFFF; i++) {
            // wait here
        }
    }

    __enable_irq();
}
