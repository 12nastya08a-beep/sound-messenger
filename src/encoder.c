#include "encoder.h"
#include "protocol.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define BITS_PER_BYTE 8
#define TWO_PI 6.28318530717958647692f

size_t encoder_samples_per_bit(void) {
    return (size_t)(SAMPLE_RATE * BIT_DURATION + 0.5f);
}

size_t encoder_bit_count(const char *message) {
    return PREAMBLE_BITS + (strlen(message) + 1) * BITS_PER_BYTE;
}

static void write_byte_bits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < BITS_PER_BYTE; i++) {
        bits[i] = (byte >> (BITS_PER_BYTE - 1 - i)) & 1;
    }
}

void encoder_message_to_bits(const char *message, uint8_t *bits) {
    size_t length = strlen(message);
    size_t pos = 0;

    for (size_t i = 0; i < PREAMBLE_BITS; i++) {
        bits[pos++] = (i % 2 == 0) ? 1 : 0;
    }
    for (size_t i = 0; i < length; i++) {
        write_byte_bits((uint8_t)message[i], bits + pos);
        pos += BITS_PER_BYTE;
    }
    write_byte_bits(calculate_checksum((const uint8_t *)message, length), bits + pos);
}

void encoder_generate_tone(float *buffer, float frequency, size_t sample_count) {
    for (size_t i = 0; i < sample_count; i++) {
        buffer[i] = sinf(TWO_PI * frequency * (float)i / SAMPLE_RATE);
    }
}

float *encoder_encode(const char *message, size_t *sample_count) {
    size_t bit_count = encoder_bit_count(message);
    size_t per_bit = encoder_samples_per_bit();

    uint8_t *bits = malloc(bit_count);
    float *samples = malloc(bit_count * per_bit * sizeof(float));
    if (bits == NULL || samples == NULL) {
        free(bits);
        free(samples);
        *sample_count = 0;
        return NULL;
    }

    encoder_message_to_bits(message, bits);
    for (size_t i = 0; i < bit_count; i++) {
        float frequency = bits[i] ? FREQ_BIT_1 : FREQ_BIT_0;
        encoder_generate_tone(samples + i * per_bit, frequency, per_bit);
    }

    free(bits);
    *sample_count = bit_count * per_bit;
    return samples;
}
