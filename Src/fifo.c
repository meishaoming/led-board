#include "fifo.h"

unsigned int fifo_size(fifo_t *fifo)
{
    return fifo->mask + 1;
}

unsigned int fifo_len(fifo_t *fifo)
{
    return fifo->write_pos - fifo->read_pos;
}

unsigned int fifo_avail(fifo_t *fifo)
{
    return fifo_size(fifo) - fifo_len(fifo);
}

int fifo_is_full(fifo_t *fifo)
{
    return fifo_len(fifo) > fifo->mask;
}

int fifo_is_empty(fifo_t *fifo)
{
    return fifo->read_pos == fifo->write_pos;
}

void fifo_reset(fifo_t *fifo)
{
    fifo->read_pos = fifo->write_pos = 0;
}

int fifo_put(fifo_t *fifo, char ch)
{
    if (fifo_is_full(fifo)) {
        return -1;
    }
    fifo->buf[fifo->write_pos & fifo->mask] = ch;
    fifo->write_pos++;
    return 0;
}

int fifo_get(fifo_t *fifo, char *ch)
{
    if (fifo_is_empty(fifo)) {
        return -1;
    }
    *ch = fifo->buf[fifo->read_pos & fifo->mask];
    fifo->read_pos++;
    return 0;
}

int fifo_peek(fifo_t *fifo, char *ch)
{
    if (fifo_is_empty(fifo)) {
        return -1;
    }
    *ch = fifo->buf[fifo->read_pos & fifo->mask];
    return 0;
}

int fifo_peek_buf(fifo_t *fifo, char *buf, unsigned int offs, unsigned int len)
{
    if (fifo_len(fifo) < len) {
        return -1;
    }
    unsigned int read_pos = fifo->read_pos + offs;
    while (len--) {
        *buf = fifo->buf[read_pos & fifo->mask];
        buf++;
        read_pos++;
    }
    return 0;
}
