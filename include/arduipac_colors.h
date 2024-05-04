#ifndef ARDUIPAC_COLORS_H
#define ARDUIPAC_COLORS_H

#include <stdint.h>

extern const uint16_t char_colors[];
extern const uint16_t dark_colors[];
extern const uint16_t light_colors[];

#define CHAR_COLORS(color_index) char_colors[(color_index)] 
#define DARK_COLORS(color_index) dark_colors[(color_index)] 
#define LIGHT_COLORS(color_index) light_colors[(color_index)] 

#endif /* (ARDUIPAC_COLORS_H) */
