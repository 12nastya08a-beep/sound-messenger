#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoder.h"
#include "protocol.h"

static int check_bits(void) {
    const char *expected = "1010" "01001000" "01001001" "00000001";
    size_t count = encoder_bit_count("HI");
    if (count != strlen(expected)) {
        printf("FAILED: bit count %zu, expected %zu\n", count, strlen(expected));
        return 1;
    }

    uint8_t *bits = malloc(count);
    if (bits == NULL) {
        return 1;
    }
    encoder_message_to_bits("HI", bits);

    int errors = 0;
    for (size_t i = 0; i < count; i++) {
        if (bits[i] != (uint8_t)(expected[i] - '0')) {
            errors++;
        }
    }
    free(bits);

    if (errors != 0) {
        printf("FAILED: %d wrong bits\n", errors);
        return 1;
    }
    printf("PASSED: bits for \"HI\"\n");
    return 0;
}

static int check_length(void) {
    size_t sample_count = 0;
    float *samples = encoder_encode("HI", &sample_count);
    if (samples == NULL) {
        printf("FAILED: encoder_encode returned NULL\n");
        return 1;
    }
    free(samples);

    size_t expected = (PREAMBLE_BITS + 8 * 2 + 8) * encoder_samples_per_bit();
    if (sample_count != expected) {
        printf("FAILED: length %zu, expected %zu\n", sample_count, expected);
        return 1;
    }
    printf("PASSED: sample count %zu\n", sample_count);
    return 0;
}

int main(void) {
    int failed = check_bits() + check_length();
    return failed;
}
