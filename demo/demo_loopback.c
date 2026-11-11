#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "audio_listen.h"
#include "encoder.h"
#include "decoder_fft.h"
#include "decoder_assemble.h"
#include "protocol.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const float *samples;
    size_t len;
    size_t pos;
} Player;

static void on_play(ma_device *device, void *out, const void *in, ma_uint32 frames)
{
    Player *p = device->pUserData;
    float *dst = out;
    (void)in;

    for (ma_uint32 i = 0; i < frames; i++)
        dst[i] = p->pos < p->len ? p->samples[p->pos++] : 0.0f;
}

static void *play_thread(void *arg)
{
    Player *p = arg;
    ma_device device;
    ma_device_config config = ma_device_config_init(ma_device_type_playback);

    config.playback.format = ma_format_f32;
    config.playback.channels = 1;
    config.sampleRate = SAMPLE_RATE;
    config.dataCallback = on_play;
    config.pUserData = p;

    ma_sleep(1000);
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
        return NULL;
    ma_device_start(&device);
    while (p->pos < p->len)
        ma_sleep(10);
    ma_sleep(1000);
    ma_device_uninit(&device);
    return NULL;
}

int main(void)
{
    const char *text = "HI";
    size_t sent_len;
    size_t heard_len;
    float *sent = encoder_encode(text, &sent_len);
    Player player = { sent, sent_len, 0 };
    pthread_t thread;

    if (sent == NULL)
        return 1;

    pthread_create(&thread, NULL, play_thread, &player);
    float *heard = listen_for_message(10.0, &heard_len);
    pthread_join(thread, NULL);

    if (heard == NULL) {
        printf("Маячок не знайдено\n");
        free(sent);
        return 1;
    }
    printf("Маячок знайдено, семплів: %zu\n", heard_len);

    size_t spb = encoder_samples_per_bit();
    size_t bit_count = heard_len / spb;
    uint8_t *bits = malloc(bit_count);
    for (size_t i = 0; i < bit_count; i++)
        bits[i] = (uint8_t)decode_bit_fft(heard + i * spb, spb);

    char *message = NULL;
    for (size_t n = bit_count; n >= PREAMBLE_BITS + 16 && message == NULL; n--)
        message = decoder_assemble(bits, n);

    printf("Розшифровано: %s\n", message ? message : "(помилка)");

    int ok = message != NULL;
    free(message);
    free(bits);
    free(heard);
    free(sent);
    return ok ? 0 : 1;
}
