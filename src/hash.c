/*
 * hash.c
 */

#include <stdint.h>

#include "hash.h"

uint64_t hash(const unsigned char * str) {
    uint64_t hash = 5381;
    int32_t c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
