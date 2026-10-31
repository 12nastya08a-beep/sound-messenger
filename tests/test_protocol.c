#include <stdio.h>
#include "protocol.h"

int main(void) {
    uint8_t test_data[3] = {'A', 'B', 'C'};
    uint8_t expected = 64;
    uint8_t actual = calculate_checksum(test_data, 3);

    if (actual == expected) {
        printf("Checksum test PASSED! (Result: %u)\n", actual);
        return 0;
    } else {
        printf("Checksum test FAILED! Expected %u, got %u\n", expected, actual);
        return 1;
    }
}