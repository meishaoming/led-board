#ifndef __WS2812_H__
#define __WS2812_H__

#include <stdint.h>

#define LED_NR  87

void ws2812_hw_send(uint8_t *data, uint32_t size);
void ws2812_hw_delay_ms(uint32_t ms);

void ws2812_init(void);
void ws2812_set_pixel_color(int nr, uint8_t r, uint8_t g, uint8_t b);
void ws2812_show(void);

void color_wipe(uint8_t r, uint8_t g, uint8_t b);
void threater_chase(uint8_t r, uint8_t g, uint8_t b);
void rainbow(int wait, int iterations);
void rainbow_cycle(int wait, int iterations);
void threater_chase_rainbow(void);

#endif /* #ifndef __WS2812_H__ */

