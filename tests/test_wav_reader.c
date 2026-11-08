#include "../include/wav_reader.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t count = 0;
    float *samples = wav_read("message.wav", &count);

    if (samples == NULL) {
        printf("ERROR: could not read message.wav\n");
        return 1;
    }

    printf("OK: read %zu samples from message.wav\n", count);
    printf("First 3 samples: %f %f %f\n", samples[0], samples[1], samples[2]);

    free(samples);
    return 0;
}