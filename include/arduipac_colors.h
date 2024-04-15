#ifndef ARDUIPAC_COLORS_H
#define ARDUIPAC_COLORS_H

#include <stdint.h>
#include <avr/pgmspace.h>

extern const PROGMEM uint16_t colors[];

//#define COLORS(color_index) (pgm_read_byte_near (colors + (color_index*2))) // TODO
#define COLORS(color_index) colors[color_index] 

#endif /* (ARDUIPAC_COLORS_H) */
