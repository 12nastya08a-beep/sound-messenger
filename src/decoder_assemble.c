#include "decoder_assemble.h"
#include "protocol.h"
#include <stdlib.h>

#define BITS_PER_BYTE 8

static int preamble_is_valid(const uint8_t *bits) {
    for (size_t i = 0; i < PREAMBLE_BITS; i++) {
        uint8_t expected = (i % 2 == 0) ? 1 : 0;
        if (bits[i] != expected) {
            return 0;
        }
    }
    return 1;
}

static uint8_t bits_to_byte(const uint8_t *bits) {
    uint8_t byte = 0;
    for (int i = 0; i < BITS_PER_BYTE; i++) {
        byte = (uint8_t)((byte << 1) | bits[i]);
    }
    return byte;
}

char *decoder_assemble(const uint8_t *bits, size_t bit_count) {
    if (bit_count < PREAMBLE_BITS + BITS_PER_BYTE) {
        return NULL;
    }
    size_t data_bits = bit_count - PREAMBLE_BITS;
    if (data_bits % BITS_PER_BYTE != 0 || !preamble_is_valid(bits)) {
        return NULL;
    }

    size_t byte_count = data_bits / BITS_PER_BYTE;
    size_t text_length = byte_count - 1;
    const uint8_t *data_bits_start = bits + PREAMBLE_BITS;

    uint8_t *bytes = malloc(byte_count);
    if (bytes == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < byte_count; i++) {
        bytes[i] = bits_to_byte(data_bits_start + i * BITS_PER_BYTE);
    }

    uint8_t received = bytes[text_length];
    uint8_t calculated = calculate_checksum(bytes, text_length);
    if (received != calculated) {
        free(bytes);
        return NULL;
    }

    char *text = malloc(text_length + 1);
    if (text != NULL) {
        for (size_t i = 0; i < text_length; i++) {
            text[i] = (char)bytes[i];
        }
        text[text_length] = '\0';
    }
    free(bytes);
    return text;
}
