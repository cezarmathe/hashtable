/*
 * hash_djb2.c
 */

#include <stdint.h>

#include "hash.h"

uint64_t hash_djb2(const unsigned char * str) {
    uint64_t hash = 5381;
    int32_t c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash_djb2 * 33 + c */

    return hash;
}
