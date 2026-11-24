#include "../include/encoder.h"
#include "../include/wav.h"
#include "../include/audio_play.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  send \"your message\"            save to message.wav\n");
        printf("  send --live \"your message\"     play through speaker (live)\n");
        return 1;
    }

    int live_mode = 0;
    const char *message = NULL;

    if (strcmp(argv[1], "--live") == 0) {
        if (argc < 3) {
            printf("ERROR: no message after --live\n");
            return 1;
        }
        live_mode = 1;
        message = argv[2];
    } else {
        message = argv[1];
    }

    size_t sample_count = 0;
    float *samples = encoder_encode(message, &sample_count);
    if (samples == NULL) {
        printf("ERROR: encoding failed\n");
        return 1;
    }

    if (live_mode) {
        printf("Playing \"%s\" through speaker...\n", message);
        play_audio(samples, sample_count);
        free(samples);
        printf("Done.\n");
        return 0;
    } else {
        int result = wav_write("message.wav", samples, sample_count);
        free(samples);
        if (result != 0) {
            printf("ERROR: could not write wav file\n");
            return 1;
        }
        printf("OK: message \"%s\" saved to message.wav (%zu samples)\n", message, sample_count);
        return 0;
    }
}