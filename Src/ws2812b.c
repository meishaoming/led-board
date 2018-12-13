#include <string.h>
#include "ws2812b.h"

/* nr led needs nr*3 bytes == nr*3*8 bits
 * 1 ws2812 bit use 4 spi bit.
 * total bits = nr*3*8*4 bits == nr*3*4 bytes
 */
static uint8_t rgb_buffer[LED_NR*12];

void ws2812_init(void)
{
    for (int i = 0; i < LED_NR; i++) {
        ws2812_set_pixel_color(i, 0, 0, 0);
    }
    ws2812_show();
}

void ws2812_set_pixel_color(int nr, uint8_t r, uint8_t g, uint8_t b)
{
    if (nr < LED_NR) {
        rgb_buffer[nr*12+0]  = ((g & 0x80) ? 0xE0 : 0x80) | ((g & 0x40) ? 0x0E : 0x08);
        rgb_buffer[nr*12+1]  = ((g & 0x20) ? 0xE0 : 0x80) | ((g & 0x10) ? 0x0E : 0x08);
        rgb_buffer[nr*12+2]  = ((g & 0x08) ? 0xE0 : 0x80) | ((g & 0x04) ? 0x0E : 0x08);
        rgb_buffer[nr*12+3]  = ((g & 0x02) ? 0xE0 : 0x80) | ((g & 0x01) ? 0x0E : 0x08);
        rgb_buffer[nr*12+4]  = ((r & 0x80) ? 0xE0 : 0x80) | ((r & 0x40) ? 0x0E : 0x08);
        rgb_buffer[nr*12+5]  = ((r & 0x20) ? 0xE0 : 0x80) | ((r & 0x10) ? 0x0E : 0x08);
        rgb_buffer[nr*12+6]  = ((r & 0x08) ? 0xE0 : 0x80) | ((r & 0x04) ? 0x0E : 0x08);
        rgb_buffer[nr*12+7]  = ((r & 0x02) ? 0xE0 : 0x80) | ((r & 0x01) ? 0x0E : 0x08);
        rgb_buffer[nr*12+8]  = ((b & 0x80) ? 0xE0 : 0x80) | ((b & 0x40) ? 0x0E : 0x08);
        rgb_buffer[nr*12+9]  = ((b & 0x20) ? 0xE0 : 0x80) | ((b & 0x10) ? 0x0E : 0x08);
        rgb_buffer[nr*12+10] = ((b & 0x08) ? 0xE0 : 0x80) | ((b & 0x04) ? 0x0E : 0x08);
        rgb_buffer[nr*12+11] = ((b & 0x02) ? 0xE0 : 0x80) | ((b & 0x01) ? 0x0E : 0x08);
    }
}

void ws2812_show(void)
{
    ws2812_hw_send(rgb_buffer, sizeof(rgb_buffer));
}

void color_wipe(uint8_t r, uint8_t g, uint8_t b)
{
    int wait = 50;
    int i;
    for (i = 0; i < LED_NR; i++) {
        ws2812_set_pixel_color(i, r, g, b);
        ws2812_show();
        ws2812_hw_delay_ms(wait);
    }
}

void threater_chase(uint8_t r, uint8_t g, uint8_t b)
{
    int i, q, j;

    int wait = 50;

    int iterations = 10;

    for (j = 0; j < iterations; j++) {
        for (q = 0; q < 3; q++) {
            for (i = 0; i < LED_NR; i += 3) {
                ws2812_set_pixel_color(i+q, r, g, b);
            }
            ws2812_show();
            ws2812_hw_delay_ms(wait);
            for (i = 0; i < LED_NR; i += 3) {
                ws2812_set_pixel_color(i+q, 0, 0, 0);
            }
        }
    }
}

static void wheel(uint8_t pos, uint8_t *r, uint8_t *g, uint8_t *b) 
{
    if (pos < 85) {
        *r = pos * 3;
        *g = 255 - pos * 3;
        *b = 0;
    }
    else if (pos < 170) {
        pos -= 85;
        *r = 0;
        *g = pos * 3;
        *b = 255 - pos * 3;
    }
    else {
        pos -= 170;
        *r = 0;
        *g = pos * 3;
        *b = 255 - pos * 3;
    }
}

void rainbow(int wait, int iterations)
{
    int i, j;
    uint8_t r, g, b;
    for (j = 0; j < 256 * iterations; j++) {
        for (i = 0; i < LED_NR; i++) {
            wheel((i+j)&0xFF, &r, &g, &b);
            ws2812_set_pixel_color(i, r, g, b);
        }
        ws2812_show();
        ws2812_hw_delay_ms(wait);
    }
}

void rainbow_cycle(int wait, int iterations)
{
    int i, j;
    uint8_t r, g, b;
    for (j = 0; j < 256 * iterations; j++) {
        for (i = 0; i < LED_NR; i++) {
            wheel(((i*256/LED_NR)+j)&0xFF, &r, &g, &b);
            ws2812_set_pixel_color(i, r, g, b);
        }
        ws2812_show();
        ws2812_hw_delay_ms(wait);
    }
}

void threater_chase_rainbow(void)
{
    int wait = 50;
    int i, j, q;
    uint8_t r, g, b;

    for (j = 0; j < 256; j++) {
        for (q = 0; q < 3; q++) {
            for (i = 0; i < LED_NR; i += 3) {
                wheel((i+j)%255, &r, &g, &b);
                ws2812_set_pixel_color(i+q, r, g, b);
            }
            ws2812_show();
            ws2812_hw_delay_ms(wait);
            for (i = 0; i < LED_NR; i += 3) {
                ws2812_set_pixel_color(i+q, 0, 0, 0);
            }
        }
    }
}

