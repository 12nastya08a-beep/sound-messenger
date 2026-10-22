#define MINIAUDIO_IMPLEMENTATION
#include "../third_party/miniaudio.h"
#include <stdio.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define FREQUENCY 1000.0
#define AMPLITUDE 0.2
#define DURATION_SECONDS 1.0

static double g_phase = 0.0;

static void data_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
    float* out = (float*)output;
    double step = 2.0 * 3.14159265358979323846 * FREQUENCY / SAMPLE_RATE;
    for (ma_uint32 i = 0; i < frame_count; i++) {
        float sample = (float)(AMPLITUDE * sin(g_phase));
        g_phase += step;
        out[i] = sample;
    }
    (void)device;
    (void)input;
}

int main(void) {
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = SAMPLE_RATE;
    config.dataCallback = data_callback;

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        printf("Failed to open audio device\n");
        return 1;
    }

    printf("Playing a %.0f Hz tone for %.0f second...\n", FREQUENCY, DURATION_SECONDS);
    ma_device_start(&device);

    ma_sleep((ma_uint32)(DURATION_SECONDS * 1000));

    ma_device_uninit(&device);
    printf("Done.\n");
    return 0;
}