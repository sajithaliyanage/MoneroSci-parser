//
// Created by sajithaliyanage on 26/5/18.
//

#ifndef MONEROSCI_HASHFN_H
#define MONEROSCI_HASHFN_H

#include "base.h"

typedef uint64_t uint64Array[30];
static int precomputedArraySize = sizeof(uint64Array) / sizeof(uint64_t);

inline uint64_t customPow(uint64Array *precomputedPowers, bool usePrecomputed,
                          uint64_t base, int exp) {
    if (usePrecomputed && exp < precomputedArraySize) {
        return (*precomputedPowers)[exp];
    }

    // TOOD: Optimization possible here when passed in toSize which is bigger
    // than precomputedArraySize, we can start from the value of the last
    // precomputed value.
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}


// Functor for a hashing function
// Implements a Rabin fingerprint hash function
class HashFn {
public:
    // Initialize a HashFn with the prime p which is used as the base of the Rabin
    // fingerprint algorithm
    explicit HashFn(int p, bool precompute = true) {
        this->p = p;
        this->precompute = precompute;
        if (precompute) {
            uint64_t result = 1;
            for (int i = 0; i < precomputedArraySize; i++) {
                precomputedPowers[i] = result;
                result *= p;
            }
        }
    }

    virtual uint64_t operator()(const char *input, int len,
                                unsigned char lastCharCode, uint64_t lastHash);

    virtual uint64_t operator()(const char *input, int len);

private:
    int p;
    bool precompute;
    uint64Array precomputedPowers;
};


#endif //MONEROSCI_HASHFN_H
