#include "../include/wav_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

float *wav_read(const char *filename, size_t *sample_count) {
    *sample_count = 0;

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return NULL;
    }

    unsigned char header[44];
    if (fread(header, 1, 44, f) != 44) {
        fclose(f);
        return NULL;
    }

    long data_start = ftell(f);
    fseek(f, 0, SEEK_END);
    long file_end = ftell(f);
    fseek(f, data_start, SEEK_SET);

    long data_bytes = file_end - data_start;
    size_t count = (size_t)(data_bytes / 2);

    float *samples = malloc(count * sizeof(float));
    if (samples == NULL) {
        fclose(f);
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        int16_t value;
        if (fread(&value, sizeof(int16_t), 1, f) != 1) {
            free(samples);
            fclose(f);
            return NULL;
        }
        samples[i] = (float)value / 32767.0f;
    }

    fclose(f);
    *sample_count = count;
    return samples;
}