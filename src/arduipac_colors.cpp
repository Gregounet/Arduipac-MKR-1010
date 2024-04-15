#include <Arduino.h>
#include <Adafruit_ST77xx.h>

#include "arduipac_colors.h"
#include "arduipac_config.h"

const uint16_t colors[8] PROGMEM = {
    ST77XX_CYAN,
    ST77XX_RED,
    ST77XX_GREEN,
    ST77XX_ORANGE,
    ST77XX_BLUE,
    ST77XX_MAGENTA,
    ST77XX_YELLOW,
    ST77XX_WHITE
    };
