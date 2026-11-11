#ifndef AUDIO_LISTEN_H
#define AUDIO_LISTEN_H

#include <stddef.h>

/* Слухає мікрофон до маячка, потім записує повідомлення.
   Повертає семпли, що починаються з маячка, звільняти через free().
   Якщо маячок не знайдено за timeout_sec, повертає NULL. */
float *listen_for_message(double timeout_sec, size_t *out_len);

#endif
