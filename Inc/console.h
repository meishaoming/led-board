#ifndef _USER_CONSOLE_H_
#define _USER_CONSOLE_H_

#include <soc.h>
#include <stddef.h>

int console_init(UART_HandleTypeDef *huart);

int console_putchar(int ch);

int console_getchar(void);

size_t console_read_buf(uint8_t *buf, size_t len);

int console_peek_buf(uint8_t *buf, unsigned int offs, unsigned int len);

size_t console_recv_size(void);

void console_process_in_loop(void);

#endif /* #ifndef _USER_CONSOLE_H_ */
