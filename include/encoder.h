#ifndef ENCODER_H
#define ENCODER_H

#include <stddef.h>
#include <stdint.h>

size_t encoder_samples_per_bit(void);
size_t encoder_bit_count(const char *message);
void encoder_message_to_bits(const char *message, uint8_t *bits);
void encoder_generate_tone(float *buffer, float frequency, size_t sample_count);

/* Повертає масив семплів, звільняти через free(). При помилці повертає NULL. */
float *encoder_encode(const char *message, size_t *sample_count);

#endif
