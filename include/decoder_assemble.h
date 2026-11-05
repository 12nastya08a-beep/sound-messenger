#ifndef DECODER_ASSEMBLE_H
#define DECODER_ASSEMBLE_H

#include <stddef.h>
#include <stdint.h>

/* Повертає рядок, звільняти через free(). При помилці повертає NULL. */
char *decoder_assemble(const uint8_t *bits, size_t bit_count);

#endif
