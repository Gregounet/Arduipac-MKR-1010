#include <Arduino.h>

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "arduipac.h"
#include "arduipac_8245.h"
#include "arduipac_8048.h"
#include "arduipac_vmachine.h"
#include "arduipac_cset.h"
#include "arduipac_colors.h"
#include "arduipac_config.h"
#include "arduipac_collisions.h"

#define COLLISION_SP0 0x01
#define COLLISION_SP1 0x02
#define COLLISION_SP2 0x04
#define COLLISION_SP3 0x08
#define COLLISION_VGRID 0x10
#define COLLISION_HGRID 0x20
#define COLLISION_CHAR 0x80

// #undef DEBUG_STDERR
// #define DEBUG_SERIAL
// #define DEBUG_GRID
// #define DEBUG_DETAIL
// #undef DEBUG_TFT

// #undef DEBUG_CHARS
// #undef DEBUG_SPRITES

// #undef DEBUG_DETAIL

uint8_t intel8245_ram[256];
uint8_t collision_table[256]; // TODO Va falloir trouver un moyen de remplacer ce tableau ENORME

/*
 * show_grid()
 *
 */

void show_grid()
{

#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
  Serial.println("show_grid()");
#endif

  if (grid_dots)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.println("draw_grid() - affichage des points");
#endif
    // balayage par lignes de 0 à 8
    for (uint8_t ligne = 0; ligne < 9; ligne++)
      // balayage par colonne de 0 à 9
      for (uint8_t colonne = 0; colonne < 10; colonne++)
        graphic_tft.fillRect(20 + 32 * colonne, 24 + 24 * ligne, 4, 3, grid_color);
  }

  //
  // Tracé des segments horizontaux
  //
  for (uint8_t h_seg_idx = 0; h_seg_idx < NB_H_SEGMENTS; h_seg_idx++)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.print("show_grid() - segments horizontaux");
#endif
    if (h_segments[h_seg_idx].displayed)
      graphic_tft.fillRect(h_segments[h_seg_idx].start_x * 2, h_segments[h_seg_idx].start_y, 36, 3, grid_color);
  }

  //
  // Tracé des segments verticaux
  //

  for (uint8_t v_seg_idx = 0; v_seg_idx < NB_V_SEGMENTS; v_seg_idx++)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.print("show_grid() - segments verticaux");
#endif
    if (v_segments[v_seg_idx].displayed)
      graphic_tft.fillRect(v_segments[v_seg_idx].start_x * 2, v_segments[v_seg_idx].start_y, v_segments_width *2, 24, grid_color);
  }
}
/*
 * Major system
 *
 */

/*
 * show_1char()
 */

void show_1char(uint8_t x, uint8_t y, uint8_t offset, uint8_t char_color_index)
{
  uint16_t char_color = CHAR_COLORS((char_color_index >> 1) & 0x07);
  uint16_t cset_start_address = ((uint16_t)((char_color_index & 1) << 8) + (uint16_t)offset + (uint16_t)(y >> 1)) % 512;
  uint8_t number_of_rows = ((cset_start_address / 8 + 1) * 8) - cset_start_address;

  y &= 0xFE; // TODO pas clair ce Y...

#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
  fprintf(stderr, "show1_char(): x = %d, y = %d, indice dans cset = %0x03x, index couleur = %d\n", x, y, cset_start_offset, char_color_index);
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.print("show1_char() x = ");
  Serial.print(x);
  Serial.print(", y = ");
  Serial.print(y);
  Serial.print(", offset = 0x");
  Serial.print(offset, HEX);
  Serial.print(", indice dans cset = 0x");
  Serial.print(cset_start_address, HEX);
  Serial.print(", char number = 0x");
  Serial.print(cset_start_address / 8, HEX);
  Serial.print(", color_index = 0x");
  Serial.println(char_color_index, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS) && defined(DEBUG_DETAIL)
#endif

  for (uint8_t char_row = 0; char_row < number_of_rows; char_row++)
  {
    uint8_t cset_byte = CSET(cset_start_address + char_row);
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS) && defined(DEBUG_DETAIL)
    Serial.print("cset_byte[0x");
    Serial.print(char_row, HEX);
    Serial.print("] = 0x");
    Serial.println(cset_byte, HEX);
#endif

    uint8_t mask = 0x80;
    for (int8_t char_column = 0; char_column < 8; char_column++)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS) && defined(DEBUG_DETAIL)
      Serial.print("char_column = ");
      Serial.println(char_column, HEX);
#endif
      if (cset_byte & mask)
      {
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS) && defined(DEBUG_DETAIL)
        Serial.print("drawPixel(");
        Serial.print(x + (7 - char_column), HEX);
        Serial.print(", ");
        Serial.print(y + char_row, HEX);
        Serial.print(", ");
        Serial.print(char_color);
        Serial.println(")");
#endif
        graphic_tft.fillRect(20 + 2 * (x + char_column - 8), y + char_row * 2, 2, 2, char_color);
      }
      mask >>= 1;
    }
  }
}

/*
 * show_4quads()
 *
 */

void show_4quads()
{
#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.println("show_4quads()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif
  for (uint8_t quad_indx = 0x40; quad_indx < 0x80; quad_indx += 0x10)
  {
    show_1char(intel8245_ram[quad_indx + 1], intel8245_ram[quad_indx + 0], intel8245_ram[quad_indx + 2], intel8245_ram[quad_indx + 3]);
    show_1char(intel8245_ram[quad_indx + 5], intel8245_ram[quad_indx + 4], intel8245_ram[quad_indx + 6], intel8245_ram[quad_indx + 7]);
    show_1char(intel8245_ram[quad_indx + 9], intel8245_ram[quad_indx + 8], intel8245_ram[quad_indx + 10], intel8245_ram[quad_indx + 11]);
    show_1char(intel8245_ram[quad_indx + 13], intel8245_ram[quad_indx + 12], intel8245_ram[quad_indx + 14], intel8245_ram[quad_indx + 15]);
  }
}

/*
 * show_12chars()
 *
 */

void show_12chars()
{
#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.print(bigben);
  Serial.println(" - show_12chars()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif
  for (uint8_t i = 0; i < 12; i++)
    show_1char(intel8245_ram[0x10 + i * 4 + 1], intel8245_ram[0x10 + i * 4], intel8245_ram[0x10 + i * 4 + 2], intel8245_ram[0x10 + i * 4 + 3]);
}

/*
 * Minor system
 *
 */

/*
 * show_4sprites()
 *
 */

void show_4sprites()
{
  uint8_t sprite_control = 0x00;
  uint8_t sprite_pattern = 0x80;

  uint8_t sprite_data;
  uint16_t sprite_x;
  uint8_t sprite_y;
  uint8_t sprite_color_index;
  uint16_t sprite_color;
  uint8_t sprite_size;
  uint8_t sprite_even_shift;
  uint8_t sprite_full_shift;

#if defined(DEBUG_STDERR) && defined(DEBUG_SPRITES)
  fprintf(stderr, "show_4sprites()\n");
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES)
  Serial.print(bigben);
  Serial.println(" - show_4sprites()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_SPRITES)
#endif

  for (uint8_t sprite_number = 0; sprite_number < 4; sprite_number++)
  {
    sprite_y = intel8245_ram[sprite_control];
    sprite_x = intel8245_ram[sprite_control + 1] - 8;
    sprite_x *= 2;
    sprite_color_index = (intel8245_ram[sprite_control + 2] & 0x38) >> 3;
    sprite_color = CHAR_COLORS(sprite_color_index);

    sprite_size = intel8245_ram[sprite_control + 0x02] & 0x04 >> 2;
    sprite_even_shift = intel8245_ram[sprite_control + 0x02] & 0x02 >> 1;
    sprite_full_shift = intel8245_ram[sprite_control + 0x02] & 0x01;

// #define DEBUG_SERIAL
// #define DEBUG_SPRITES
// #define DEBUG_DETAIL
#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES) && defined(DEBUG_DETAIL)
    Serial.print("show_4sprites() - sprite numéro ");
    Serial.print(sprite_number);
    Serial.print(" , x = ");
    Serial.print(sprite_x);
    Serial.print(" , y = ");
    Serial.print(sprite_y);
    Serial.print(" , color_index = ");
    Serial.print(sprite_color_index);
    Serial.print(" , size = ");
    Serial.print(sprite_size);
    Serial.print(" , even_shift = ");
    Serial.print(sprite_even_shift);
    Serial.print(" , full_shift = ");
    Serial.println(sprite_full_shift);
#endif
    for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
    {
      sprite_data = intel8245_ram[sprite_pattern + sprite_row];
#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES) && defined(DEBUG_DETAIL)
      Serial.print("show_4sprites() - affichage de la ligne ");
      Serial.print(sprite_row);
      Serial.print(" - data : 0x");
      Serial.println(sprite_data, HEX);
#endif

      uint8_t mask = 0x01;

      for (uint8_t sprite_column = 0; sprite_column < 8; sprite_column++)
      {
        {
          if (sprite_data & mask)
          {
            graphic_tft.fillRect(20 + sprite_x + sprite_column * 2, sprite_y + sprite_row * 2, 2, 2, sprite_color);
          }
          mask <<= 1;
        }
      }
    }
    sprite_control += 0x04;
    sprite_pattern += 0x08;
  }
}

/*
 * clear_collision()
 *
 */

void clear_collision()
{
#if defined(DEBUG_STDERR) && defined(DEBUG_SPRITES)
  fprintf(stderr, "clear_collision()\n");
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES)
  Serial.println("clear_collision()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_SPRITES)
#endif
  collision_table[0x01] = 0;
  collision_table[0x02] = 0;
  collision_table[0x04] = 0;
  collision_table[0x08] = 0;
  collision_table[0x10] = 0;
  collision_table[0x20] = 0;
  collision_table[0x80] = 0;
}

/*
 *
 * draw_display()
 *
 */

void draw_display()
{
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
  Serial.println("draw_display()");
  Serial.print("Registre 0xA0 == 0x");
  Serial.println(intel8245_ram[0xA0], HEX);
#endif
#ifdef DEBUG_TFT
#endif
 
  graphic_tft.fillScreen(background_color);

  // bit 3 == enable grid
  if (grid_control)
    show_grid();

  // bit 5 = enable display
  if (foreground_control)
  {
    show_12chars();
    show_4quads();
    show_4sprites();
  }
}

/*
 *
 * init_intel8245()
 */

void init_intel8245()
{
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
  Serial.println("init_intel8245()");
#endif
#ifdef DEBUG_TFT
#endif
  for (uint8_t i = 0x00; i < 0xFF; i++)
    intel8245_ram[i] = 0x00;
  init_grid_elements();
}
