#include "../include/wav.h"
#include "../include/protocol.h"
#include <stdio.h>
#include <math.h>

int main(void) {
    size_t sample_count = SAMPLE_RATE;
    float samples[SAMPLE_RATE];

    double step = 2.0 * 3.14159265358979323846 * 1000.0 / SAMPLE_RATE;
    double phase = 0.0;
    for (size_t i = 0; i < sample_count; i++) {
        samples[i] = (float)(0.2 * sin(phase));
        phase += step;
    }

    int result = wav_write("test_tone.wav", samples, sample_count);
    if (result == 0) {
        printf("OK: file test_tone.wav created\n");
    } else {
        printf("ERROR: could not write file\n");
    }
    return 0;
}