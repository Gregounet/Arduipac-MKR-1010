#include <Arduino.h>
#include <stdint.h>

#include "arduipac.h"
#include "arduipac_graphics.h"
#include "arduipac_8245.h"
#include "arduipac_config.h"

uint8_t *
create_bitmap(int w, int h)
{
  uint8_t *bitmap;
  // ajouter un malloc
  return bitmap;
}

void clear_bitmap(uint8_t *bitmap)
{
  // appeler rectfill()
}

uint8_t *
get_raw_pixel_line(uint8_t *bitmap, uint8_t y)
{
  return (uint8_t *)bitmap + BITMAP_WIDTH * y;
}

uint8_t *
get_raw_pixel(uint8_t *bitmap, uint8_t x, uint8_t y)
{
  return (uint8_t *)bitmap + BITMAP_WIDTH * y + x;
}
