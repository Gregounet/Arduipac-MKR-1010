#include <Arduino.h>
#include "arduipac_cset.h"

#define DEBUG 0

const  uint8_t cset[512] PROGMEM = {
  0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,
  0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00,
  0x3C, 0x66, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00,
  0x7C, 0xC6, 0x06, 0x3C, 0x06, 0xC6, 0x7C, 0x00,
  0xCC, 0xCC, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x00,
  0xFE, 0xC0, 0xC0, 0x7C, 0x06, 0xC6, 0x7C, 0x00,
  0x7C, 0xC6, 0xC0, 0xFC, 0xC6, 0xC6, 0x7C, 0x00,
  0xFE, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00,
  0x7C, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0x7C, 0x00,
  0x7C, 0xC6, 0xC6, 0x7E, 0x06, 0xC6, 0x7C, 0x00,
  0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00,
  0x18, 0x7E, 0x58, 0x7E, 0x1A, 0x7E, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00,
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0x00,
  0xFC, 0xC6, 0xC6, 0xFC, 0xC0, 0xC0, 0xC0, 0x00,
  0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00,
  0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00,
  0xFE, 0xC0, 0xC0, 0xF8, 0xC0, 0xC0, 0xFE, 0x00,
  0xFC, 0xC6, 0xC6, 0xFC, 0xD8, 0xCC, 0xC6, 0x00,
  0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
  0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,
  0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00,
  0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,
  0x7C, 0xC6, 0xC6, 0xC6, 0xDE, 0xCC, 0x76, 0x00,
  0x7C, 0xC6, 0xC0, 0x7C, 0x06, 0xC6, 0x7C, 0x00,
  0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xFC, 0x00,
  0xFE, 0xC0, 0xC0, 0xF8, 0xC0, 0xC0, 0xC0, 0x00,
  0x7C, 0xC6, 0xC0, 0xC0, 0xCE, 0xC6, 0x7E, 0x00,
  0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00,
  0x06, 0x06, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00,
  0xC6, 0xCC, 0xD8, 0xF0, 0xD8, 0xCC, 0xC6, 0x00,
  0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0x00,
  0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00,
  0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00,
  0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00,
  0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00,
  0xFC, 0xC6, 0xC6, 0xFC, 0xC6, 0xC6, 0xFC, 0x00,
  0xC6, 0xEE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00,
  0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x00, 0x00,
  0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x00, 0x00,
  0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00,
  0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0x00,
  0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
  0xCE, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xCE, 0x00,
  0x00, 0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x3C, 0x00,
  0x1C, 0x1C, 0x18, 0x1E, 0x18, 0x18, 0x1C, 0x00,
  0x1C, 0x1C, 0x18, 0x1E, 0x18, 0x34, 0x26, 0x00,
  0x38, 0x38, 0x18, 0x78, 0x18, 0x2C, 0x64, 0x00,
  0x38, 0x38, 0x18, 0x78, 0x18, 0x18, 0x38, 0x00,
  0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00,
  0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x18, 0x18, 0x00,
  0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x00,
  0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0x00,
  0x38, 0x38, 0x12, 0xFE, 0xB8, 0x28, 0x6C, 0x00,
  0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x00,
  0x00, 0x00, 0x0C, 0x08, 0x08, 0xFF, 0x7E, 0x00,
  0x00, 0x03, 0x63, 0xFF, 0xFF, 0x18, 0x08, 0x00,
  0x00, 0x00, 0x00, 0x10, 0x38, 0xFF, 0x7E, 0x00,
  0x00, 0x00, 0x00, 0x06, 0x6E, 0xFF, 0x7E, 0x00
};
