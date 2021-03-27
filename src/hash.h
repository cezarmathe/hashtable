/*
 * hash.h
 */

/*
 * # [djb2]
 *
 * This algorithm (k=33) was first reported by dan bernstein many years ago in
 * comp.lang.c. another version of this algorithm (now favored by bernstein)
 * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33 (why
 * it works better than many other constants, prime or not) has never been
 * adequately explained.
 *
 * ## Motivation
 *
 * This algorithm has a pretty straightforward implementation and good
 * performance, therefore, I think it is a pretty obvious choice.
 *
 * [djb2](http://www.cse.yorku.ca/~oz/hash.html)
 */

#include <stdint.h>

#ifndef HASHTABLE_HASH_H
#define HASHTABLE_HASH_H

/**
 * Create a hash.
 * @param str the source string
 * @return the hash.
 */
uint64_t hash(const unsigned char * str);

#endif //HASHTABLE_HASH_H
