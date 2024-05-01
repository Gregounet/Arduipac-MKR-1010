#include <Arduino.h>

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

// #undef DEBUG_STDERR
// #define DEBUG_SERIAL
// #undef DEBUG_TFT

uint8_t intel8245_ram[256];
uint8_t collisions;

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
    for (uint8_t dot_idx = 0; dot_idx < NB_DOTS; dot_idx++)
      graphic_tft.fillRect(dots[dot_idx].start_x * 2, dots[dot_idx].start_y, 4, 3, grid_color);
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
          20 + (2 * displayed_chars[char_number].previous_start_x),
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
            Serial.print(20 + 2 * (displayed_chars[char_number].start_x + char_column));
            Serial.print(", ");
            Serial.print(displayed_chars[char_number].start_y + row * 2);
            Serial.print(", 2, 2, ");
            Serial.print(displayed_chars[char_number].color);
            Serial.println(")");
#endif
            graphic_tft.fillRect(
                20 + (2 * displayed_chars[char_number].start_x) + (2 * char_column),
                displayed_chars[char_number].start_y + (row * 2),
                2,
                2,
                random(0,0x10000));
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
#undef DEBUG_SERIAL

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
          20 + (displayed_sprites[sprite_number].previous_start_x * 2),
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
                (20 + displayed_sprites[sprite_number].start_x * 2) + (sprite_column * 2 * displayed_sprites[sprite_number].size),
                (displayed_sprites[sprite_number].start_y) + (sprite_row * 2 * displayed_sprites[sprite_number].size),
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
#define DEBUG_SERIAL

/*
 * Detect Collisions
 */
#undef DEBUG_SERIAL
uint8_t detect_collisions()
{
  /*
   * Data structures for sprites collisions
   */

  bool sprites_to_sprites[NB_SPRITES][NB_SPRITES];
  bool sprites_to_vgrid[NB_SPRITES][NB_V_SEGMENTS];
  bool sprites_to_hgrid[NB_SPRITES][NB_H_SEGMENTS];
  bool sprites_to_dots[NB_SPRITES][NB_DOTS];
  bool sprites_to_chars[NB_SPRITES][NB_CHARS];

  for (uint8_t sprite = 0; sprite < NB_SPRITES; sprite++)
  {
    for (uint8_t second_sprite = 0; second_sprite < NB_SPRITES; second_sprite++)
      sprites_to_sprites[sprite][second_sprite] = false;
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
      sprites_to_vgrid[sprite][v_segment] = false;
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
      sprites_to_hgrid[sprite][h_segment] = false;
    for (uint8_t dot = 0; dot < NB_DOTS; dot++)
      sprites_to_dots[sprite][dot] = false;
    for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
      sprites_to_chars[sprite][char_number] = false;
  }

  bool digest_sprites_to_vgrid[NB_SPRITES];
  bool digest_sprites_to_hgrid[NB_SPRITES];
  bool digest_sprites_to_chars[NB_SPRITES];

  for (uint8_t sprite = 0; sprite < NB_SPRITES; sprite++)
  {
    digest_sprites_to_vgrid[sprite] = false;
    digest_sprites_to_hgrid[sprite] = false;
    digest_sprites_to_chars[sprite] = false;
  }

  /*
   * Data Structures for Chars Collisions
   */

  bool chars_to_vgrid[NB_CHARS][NB_V_SEGMENTS];
  bool chars_to_hgrid[NB_CHARS][NB_H_SEGMENTS];
  bool chars_to_dots[NB_CHARS][NB_DOTS];

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
      chars_to_vgrid[char_number][v_segment] = false;
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
      chars_to_hgrid[char_number][h_segment] = false;
    for (uint8_t dot = 0; dot < NB_DOTS; dot++)
      chars_to_dots[char_number][dot] = false;
  }

  bool digest_chars_to_vgrid = 0;
  bool digest_chars_to_hgrid = 0;

  /*
   * Collisions Sprites to Sprites
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    if (displayed_sprites[sprite_number].start_y != 248)
      for (uint8_t second_sprite_number = 0; second_sprite_number < NB_SPRITES; second_sprite_number++)
      {
        if (((displayed_sprites[second_sprite_number].start_y != 248) && (sprite_number != second_sprite_number)) &&
            ((((displayed_sprites[sprite_number].start_x >= displayed_sprites[second_sprite_number].start_x) &&
               (displayed_sprites[sprite_number].start_x <= displayed_sprites[second_sprite_number].end_x)) ||
              ((displayed_sprites[sprite_number].end_x >= displayed_sprites[second_sprite_number].start_x) &&
               (displayed_sprites[sprite_number].end_x <= displayed_sprites[second_sprite_number].end_x))) &&
             (((displayed_sprites[sprite_number].start_y >= displayed_sprites[second_sprite_number].start_y) &&
               (displayed_sprites[sprite_number].start_y <= displayed_sprites[second_sprite_number].end_y)) ||
              ((displayed_sprites[sprite_number].end_y >= displayed_sprites[second_sprite_number].start_y) &&
               (displayed_sprites[sprite_number].end_y <= displayed_sprites[second_sprite_number].end_y)))))
        {
#ifdef DEBUG_SERIAL
          Serial.print("Collision sprites ");
          Serial.print(sprite_number);
          Serial.print(" avec ");
          Serial.println(second_sprite_number);
#endif
          sprites_to_sprites[sprite_number][second_sprite_number] = true;
        }
      }
  }

  /*
   * Collisions Sprites to Vertical Grid
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
    {
      if (v_segments[v_segment].changed_displayed & 0x01)
      {
        if (
            (((displayed_sprites[sprite_number].start_x >= v_segments[v_segment].start_x) &&
              (displayed_sprites[sprite_number].start_x <= v_segments[v_segment].end_x)) ||
             ((displayed_sprites[sprite_number].end_x >= v_segments[v_segment].start_x) &&
              (displayed_sprites[sprite_number].end_x <= v_segments[v_segment].end_x))) &&
            (((displayed_sprites[sprite_number].start_y >= v_segments[v_segment].start_y) &&
              (displayed_sprites[sprite_number].start_y <= v_segments[v_segment].start_y + 23)) ||
             ((displayed_sprites[sprite_number].end_y >= v_segments[v_segment].start_y) &&
              (displayed_sprites[sprite_number].end_y <= v_segments[v_segment].start_y + 23))))
        {
#ifdef DEBUG_SERIAL
          Serial.print("Collision sprite ");
          Serial.print(sprite_number);
          Serial.print(" x ");
          Serial.print(displayed_sprites[sprite_number].start_x);
          Serial.print(" y ");
          Serial.print(displayed_sprites[sprite_number].start_y);
          Serial.print(" avec v segment ");
          Serial.print(v_segment);
          Serial.print(" x ");
          Serial.print(v_segments[v_segment].start_x);
          Serial.print(" y ");
          Serial.println(v_segments[v_segment].start_y);
#endif
          sprites_to_vgrid[sprite_number][v_segment] = true;
        }
      }
    }
  }

  /*
   * Digest Sprites to Vertical Grid
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
    {
      digest_sprites_to_vgrid[sprite_number] = digest_sprites_to_vgrid[sprite_number] ?: sprites_to_vgrid[sprite_number][v_segment];
    }
  }

  /*
   * Collisions Sprites to Horizontal Grid
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      if (h_segments[h_segment].changed_displayed & 0x01)
      {
        if (
            (((displayed_sprites[sprite_number].start_x >= h_segments[h_segment].start_x) &&
              (displayed_sprites[sprite_number].start_x <= h_segments[h_segment].start_x + 15)) ||
             ((displayed_sprites[sprite_number].end_x >= h_segments[h_segment].start_x) &&
              (displayed_sprites[sprite_number].end_x <= h_segments[h_segment].start_x + 15))) &&
            (((displayed_sprites[sprite_number].start_y >= h_segments[h_segment].start_y) &&
              (displayed_sprites[sprite_number].start_y <= h_segments[h_segment].start_y + 2)) ||
             ((displayed_sprites[sprite_number].end_y >= h_segments[h_segment].start_y) &&
              (displayed_sprites[sprite_number].end_y <= h_segments[h_segment].start_y + 2))))
        {
          sprites_to_hgrid[sprite_number][h_segment] = true;
        }
      }
    }
  }

  /*
   * Digest Sprites to Horizontal Grid
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      digest_sprites_to_hgrid[sprite_number] = digest_sprites_to_hgrid[sprite_number] ?: sprites_to_hgrid[sprite_number][h_segment];
    }
  }

  /*
   * Collisions Sprites to Dots
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    ;
    for (uint8_t dot_number = 0; dot_number < NB_DOTS; dot_number++)
    {
      ;
      // sprites_to_dots[sprite_number][dot_number];
    }
  }

  /*
   * Digest Sprites to Dots
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t dot_number = 0; dot_number < NB_DOTS; dot_number++)
    {
      ;
      // digest_sprites_to_hgrid[sprite_number] = digest_sprites_to_dots[sprite_number] ?: sprites_to_dots[sprite_number][dot_number];
    }
  }

  /*
   * Collisions Sprites to Chars
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    if (displayed_sprites[sprite_number].start_y != 248)
      for (uint8_t disp_char = 0; disp_char < NB_CHARS; disp_char++)
      {
        if ((displayed_chars[disp_char].start_y != 248) &&
            (((displayed_sprites[sprite_number].start_x >= displayed_chars[disp_char].start_x) &&
              (displayed_sprites[sprite_number].start_x <= displayed_chars[disp_char].start_x + 7)) ||
             ((displayed_sprites[sprite_number].end_x >= displayed_chars[disp_char].start_x) &&
              (displayed_sprites[sprite_number].end_x <= displayed_chars[disp_char].start_x + 7))) &&
            (((displayed_sprites[sprite_number].start_y >= displayed_chars[disp_char].start_y) &&
              (displayed_sprites[sprite_number].start_y <= displayed_chars[disp_char].end_y)) ||
             ((displayed_sprites[sprite_number].end_y >= displayed_chars[disp_char].start_y) &&
              (displayed_sprites[sprite_number].end_y <= displayed_chars[disp_char].end_y))))
        {
          sprites_to_chars[sprite_number][disp_char] = true;
#ifdef DEBUG_SERIAL
          Serial.print("Collision sprite ");
          Serial.print(sprite_number);
          Serial.print(" (");
          Serial.print(displayed_sprites[sprite_number].start_x);
          Serial.print(", ");
          Serial.print(displayed_sprites[sprite_number].start_y);
          Serial.print(") avec char ");
          Serial.print(disp_char);
          Serial.print(" (");
          Serial.print(displayed_chars[disp_char].start_x);
          Serial.print(", ");
          Serial.print(displayed_chars[disp_char].start_y);
          Serial.println(")");
#endif
        }
      }
  }

  /*
   * Digest Sprites to Chars
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
    {
      digest_sprites_to_chars[sprite_number] = digest_sprites_to_chars[sprite_number] ?: sprites_to_chars[sprite_number][char_number];
    }
  }

  /*
   * Collisions Chars to Vertical Grid
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
    {
      if (v_segments[v_segment].changed_displayed & 0x01)
      {
        if (
            (((displayed_chars[char_number].start_x >= v_segments[v_segment].start_x) &&
              (displayed_chars[char_number].start_x <= v_segments[v_segment].end_x)) ||
             ((displayed_chars[char_number].start_x + 7 >= v_segments[v_segment].start_x) &&
              (displayed_chars[char_number].start_x + 7 <= v_segments[v_segment].end_x))) &&
            (((displayed_chars[char_number].start_y >= v_segments[v_segment].start_y) &&
              (displayed_chars[char_number].start_y <= v_segments[v_segment].start_y + 23)) ||
             ((displayed_chars[char_number].end_y >= v_segments[v_segment].start_y) &&
              (displayed_chars[char_number].end_y <= v_segments[v_segment].start_y + 23))))
        {
          chars_to_vgrid[char_number][v_segment] = true;
        }
      }
    }
  }

  /*
   * Digest Chars to Vertical Grid
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
    {
      digest_chars_to_vgrid = digest_chars_to_vgrid ?: chars_to_vgrid[char_number][v_segment];
    }
  }

  /*
   * Collisions Chars to Horizontal Grid
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      if (h_segments[h_segment].changed_displayed & 0x01)
      {
        if (
            (((displayed_chars[char_number].start_x >= h_segments[h_segment].start_x) &&
              (displayed_chars[char_number].start_x <= h_segments[h_segment].start_x + 15)) ||
             ((displayed_chars[char_number].start_x + 7 >= h_segments[h_segment].start_x) &&
              (displayed_chars[char_number].start_x + 7 <= h_segments[h_segment].start_x + 15))) &&
            (((displayed_chars[char_number].start_y >= h_segments[h_segment].start_y) &&
              (displayed_chars[char_number].start_y <= h_segments[h_segment].start_y + 2)) ||
             ((displayed_chars[char_number].end_y >= h_segments[h_segment].start_y) &&
              (displayed_chars[char_number].end_y <= h_segments[h_segment].start_y + 2))))
        {
          chars_to_hgrid[char_number][h_segment] = true;
        }
      }
    }
  }

  /*
   * Digest Chars to Horizontal Grid
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      digest_chars_to_hgrid = digest_chars_to_hgrid ?: chars_to_hgrid[char_number][h_segment];
    }
  }

  /*
   * Collisions Chars to Dots
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t dot_number = 0; dot_number < NB_DOTS; dot_number++)
    {
      ;
    }
  }

  /*
   * Digest Chars to Dots
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t dot_number = 0; dot_number < NB_DOTS; dot_number++)
    {
      ;
    }
  }

  /*
   * Reply to question...
   */

  uint8_t data = intel8245_ram[0xA2];
  uint8_t result = 0;

  if (data & 0x01) // Sprite 0
  {
    if (sprites_to_sprites[0][1])
      result |= 0x02;
    if (sprites_to_sprites[0][2])
      result |= 0x04;
    if (sprites_to_sprites[0][3])
      result |= 0x08;
  };

  if (data & 0x02) // Sprite 1
  {
    if (sprites_to_sprites[1][0])
      result |= 0x01;
    if (sprites_to_sprites[1][2])
      result |= 0x04;
    if (sprites_to_sprites[1][3])
      result |= 0x08;
  };

  if (data & 0x04) // Sprite 2
  {
    if (sprites_to_sprites[2][0])
      result |= 0x01;
    if (sprites_to_sprites[2][1])
      result |= 0x02;
    if (sprites_to_sprites[2][3])
      result |= 0x08;
  };

  if (data & 0x08) // Sprite 3
  {
    if (sprites_to_sprites[3][0])
      result |= 0x01;
    if (sprites_to_sprites[3][1])
      result |= 0x02;
    if (sprites_to_sprites[3][2])
      result |= 0x04;
  };

  if (data & 0x10) // V Grid
  {
    if (digest_sprites_to_vgrid[0])
      result |= 0x01;
    if (digest_sprites_to_vgrid[1])
      result |= 0x02;
    if (digest_sprites_to_vgrid[2])
      result |= 0x04;
    if (digest_sprites_to_vgrid[3])
      result |= 0x08;
    if (digest_chars_to_vgrid)
      result |= 0x80;
  };

  if (data & 0x20) // H Grid
  {
    if (digest_sprites_to_hgrid[0])
      result |= 0x01;
    if (digest_sprites_to_hgrid[1])
      result |= 0x02;
    if (digest_sprites_to_hgrid[2])
      result |= 0x04;
    if (digest_sprites_to_hgrid[3])
      result |= 0x08;
    if (digest_chars_to_hgrid)
      result |= 0x80;
  };

  if (data & 0x80) // Chars
  {
    if (digest_sprites_to_chars[0])
      result |= 0x01;
    if (digest_sprites_to_chars[1])
      result |= 0x02;
    if (digest_sprites_to_chars[2])
      result |= 0x04;
    if (digest_sprites_to_chars[3])
      result |= 0x08;
    if (digest_chars_to_vgrid)
      result |= 0x10;
    if (digest_chars_to_hgrid)
      result |= 0x20;
  };

  // Serial.print("Checking collisions - ");
  // Serial.println(data, HEX);

  /*
  Serial.print("ext_write() [0x");
  Serial.print(addr, HEX);
  Serial.print("] <- 0x");
  Serial.println(data, HEX);
  if ((data & 0xF0) != 0x00)
     {
         Serial.println("Testing other object than sprite");
         // delay(10000);
     }
  else if ((data & 0x0F) != 0x01 && (data & 0x0F) != 0x02 && (data & 0x0F) != 0x04 && (data & 0x0F) != 0x08)
     {
         Serial.println("Testing many sprites");
         // delay(10000);
     }
  */

  // intel8245_ram[addr] = result;
  // intel8245_ram[addr] = data;
#ifdef DEBUG_SERIAL

  if (displayed_sprites[2].start_x != 240)
  {
    Serial.print("Tir ami: ");
    Serial.print(displayed_sprites[2].start_x);
    Serial.print(" - ");
    Serial.print(displayed_sprites[2].start_y);
    Serial.print(" -> coollision =  ");
    Serial.println(result, HEX);
  }
  if (displayed_sprites[3].start_x != 240)
  {
    Serial.print("Tir ennemi: ");
    Serial.print(displayed_sprites[3].start_x);
    Serial.print(" - ");
    Serial.println(displayed_sprites[3].start_y);
  }
  if ((result != 0x0C) && (result != 0x00))
  {
    // Serial.print("Collision ");
    // Serial.println(result, HEX);
    if ((result & 0x07) == 0x06 || (result & 0x07) == 0x05)
      Serial.println("Dans la goule a Monstroth");
    if ((result & 0x14) == 0x14)
      Serial.println("Tir ami dans le bouclier");
    if ((result & 0x18) == 0x18)
      Serial.println("Tir ennemi dans le bouclier");
    if ((result & 0x84) == 0x84)
      Serial.println("Impact ami");
    if ((result & 0x88) == 0x88)
      Serial.println("Impact ennemi");
    // Serial.println();
    ;
  }
// delay(50);
#endif

  return result;
}
#define DEBUG_SERIAL

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
  delay(1000);
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
  collisions = detect_collisions();
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
