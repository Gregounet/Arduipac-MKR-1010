#ifndef TRAVOLTA_H
#define TRAVOLTA_H

#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

extern Adafruit_ST7789 tft;

#define DEBUG

#undef TRAVOLTA_INVERT_DISPLAY

#define EXTERNAL_KEYPADS 1
#undef DIRECT_KEYPAD

#undef EXTERNAL_JOYSTICKS
#define DIRECT_JOYSTICK 1

#define TRAVOLTA_MKR1010 1
#define TRAVOLTA_PICO 2
#define TRAVOLTA_ESP32C3 3
#define TRAVOLTA_ESP32_NANO 4

#define TRAVOLTA_TARGET TRAVOLTA_ESP32_NANO

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#include <RTCZero.h>
extern RTCZero rtc;
extern uint8_t minutes;
extern uint8_t previous_minutes;
extern uint8_t seconds;
extern uint8_t previous_seconds;
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#define GRAPHIC_TFT_CS 6
#define GRAPHIC_TFT_RST -1
#define GRAPHIC_TFT_DC 7

#define SLOW_DOWN_DELAY 1
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_PICO)
#define GRAPHIC_TFT_CS 17
#define GRAPHIC_TFT_RST 21
#define GRAPHIC_TFT_DC 20

#define SLOW_DOWN_DELAY 2
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32C3)
#define GRAPHIC_TFT_CS 7
#define GRAPHIC_TFT_RST 8
#define GRAPHIC_TFT_DC 9

#define SLOW_DOWN_DELAY 1
#endif

#if TRAVOLTA_TARGET == TRAVOLTA_ESP32_NANO
#define GRAPHIC_TFT_CS 10
#define GRAPHIC_TFT_RST -1
#define GRAPHIC_TFT_DC 9

#define SLOW_DOWN_DELAY 10
#endif

const int graphic_tft_cs = GRAPHIC_TFT_CS;
const int graphic_tft_rst = GRAPHIC_TFT_RST;
const int graphic_tft_dc = GRAPHIC_TFT_DC;

#endif /* (TRAVOLTA_H) */