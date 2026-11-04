#include "../include/wav.h"
#include "../include/protocol.h"
#include <stdio.h>
#include <stdint.h>

static void write_u32(FILE *f, uint32_t value) {
    fputc((value >> 0) & 0xFF, f);
    fputc((value >> 8) & 0xFF, f);
    fputc((value >> 16) & 0xFF, f);
    fputc((value >> 24) & 0xFF, f);
}

static void write_u16(FILE *f, uint16_t value) {
    fputc((value >> 0) & 0xFF, f);
    fputc((value >> 8) & 0xFF, f);
}

int wav_write(const char *filename, const float *samples, size_t sample_count) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return 1;
    }

    uint16_t channels = 1;
    uint32_t sample_rate = SAMPLE_RATE;
    uint16_t bits_per_sample = 16;
    uint32_t byte_rate = sample_rate * channels * (bits_per_sample / 8);
    uint16_t block_align = channels * (bits_per_sample / 8);
    uint32_t data_size = (uint32_t)(sample_count * (bits_per_sample / 8));

    fputs("RIFF", f);
    write_u32(f, 36 + data_size);
    fputs("WAVE", f);

    fputs("fmt ", f);
    write_u32(f, 16);
    write_u16(f, 1);
    write_u16(f, channels);
    write_u32(f, sample_rate);
    write_u32(f, byte_rate);
    write_u16(f, block_align);
    write_u16(f, bits_per_sample);

    fputs("data", f);
    write_u32(f, data_size);

    for (size_t i = 0; i < sample_count; i++) {
        float s = samples[i];
        if (s > 1.0f) s = 1.0f;
        if (s < -1.0f) s = -1.0f;
        int16_t value = (int16_t)(s * 32767.0f);
        write_u16(f, (uint16_t)value);
    }

    fclose(f);
    return 0;
}