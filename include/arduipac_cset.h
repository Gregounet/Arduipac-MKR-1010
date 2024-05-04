#ifndef ARDUIPAC_CSET_H
#define ARDUIPAC_CSET_H

#include <stdint.h>

extern const uint8_t cset[512];

#define CSET(addr) cset[(addr) & 0xFFF]

#endif /* ARDUIPAC_CSET_H */
