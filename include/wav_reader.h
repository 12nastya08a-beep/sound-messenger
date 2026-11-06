#ifndef WAV_READER_H
#define WAV_READER_H

#include <stddef.h>

float *wav_read(const char *filename, size_t *sample_count);

#endif