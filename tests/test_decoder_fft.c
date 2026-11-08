#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "decoder_fft.h"
#include "protocol.h"

#define M_PI 3.14159265358979323846

static void generate_tone(float *samples, size_t count, float freq) {
    for (size_t i = 0; i < count; i++) {
        double t = (double)i / SAMPLE_RATE;
        samples[i] = (float)sin(2.0 * M_PI * freq * t);
    }
}

int main(void) {
    size_t num_samples = (size_t)(SAMPLE_RATE * BIT_DURATION);
    float *samples = malloc(sizeof(float) * num_samples);

    generate_tone(samples, num_samples, FREQ_BIT_0);
    int bit0 = decode_bit_fft(samples, num_samples);

    generate_tone(samples, num_samples, FREQ_BIT_1);
    int bit1 = decode_bit_fft(samples, num_samples);

    free(samples);

    if (bit0 == 0 && bit1 == 1) {
        printf("FFT decoder test PASSED! (Bit 0 -> %d, Bit 1 -> %d)\n", bit0, bit1);
        return 0;
    } else {
        printf("FFT decoder test FAILED! (Bit 0 -> %d, Bit 1 -> %d)\n", bit0, bit1);
        return 1;
    }
}