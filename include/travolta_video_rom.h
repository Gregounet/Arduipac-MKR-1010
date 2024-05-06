#ifndef TRAVOLTA_VIDEO_ROM
#define TRAVOLTA_VIDEO_ROM

#include <stdint.h>

extern const uint16_t char_colors[];
extern const uint16_t dark_colors[];
extern const uint16_t light_colors[];

#define CHAR_COLORS(color_index) char_colors[(color_index)]
#define DARK_COLORS(color_index) dark_colors[(color_index)]
#define LIGHT_COLORS(color_index) light_colors[(color_index)]

// Colors copied from MAME:

#define TRAVOLTA_BLACK 0x0000
#define TRAVOLTA_GREY 0x632C
#define TRAVOLTA_RED 0x7800
#define TRAVOLTA_LITE_RED 0xC28A
#define TRAVOLTA_GREEN 0x003A
#define TRAVOLTA_LITE_GREEN 0x562D
#define TRAVOLTA_KHAKI 0x7321
#define TRAVOLTA_YELLOW 0xC5CD
#define TRAVOLTA_BLUE 0x19B7
#define TRAVOLTA_LITE_BLUE 0x5C1E
#define TRAVOLTA_VIOLET 0x9193
#define TRAVOLTA_LITE_VIOLET 0xDC3D
#define TRAVOLTA_BLUE_GREEN 0x2D57
#define TRAVOLTA_LITE_BLUE_GREEN 0x773D
#define TRAVOLTA_LITE_GREY 0xCE79
#define TRAVOLTA_WHITE 0xFFFF

extern const uint8_t cset[512];

#define CSET(addr) cset[(addr) & 0xFFF]

#endif /* TRAVOLTA_VIDEO_ROM */
