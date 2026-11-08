#include <math.h>
#include <stdlib.h>
#include "decoder_fft.h"
#include "protocol.h"
#include "kiss_fft.h"

int decode_bit_fft(const float *samples, size_t num_samples) {
    kiss_fft_cfg cfg = kiss_fft_alloc((int)num_samples, 0, NULL, NULL);
    kiss_fft_cpx *in = malloc(sizeof(kiss_fft_cpx) * num_samples);
    kiss_fft_cpx *out = malloc(sizeof(kiss_fft_cpx) * num_samples);

    for (size_t i = 0; i < num_samples; i++) {
        in[i].r = samples[i];
        in[i].i = 0.0f;
    }

    kiss_fft(cfg, in, out);

    double freq_bin_size = (double)SAMPLE_RATE / num_samples;
    int k0 = (int)(FREQ_BIT_0 / freq_bin_size + 0.5);
    int k1 = (int)(FREQ_BIT_1 / freq_bin_size + 0.5);

    double energy_0 = (double)out[k0].r * out[k0].r + (double)out[k0].i * out[k0].i;
    double energy_1 = (double)out[k1].r * out[k1].r + (double)out[k1].i * out[k1].i;

    free(cfg);
    free(in);
    free(out);

    return (energy_1 > energy_0) ? 1 : 0;
}