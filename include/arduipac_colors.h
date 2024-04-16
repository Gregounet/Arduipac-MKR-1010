#ifndef ARDUIPAC_COLORS_H
#define ARDUIPAC_COLORS_H

#include <stdint.h>
#include <avr/pgmspace.h>


extern const PROGMEM uint16_t char_colors[];
extern const PROGMEM uint16_t dark_colors[];
extern const PROGMEM uint16_t light_colors[];

//#define CHAR_COLORS(color_index) (pgm_read_byte_near (char_colors + (color_index*2))) // TODO
//#define DARK_COLORS(color_index) (pgm_read_byte_near (dark_colors + (color_index*2))) // TODO
//#define LIGHT_COLORS(color_index) (pgm_read_byte_near (light_colors + (color_index*2))) // TODO

#define CHAR_COLORS(color_index) char_colors[color_index] 
#define DARK_COLORS(color_index) dark_colors[color_index] 
#define LIGHT_COLORS(color_index) light_colors[color_index] 

#endif /* (ARDUIPAC_COLORS_H) */
