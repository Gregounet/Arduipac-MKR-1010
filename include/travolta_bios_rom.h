#ifndef TRAVOLTA_ROM_BIOS_H
#define TRAVOLTA_ROM_BIOS_H

extern uint8_t *rom;

extern const uint8_t rom_size;
extern const char bios_name[];
extern const char rom_name[];

#define ROM(addr) (rom[(rom_bank_select & 0x3000) + ((addr) & 0x0FFF)])

#endif /* (TRAVOLTA_ROM_BIOS_H) */
