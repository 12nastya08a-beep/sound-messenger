#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "miniaudio.h"
#include "audio_play.h"
#include "protocol.h"
#include <time.h>

typedef struct {
    const float *samples;
    size_t num_samples;
    size_t current_index;
} play_context_t;

static void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
    (void)pInput;
    play_context_t *ctx = (play_context_t *)pDevice->pUserData;
    float *out = (float *)pOutput;

    for (ma_uint32 i = 0; i < frameCount; i++) {
        if (ctx->current_index < ctx->num_samples) {
            out[i] = ctx->samples[ctx->current_index++];
        } else {
            out[i] = 0.0f;
        }
    }
}

void play_audio(const float *samples, size_t num_samples) {
    if (!samples || num_samples == 0) return;

    play_context_t ctx = {
        .samples = samples,
        .num_samples = num_samples,
        .current_index = 0
    };

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_f32;
    deviceConfig.playback.channels = 1;
    deviceConfig.sampleRate        = SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &ctx;

    ma_device device;
    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        return;
    }

    while (ctx.current_index < ctx.num_samples) {
        struct timespec ts = {0, 10 * 1000000L};
        nanosleep(&ts, NULL);
    }

    ma_device_uninit(&device);
}