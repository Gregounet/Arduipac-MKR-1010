#ifndef ARDUIPAC_H
#define ARDUIPAC_H

#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

extern Adafruit_ST7735 text_tft;
extern Adafruit_ST7789 graphic_tft;

extern void text_print_string(char *text);
extern void text_print_dec(uint32_t number);
extern void text_print_hex(uint32_t number);

#endif /* (ARDUIPAC_H) */