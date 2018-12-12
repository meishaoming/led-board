#include <stdio.h>
#include <assert.h>

#include "serial.h"
#include "fifo.h"
#include "console.h"
#include "ws2812b.h"

// header + sync + version + length + payload + crc

#define CMD_HEADER_SIZE     2
#define SYNC_BYTE           0xC0
#define CMD_VERSION         0x00

static uint8_t cmd_buffer[1024];

static int try_read_cmd(void)
{
    if (console_recv_size() < CMD_HEADER_SIZE) {
        return -1;
    }

    uint8_t header[CMD_HEADER_SIZE];
    console_peek_buf(header, 0, sizeof(header));

    if (header[0] != 'A' || header[1] != 'T') {
        console_getchar();
        return -1;
    }

    if (console_recv_size() < CMD_HEADER_SIZE + 4) {
        return -1;
    }

    uint8_t sync = 0;
    console_peek_buf(&sync, CMD_HEADER_SIZE, sizeof(sync));
    assert(sync == SYNC_BYTE);
    //printf("sync 0x%02x\n", sync);

    uint8_t version = 0;
    console_peek_buf((uint8_t *)&version, CMD_HEADER_SIZE+1, sizeof(version));
    assert(version == CMD_VERSION);
    //printf("version 0x%02x\n", version);

    uint16_t cmd_length;
    console_peek_buf((uint8_t *)&cmd_length, CMD_HEADER_SIZE+2, sizeof(cmd_length));
    //printf("cmd_length %d/%d bytes\n", cmd_length, console_recv_size());

    if (console_recv_size() < (CMD_HEADER_SIZE + 2 + cmd_length + 2)) {
        return 1;
    }

    size_t read_len;

    // pop `header + sync + version + length`
    read_len = console_read_buf(cmd_buffer, (CMD_HEADER_SIZE + 4));
    assert(read_len == (CMD_HEADER_SIZE + 4));
    //printf("header read_len %d\n", read_len);

    read_len = console_read_buf(cmd_buffer, cmd_length);
    assert(read_len == cmd_length);
    //printf("cmd read_len %d\n", read_len);

    uint16_t crc = 0;
    read_len = console_read_buf((uint8_t *)&crc, sizeof(crc));
    assert(read_len == sizeof(crc));
    //printf("crc 0x%04x\n", crc);

    //crc_check();
    return 0;
}

static void render(uint8_t *data)
{
    for (int i = 0; i < LED_NR; i++) {
        int j = i*3;
        ws2812_set_pixel_color(i, data[j], data[j+1], data[j+2]);
    }
    ws2812_show();
}

void cmd_loop(void)
{
    for (;;) {
#if 0
        size_t read_len = console_recv_size();
        printf("%d\n", read_len);
        if (read_len > 0) {
            console_read_buf(cmd_buffer, read_len);
            for (int i = 0; i < read_len; i++) {
                printf("%02X ", cmd_buffer[i]);
            }
            printf("\n");
        }
        HAL_Delay(1000);
#else
        if (!try_read_cmd()) {

            uint8_t action = cmd_buffer[0];
            switch (action) {
            case 0:
                render(&cmd_buffer[1]);
                break;
            default:
                break;
            }
        }
#endif
    }
}
