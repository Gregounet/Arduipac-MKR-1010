#include <Arduino.h>
#include <Adafruit_ST77xx.h>

#include "arduipac_colors.h"

#define O2_COLOR_DARK_GREY   0x8410
#define O2_COLOR_LIGHT_GREY  0xC618
#define O2_COLOR_VIOLET      0xC418
#define O2_COLOR_DARK_BLUE   0x0010
#define O2_COLOR_DARK_GREEN  0x0400
#define O2_COLOR_LIGHT_GREEN 0x87F0

const uint16_t char_colors[8] 
 = {
    O2_COLOR_DARK_GREY,
    ST77XX_RED,
    ST77XX_GREEN,
    ST77XX_ORANGE,
    ST77XX_BLUE,
    O2_COLOR_VIOLET,
    O2_COLOR_LIGHT_GREY,
    ST77XX_WHITE
    };

const uint16_t dark_colors[8]  = {
    ST77XX_BLACK,
    O2_COLOR_DARK_BLUE,
    O2_COLOR_DARK_GREEN,
    O2_COLOR_LIGHT_GREEN,
    ST77XX_RED,
    O2_COLOR_VIOLET,
    ST77XX_ORANGE,
    O2_COLOR_LIGHT_GREY
    };

const uint16_t light_colors[8] 
 = {
    ST77XX_BLACK,
    ST77XX_BLUE,
    ST77XX_GREEN,
    O2_COLOR_LIGHT_GREEN,
    ST77XX_RED,
    O2_COLOR_VIOLET,
    ST77XX_ORANGE,
    O2_COLOR_LIGHT_GREY
    };

