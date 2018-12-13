#include "serial.h"

static void uart_isr(uart_t *uart);

int serial_init(serial_t *serial, UART_HandleTypeDef *huart, fifo_t *txbuf, fifo_t *rxbuf)
{
    uart_t *uart = &serial->uart;

    serial->txbuf = txbuf;
    serial->rxbuf = rxbuf;

    uart_init(uart, huart, uart_isr);

    uart_tx_irq_disable(uart);
    uart_rx_irq_enable(uart);

    return 0;
}

int serial_write(serial_t *serial, const void *buffer, int length)
{
    uart_t *uart = &serial->uart;
    fifo_t *txbuf = serial->txbuf;

    int data_written = 0;
    const char *ptr = buffer;

    while (data_written < length) {
        if (fifo_is_full(txbuf)) {
            break;
        }
        fifo_put(txbuf, *ptr++);
        data_written++;
    }

    if (!fifo_is_empty(txbuf)) {
        uart_tx_irq_enable(uart);
    }

    return data_written;
}

int serial_read(serial_t *serial, void *buffer, int length)
{
    uart_t *uart = &serial->uart;
    fifo_t *rxbuf = serial->rxbuf;

    int data_read = 0;
    char *ptr = buffer;

    while (data_read < length) {
        if (fifo_is_empty(rxbuf)) {
            break;
        }
        fifo_get(rxbuf, ptr++);
        data_read++;
    }

    if (!fifo_is_full(rxbuf)) {
        uart_rx_irq_enable(uart);
    }

    return data_read;
}

static void uart_isr(uart_t *uart)
{
    serial_t *serial = container_of(uart, serial_t, uart);
    fifo_t *txbuf = serial->txbuf;
    fifo_t *rxbuf = serial->rxbuf;

    if (uart_readable(uart)) {
        while (1) {
            int ch;
            if ((ch = uart_getc(uart)) == -1) {
                break;
            }
            if (!fifo_is_full(rxbuf)) {
                fifo_put(rxbuf, ch);
            } else {
                uart_rx_irq_disable(uart);
            }
        }
    }

    if (uart_writeable(uart)) {
        if (fifo_is_empty(txbuf)) {
            uart_tx_irq_disable(uart);
        } else {
            char ch;
            fifo_get(txbuf, &ch);
            uart_putc(uart, ch);
        }
    }
}
