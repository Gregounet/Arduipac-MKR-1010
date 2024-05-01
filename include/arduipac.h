#ifndef ARDUIPAC_H
#define ARDUIPAC_H

#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
// #include <RTCZero.h>

extern Adafruit_ST7789 graphic_tft;
// extern RTCZero rtc;

#undef DEBUG

extern uint8_t minutes;
extern uint8_t previous_minutes;
extern uint8_t seconds;
extern uint8_t previous_seconds;

#endif /* (ARDUIPAC_H) */