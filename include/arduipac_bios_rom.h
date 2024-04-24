#ifndef ARDUIPAC_ROM_BIOS_H
#define ARDUIPAC_ROM_BIOS_H

#include <avr/pgmspace.h>

// extern const PROGMEM uint8_t rom[8192];
extern uint8_t rom[8192];

// #define ROM(addr) (rom[(addr) & 0xFFF])

// #define ROM(addr) (pgm_read_byte_near (rom + (rom_bank_select & 0x1FFF) + ((addr) & 0x0FFF)))
#define ROM(addr) (rom[(rom_bank_select & 0x1000) + ((addr) & 0x0FFF)])

#endif /* (ARDUIPAC_ROM_BIOS_H) */

