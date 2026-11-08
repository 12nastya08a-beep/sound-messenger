#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decoder_assemble.h"

static const uint8_t good_bits[28] = {
    1, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 0,
    0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 1
};

static const uint8_t bad_bits[28] = {
    1, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 0,
    0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0
};

static int check_good(void) {
    char *text = decoder_assemble(good_bits, 28);
    if (text == NULL) {
        printf("FAILED: valid bits returned NULL\n");
        return 1;
    }
    int ok = strcmp(text, "HI") == 0;
    if (!ok) {
        printf("FAILED: got \"%s\", expected \"HI\"\n", text);
    } else {
        printf("PASSED: valid bits -> \"HI\"\n");
    }
    free(text);
    return ok ? 0 : 1;
}

static int check_bad(void) {
    char *text = decoder_assemble(bad_bits, 28);
    if (text != NULL) {
        printf("FAILED: bad checksum was not detected\n");
        free(text);
        return 1;
    }
    printf("PASSED: bad checksum -> NULL\n");
    return 0;
}

int main(void) {
    int failed = check_good() + check_bad();
    return failed;
}
