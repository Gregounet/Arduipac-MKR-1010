#ifndef ARDUIPAC_CSET_H
#define ARDUIPAC_CSET_H

#include <stdint.h>
#include <avr/pgmspace.h>

extern const PROGMEM  uint8_t cset[512];

#define CSET(addr) (pgm_read_byte_near (cset + ((addr) & 0xFFF)))

#endif /* ARDUIPAC_CSET_H */
