#include <stdio.h>
#include <assert.h>

#include "serial.h"
#include "fifo.h"
#include "console.h"
#include "ws2812b.h"
#include "crc16.h"

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
    if (sync != SYNC_BYTE) {
        printf("sync 0x%02x\n", sync);
        console_read_buf(cmd_buffer, CMD_HEADER_SIZE);
        return -1;
    }

    uint8_t version = 0;
    console_peek_buf((uint8_t *)&version, CMD_HEADER_SIZE+1, sizeof(version));
    if (version != CMD_VERSION) {
        printf("version 0x%02x\n", version);
        console_read_buf(cmd_buffer, CMD_HEADER_SIZE+1);
        return -1;
    }

    uint16_t cmd_length;
    console_peek_buf((uint8_t *)&cmd_length, CMD_HEADER_SIZE+2, sizeof(cmd_length));

    if (console_recv_size() < (CMD_HEADER_SIZE + 4 + cmd_length + 2)) {
        //printf("cmd_length %d/%d bytes\n", cmd_length, console_recv_size());
        return 1;
    }

    size_t read_len;

    // pop `header + sync + version`
    read_len = console_read_buf(cmd_buffer, (CMD_HEADER_SIZE + 4));
    assert(read_len == (CMD_HEADER_SIZE + 4));
    //printf("header read_len %d\n", read_len);

    read_len = console_read_buf(cmd_buffer, cmd_length);
    if (read_len != cmd_length) {
        printf("cmd read_len %d\n", read_len);
        return 1;
    }

    uint16_t crc_calc = crc16_calc(cmd_buffer, cmd_length);

    uint16_t crc_read = 0;
    read_len = console_read_buf((uint8_t *)&crc_read, sizeof(crc_read));
    if (read_len != sizeof(crc_read)) {
        printf("crc read_len %d\n", read_len);
        return 1;
    }

    if (crc_read != crc_calc) {
        printf("crc calc 0x%04x\n", crc_calc);
        printf("crc read 0x%04x\n", crc_read);
        printf("crc failed\n");
        return 1;
    }

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
            case 'D':
                render(&cmd_buffer[1]);
                break;
            case 'E':
                printf("command test: E\n");
                break;
            case 'R':
                printf("system reset: R\n");
                HAL_Delay(200);
                HAL_NVIC_SystemReset();
                break;
            default:
                break;
            }
        }
#endif
    }
}
