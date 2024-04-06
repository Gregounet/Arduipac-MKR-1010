#ifndef ARDUIPAC_ROM_BIOS_H
#define ARDUIPAC_ROM_BIOS_H

#include <avr/pgmspace.h>

extern const PROGMEM uint8_t rom[4096];

//#define ROM(addr) (rom[(addr) & 0xFFF])

#define ROM(addr) (pgm_read_byte_near (rom + (addr & 0xFFF)))

#endif /* (ARDUIPAC_ROM_BIOS_H) */
