#ifndef _USER_FIFO_H_
#define _USER_FIFO_H_

typedef struct {
    unsigned int read_pos;
    unsigned int write_pos;
    unsigned int mask;
    char *buf;
} fifo_t;

#define FIFO_INIT(size, buffer) \
{                               \
    .read_pos   = 0,            \
    .write_pos  = 0,            \
    .mask       = size - 1,     \
    .buf        = buffer,  }

#define DEFINE_FIFO_BUFFER(name, size) \
    char name[((size < 2) || (size & (size - 1))) ? -1 : size]

#define DEFINE_FIFO(fifo, size) \
    DEFINE_FIFO_BUFFER(__fifo_##buf, size); \
    fifo_t fifo = FIFO_INIT(size, __fifo_##buf)

#define STATIC_DEFINE_FIFO(fifo, size) \
    static DEFINE_FIFO_BUFFER(__##fifo##_##buf, size); \
    static fifo_t fifo = FIFO_INIT(size, __##fifo##_##buf)

#ifdef __cplusplus
extern "C" {
#endif

unsigned int fifo_size(fifo_t *fifo);
unsigned int fifo_len(fifo_t *fifo);
unsigned int fifo_avail(fifo_t *fifo);

int fifo_is_full(fifo_t *fifo);
int fifo_is_empty(fifo_t *fifo);

void fifo_reset(fifo_t *fifo);

int fifo_put(fifo_t *fifo, char ch);
int fifo_get(fifo_t *fifo, char *ch);
int fifo_peek(fifo_t *fifo, char *ch);
int fifo_peek_buf(fifo_t *fifo, char *buf, unsigned int offs, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _USER_FIFO_H_
