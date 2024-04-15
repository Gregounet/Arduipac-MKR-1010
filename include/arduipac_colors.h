#ifndef ARDUIPAC_COLORS_H
#define ARDUIPAC_COLORS_H

#include <stdint.h>
#include <avr/pgmspace.h>

extern const PROGMEM uint16_t colors[];

#define COLORS(color_o2) (pgm_read_byte_near (colors + (color_o2*2)) // TODO: checker le fonctionnement

#endif /* (ARDUIPAC_COLORS_H) */
