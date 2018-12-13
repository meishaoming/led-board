#ifndef __CRC16_H__
#define __CRC16_H__

#include <stddef.h>
#include <stdint.h>

uint16_t crc16_calc(uint8_t *buffer, size_t length);

#endif /* #ifndef __CRC16_H__ */
