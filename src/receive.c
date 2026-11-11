#include "../include/wav_reader.h"
#include "../include/decoder_fft.h"
#include "../include/decoder_assemble.h"
#include "../include/protocol.h"
#include "../include/audio_listen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  receive <file.wav>    decode from a wav file\n");
        printf("  receive --live        listen on the microphone (live)\n");
        return 1;
    }

    int live_mode = 0;
    const char *filename = NULL;

    if (strcmp(argv[1], "--live") == 0) {
        live_mode = 1;
    } else {
        filename = argv[1];
    }

    float *samples = NULL;
    size_t sample_count = 0;

    if (live_mode) {
        printf("Listening on the microphone...\n");
        samples = listen_for_message(10.0, &sample_count);
        if (samples == NULL) {
            printf("ERROR: no message detected (no beacon heard)\n");
            return 1;
        }
    } else {
        samples = wav_read(filename, &sample_count);
        if (samples == NULL) {
            printf("ERROR: could not read %s\n", filename);
            return 1;
        }
    }

    size_t samples_per_bit = (size_t)(SAMPLE_RATE * BIT_DURATION);
    size_t bit_count = sample_count / samples_per_bit;

    uint8_t *bits = malloc(bit_count * sizeof(uint8_t));
    if (bits == NULL) {
        printf("ERROR: out of memory\n");
        free(samples);
        return 1;
    }

    for (size_t i = 0; i < bit_count; i++) {
        const float *chunk = samples + i * samples_per_bit;
        bits[i] = (uint8_t)decode_bit_fft(chunk, samples_per_bit);
    }

    char *message = decoder_assemble(bits, bit_count);

    free(samples);
    free(bits);

    if (message == NULL) {
        printf("ERROR: could not decode message (checksum failed?)\n");
        return 1;
    }

    printf("Decoded message: %s\n", message);

    FILE *out = fopen("received.txt", "w");
    if (out != NULL) {
        fprintf(out, "%s\n", message);
        fclose(out);
        printf("(saved to received.txt)\n");
    }
    
    free(message);
    return 0;
}