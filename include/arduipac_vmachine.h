#ifndef ARDUIPAC_VMACHINE_H
#define ARDUIPAC_VMACHINE_H

#include <stdint.h>

#include "arduipac_graphics.h"

#define LINECNT 21
#define MAXLINES 500

#define START_VBLCLK 5493	// Valeur commune à NTSC et PAL
#define END_VBLCLK   5964	// NTSC
// #define END_VBLCLK   7259 // PAL

extern uint32_t master_clk;
extern uint32_t int_clk;
extern uint32_t horizontal_clock;
extern uint8_t collision_table[256];
extern uint8_t mstate;
extern uint8_t intel8245_ram[];

uint8_t read_p2 ();
void write_p1 (uint8_t d);

void ext_write (uint8_t dat, uint8_t addr);
uint8_t ext_read (uint8_t addr);

void handle_start_vbl ();
void handle_end_vbl ();

uint8_t in_bus ();
uint8_t read_t1 ();

void init_vmachine ();

#endif /* ARDUIPAC_VMACHINE_H */
