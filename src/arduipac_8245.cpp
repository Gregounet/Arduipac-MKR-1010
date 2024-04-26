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
// #undef DEBUG_TFT

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
    ;
  }

  //
  // Tracé des segments horizontaux
  //
  if (!h_grid_uptodate)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.println("show_grid() - segments horizontaux");
#endif

    h_grid_uptodate = 1;
    for (uint8_t h_seg_idx = 0; h_seg_idx < NB_H_SEGMENTS; h_seg_idx++)
    {
      if (h_segments[h_seg_idx].changed_displayed & 0x02)
      {
        if (h_segments[h_seg_idx].changed_displayed & 0x01)
          graphic_tft.fillRect(h_segments[h_seg_idx].start_x * 2, h_segments[h_seg_idx].start_y, 36, 3, grid_color);
        else
          graphic_tft.fillRect(h_segments[h_seg_idx].start_x * 2, h_segments[h_seg_idx].start_y, 36, 3, background_color);
      }
    }
  }
  //
  // Tracé des segments verticaux
  //

  if (!v_grid_uptodate)
  {
    v_grid_uptodate = 1;
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.println("show_grid() - segments verticaux");
#endif

    for (uint8_t v_seg_idx = 0; v_seg_idx < NB_V_SEGMENTS; v_seg_idx++)
    {
      if (h_segments[v_seg_idx].changed_displayed & 0x02)
      {
        if (v_segments[v_seg_idx].changed_displayed & 0x01)
          graphic_tft.fillRect(v_segments[v_seg_idx].start_x * 2, v_segments[v_seg_idx].start_y, v_segments_width * 2, 24, grid_color);
        else
          graphic_tft.fillRect(v_segments[v_seg_idx].start_x * 2, v_segments[v_seg_idx].start_y, v_segments_width * 2, 24, background_color);
      }
    }
  }
}

/*
 * show_chars()
 *
 */
void show_chars()
{
#if defined(DEBUG_STDERR)
#endif
#if defined(DEBUG_SERIAL)
  Serial.println("show_chars()");
#endif
#if defined(DEBUG_TFT)
#endif
  chars_uptodate = 1;

  for (uint8_t char_number = 0; char_number < 28; char_number++)
  {
#if defined(DEBUG_SERIAL)
    Serial.print("char_number ");
    Serial.println(char_number);
#endif
    if (displayed_chars[char_number].changed_displayed & 0x02) // Char data was changed
    {
#if defined(DEBUG_SERIAL)
      Serial.println("Changed");
      Serial.print("x = ");
      Serial.print(displayed_chars[char_number].start_x);
      Serial.print(", y = ");
      Serial.println(displayed_chars[char_number].start_y);
#endif
      displayed_chars[char_number].changed_displayed &= 0xFD; // Clear change flag
      //
      // "Erase" old char position
      //
      graphic_tft.fillRect(
          20 + 2 * (displayed_chars[char_number].previous_start_x - 8),
          displayed_chars[char_number].previous_start_y,
          16,
          2 * displayed_chars[char_number].previous_height,
          background_color);
      //
      // Record new position as previous one
      //
      displayed_chars[char_number].previous_start_x = displayed_chars[char_number].start_x;
      displayed_chars[char_number].previous_start_y = displayed_chars[char_number].start_y;
      displayed_chars[char_number].previous_height = displayed_chars[char_number].height;
      for (uint8_t row = 0; row < displayed_chars[char_number].height; row++)
      {
#if defined(DEBUG_SERIAL)
        Serial.print("row ");
        Serial.println(row);
#endif
        uint8_t cset_byte = CSET(displayed_chars[char_number].cset_start_address + row);
        uint8_t mask = 0x80;
        for (int8_t char_column = 0; char_column < 8; char_column++)
        {
#if defined(DEBUG_SERIAL)
          Serial.print("char_column = ");
          Serial.println(char_column, HEX);
#endif
          if (cset_byte & mask)
          {
#if defined(DEBUG_SERIAL)
            Serial.println("fillRect");
            Serial.print(20 + 2 * (displayed_chars[char_number].start_x + char_column - 8));
            Serial.print(", ");
            Serial.print(displayed_chars[char_number].start_y + row * 2);
            Serial.print(", 2, 2, ");
            Serial.print(displayed_chars[char_number].color);
            Serial.println(")");
#endif
            graphic_tft.fillRect(
                20 + 2 * (displayed_chars[char_number].start_x + char_column - 8),
                displayed_chars[char_number].start_y + row * 2,
                2,
                2,
                displayed_chars[char_number].color);
          }
          mask >>= 1;
        }
      }
    }
  }
}

/*
 * show_sprites()
 *
 */
void show_sprites()
{
  // TODO: gérer shift et even_shift
  // TODO: tenir compte de "displayed"
#if defined(DEBUG_STDERR)
  fprintf(stderr, "show_sprites()\n");
#endif
#if defined(DEBUG_SERIAL)
  Serial.println("show_sprites()");
#endif
#if defined(DEBUG_TFT)
#endif

  sprites_uptodate = 1;
  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    if (displayed_sprites[sprite_number].changed_displayed & 0x02) // Sprite data was changed
    {
      displayed_sprites[sprite_number].changed_displayed &= 0xFD; // Clear change flag

      //
      // "Erase" old sprite position
      //
      graphic_tft.fillRect(
          20 + displayed_sprites[sprite_number].previous_start_x,
          displayed_sprites[sprite_number].previous_start_y,
          16 * displayed_sprites[sprite_number].previous_size,
          16 * displayed_sprites[sprite_number].previous_size,
          background_color);
      //
      // Record new position as previous one
      //
      displayed_sprites[sprite_number].previous_start_x = displayed_sprites[sprite_number].start_x;
      displayed_sprites[sprite_number].previous_start_y = displayed_sprites[sprite_number].start_y;
      displayed_sprites[sprite_number].previous_size = displayed_sprites[sprite_number].size;

#if defined(DEBUG_SERIAL)
      Serial.print("show_sprites() - sprite numéro ");
      Serial.println(sprite_number);
#endif
      for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
      {
        uint8_t sprite_data = intel8245_ram[0x80 + sprite_number * 0x08 + sprite_row];
#if defined(DEBUG_SERIAL)
        Serial.print("show_sprites() - affichage de la ligne ");
        Serial.print(sprite_row);
        Serial.print(" - data : 0x");
        Serial.println(sprite_data, HEX);
        Serial.print("start_x : ");
        Serial.println(displayed_sprites[sprite_number].start_x);
        Serial.print("start_y : ");
        Serial.println(displayed_sprites[sprite_number].start_y);
        Serial.print("color : 0x");
        Serial.println(displayed_sprites[sprite_number].color, HEX);
        Serial.print("size : ");
        Serial.println(displayed_sprites[sprite_number].size);
#endif

        uint8_t mask = 0x01;

        for (uint8_t sprite_column = 0; sprite_column < 8; sprite_column++)
        {
          if (sprite_data & mask)
          {
            graphic_tft.fillRect(
                20 + displayed_sprites[sprite_number].start_x + sprite_column * 2 * displayed_sprites[sprite_number].size,
                displayed_sprites[sprite_number].start_y + sprite_row * 2 * displayed_sprites[sprite_number].size,
                2 * displayed_sprites[sprite_number].size,
                2 * displayed_sprites[sprite_number].size,
                displayed_sprites[sprite_number].color);
          }
          mask <<= 1;
        }
      }
    }
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
#endif
#ifdef DEBUG_TFT
#endif

  if (!background_uptodate)
  {
    background_uptodate = 1;
    graphic_tft.fillScreen(background_color);
  }

  if (grid_control)
    show_grid();

  if (foreground_control)
  {
    if (!chars_uptodate)
      show_chars();

    if (!sprites_uptodate)
      show_sprites();
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
  init_displayed_chars();
  init_displayed_sprites();
}
