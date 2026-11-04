#ifndef WAV_H
#define WAV_H

#include <stddef.h>

int wav_write(const char *filename, const float *samples, size_t sample_count);

#endif