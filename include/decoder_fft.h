#ifndef DECODER_FFT_H
#define DECODER_FFT_H

#include <stddef.h>

int decode_bit_fft(const float *samples, size_t num_samples);

#endif