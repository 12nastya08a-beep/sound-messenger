#include "../include/encoder.h"
#include "../include/wav.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: send \"your message\"\n");
        return 1;
    }

    const char *message = argv[1];

    size_t sample_count = 0;
    float *samples = encoder_encode(message, &sample_count);
    if (samples == NULL) {
        printf("ERROR: encoding failed\n");
        return 1;
    }

    int result = wav_write("message.wav", samples, sample_count);
    free(samples);

    if (result != 0) {
        printf("ERROR: could not write wav file\n");
        return 1;
    }

    printf("OK: message \"%s\" saved to message.wav (%zu samples)\n", message, sample_count);
    return 0;
}