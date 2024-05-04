#ifndef ARDUIPAC_ROM_BIOS_H
#define ARDUIPAC_ROM_BIOS_H

extern uint8_t rom[8192];

#define ROM(addr) (rom[(rom_bank_select & 0x1000) + ((addr) & 0x0FFF)])

#endif /* (ARDUIPAC_ROM_BIOS_H) */

