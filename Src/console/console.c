#include "serial.h"
#include "fifo.h"
#include "console.h"

STATIC_DEFINE_FIFO(txfifo, 256);
STATIC_DEFINE_FIFO(rxfifo, 1024);

static serial_t serial;

int console_init(UART_HandleTypeDef *huart)
{
    return serial_init(&serial, huart, &txfifo, &rxfifo);
}

int console_putchar(int ch)
{
    uint8_t c = (uint8_t)ch;
    return serial_write(&serial, &c, 1);
}

int console_getchar(void)
{
    uint8_t ch;
    return (serial_read(&serial, &ch, 1) == 1) ? ch : -1;
}

size_t console_read_buf(uint8_t *buf, size_t len)
{
    return serial_read(&serial, buf, len);
}

int console_peek_buf(uint8_t *buf, unsigned int offs, unsigned int len)
{
    return fifo_peek_buf(&rxfifo, (char *)buf, offs, len);
}

size_t console_recv_size(void)
{
    return fifo_len(&rxfifo);
}

void console_process_in_loop(void)
{
    int ch = console_getchar();
    if (ch != -1) {
        console_putchar(ch);
        if (ch == '\r') {
            console_putchar('\n');
        }
    }
}
