#ifndef TRAVOLTA_VMACHINE_H
#define TRAVOLTA_VMACHINE_H

#include <stdint.h>
#include <stdbool.h>

extern uint16_t vertical_clock;
extern uint8_t horizontal_clock;

/*
 * Relatif au 8245
 */
#define LINECNT 21  // Cycles instructions pas ligne mais ça devrait être 22

#define START_VBLCLK 5493 // Valeur commune à NTSC et PAL mais devrait être 5466
#define END_VBLCLK 5964   // NTSC mais ça devrait être 5967
// #define END_VBLCLK   7259 // PAL

extern uint8_t machine_state;

/*
 * Relatif aux ports I/O du 8048
 */

void ext_write(uint8_t data, uint8_t addr);
uint8_t ext_read(uint8_t addr);
uint8_t read_t1() ;
void write_port1(uint8_t data);

/*
 * Autre
 */
void init_vmachine();

#endif /* TRAVOLTA_VMACHINE_H */
