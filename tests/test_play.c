#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "audio_play.h"
#include "protocol.h"

#define M_PI 3.14159265358979323846

int main(void) {
    printf("Generating test tone...\n");
    size_t num_samples = (size_t)(SAMPLE_RATE * BIT_DURATION * 4);
    float *samples = malloc(sizeof(float) * num_samples);

    for (size_t i = 0; i < num_samples; i++) {
        double t = (double)i / SAMPLE_RATE;
        double freq = (i < num_samples / 2) ? FREQ_BIT_0 : FREQ_BIT_1;
        samples[i] = (float)sin(2.0 * M_PI * freq * t);
    }

    printf("Playing audio through speaker...\n");
    play_audio(samples, num_samples);
    printf("Playback finished!\n");

    free(samples);
    return 0;
}