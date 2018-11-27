#ifndef _USER_SERIAL_H_
#define _USER_SERIAL_H_

#include <stdbool.h>

#include "uart.h"
#include "fifo.h"

struct serial_t {
    uart_t uart;
    fifo_t *txbuf;
    fifo_t *rxbuf;
};

typedef struct serial_t serial_t;

int serial_init(serial_t *serial, UART_HandleTypeDef *huart, fifo_t *txbuf, fifo_t *rxbuf);

int serial_write(serial_t *serial, const void *data, int length);

int serial_read(serial_t *serial, void *buffer, int length);

#endif
