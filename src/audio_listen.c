#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include "audio_listen.h"
#include "protocol.h"
#include "encoder.h"
#include "decoder_fft.h"
#include "miniaudio.h"
#include <stdlib.h>
#include <string.h>

#define MIN_ENERGY 0.0001f
#define RING_FRAMES (SAMPLE_RATE * 2)

static const uint8_t BEACON[PREAMBLE_BITS] = PREAMBLE_PATTERN;

static void on_capture(ma_device *device, void *out, const void *in, ma_uint32 frames)
{
    ma_pcm_rb *rb = device->pUserData;
    const float *src = in;
    ma_uint32 left = frames;
    (void)out;

    while (left > 0) {
        ma_uint32 n = left;
        void *dst;
        if (ma_pcm_rb_acquire_write(rb, &n, &dst) != MA_SUCCESS || n == 0)
            break;
        memcpy(dst, src, n * sizeof(float));
        ma_pcm_rb_commit_write(rb, n);
        src += n;
        left -= n;
    }
}

static void read_samples(ma_pcm_rb *rb, float *dst, size_t count)
{
    size_t got = 0;
    while (got < count) {
        ma_uint32 n = (ma_uint32)(count - got);
        void *src;
        if (ma_pcm_rb_acquire_read(rb, &n, &src) != MA_SUCCESS || n == 0) {
            nanosleep(&(struct timespec){0, 5000000}, NULL);
            continue;
        }
        memcpy(dst + got, src, n * sizeof(float));
        ma_pcm_rb_commit_read(rb, n);
        got += n;
    }
}

static int has_energy(const float *samples, size_t n)
{
    float sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += samples[i] * samples[i];
    return sum / n > MIN_ENERGY;
}

static int is_beacon(const float *window, size_t spb)
{
    if (!has_energy(window, PREAMBLE_BITS * spb))
        return 0;
    for (size_t i = 0; i < PREAMBLE_BITS; i++) {
        if (decode_bit_fft(window + i * spb, spb) != BEACON[i])
            return 0;
    }
    return 1;
}

static size_t trim_silence(const float *samples, size_t len, size_t spb)
{
    while (len >= spb * (PREAMBLE_BITS + 8) && !has_energy(samples + len - spb, spb))
        len -= spb;
    return len;
}

float *listen_for_message(double timeout_sec, size_t *out_len)
{
    size_t spb = encoder_samples_per_bit();
    size_t step = spb / 4;
    size_t window = PREAMBLE_BITS * spb;
    size_t result_len = (PREAMBLE_BITS + (MAX_MESSAGE_CHARS + 1) * 8) * spb;
    size_t max_steps = (size_t)(timeout_sec * SAMPLE_RATE / step);
    float *buf = calloc(window, sizeof(float));
    float *result = NULL;
    ma_pcm_rb rb;
    ma_device device;
    ma_device_config config;

    *out_len = 0;
    if (buf == NULL)
        return NULL;

    if (ma_pcm_rb_init(ma_format_f32, 1, RING_FRAMES, NULL, NULL, &rb) != MA_SUCCESS) {
        free(buf);
        return NULL;
    }

    config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_f32;
    config.capture.channels = 1;
    config.sampleRate = SAMPLE_RATE;
    config.dataCallback = on_capture;
    config.pUserData = &rb;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        ma_pcm_rb_uninit(&rb);
        free(buf);
        return NULL;
    }
    ma_device_start(&device);

    for (size_t i = 0; i < max_steps; i++) {
        memmove(buf, buf + step, (window - step) * sizeof(float));
        read_samples(&rb, buf + window - step, step);

        if (!is_beacon(buf, spb))
            continue;

        result = malloc(result_len * sizeof(float));
        if (result == NULL)
            break;
        memcpy(result, buf, window * sizeof(float));
        read_samples(&rb, result + window, result_len - window);
        *out_len = trim_silence(result, result_len, spb);
        break;
    }

    ma_device_uninit(&device);
    ma_pcm_rb_uninit(&rb);
    free(buf);
    return result;
}
