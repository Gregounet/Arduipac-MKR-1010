#ifndef ARDUIPAC_H
#define ARDUIPAC_H

#include <stdint.h>

extern void text_print_string(char *text);
extern void text_print_dec(uint32_t number);
extern void text_print_hex(uint32_t number);

Adafruit_ST7735 text_tft;
Adafruit_ST7789 graphic_tft;

#endif /* (ARDUIPAC_H) */