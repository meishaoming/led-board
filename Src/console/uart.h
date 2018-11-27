#ifndef _USER_CORE_UART_H_
#define _USER_CORE_UART_H_

#include <soc.h>

struct uart_t;

typedef struct uart_t uart_t;

typedef void (*uart_isr_t)(uart_t *uart);

struct uart_t {
    UART_HandleTypeDef *huart;
    uart_isr_t isr;
};

int uart_init(uart_t *uart, UART_HandleTypeDef *huart, uart_isr_t isr);

void uart_tx_irq_enable(uart_t *uart);
void uart_tx_irq_disable(uart_t *uart);

void uart_rx_irq_enable(uart_t *uart);
void uart_rx_irq_disable(uart_t *uart);

int uart_writeable(uart_t *uart);
int uart_readable(uart_t *uart);

int uart_putc(uart_t *uart, int ch);
int uart_getc(uart_t *uart);

#endif /* #ifndef _USER_CORE_UART_H_ */
