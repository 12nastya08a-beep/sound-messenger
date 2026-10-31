#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define SAMPLE_RATE 44100
#define FREQ_BIT_0 1000.0f
#define FREQ_BIT_1 2000.0f
#define BIT_DURATION 0.1f
#define PREAMBLE_BITS 4

uint8_t calculate_checksum(const uint8_t *data, size_t length);

#endif