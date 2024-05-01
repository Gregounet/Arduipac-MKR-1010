#ifndef ARDUIPAC_H
#define ARDUIPAC_H

#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

extern Adafruit_ST7789 tft;

#undef DEBUG

#define TRAVOLTA_MKR1010 1
#define TRAVOLTA_PICO 2

#define TRAVOLTA_TARGET TRAVOLTA_PICO

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#include <RTCZero.h>
extern RTCZero rtc;
extern uint8_t minutes;
extern uint8_t previous_minutes;
extern uint8_t seconds;
extern uint8_t previous_seconds;
#endif

#endif /* (ARDUIPAC_H) */