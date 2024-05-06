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

#include "travolta.h"
#include "travolta_8245.h"
#include "travolta_8048.h"
#include "travolta_vmachine.h"
#include "travolta_video_rom.h"
#include "travolta_display.h"

// #undef DEBUG

uint8_t intel8245_ram[256];

/*
 * show_grid()
 *
 */

void show_grid()
{
  grid_uptodate = true;

#if defined(DEBUG)
  Serial.println("show_grid()");
#endif

  //
  // Affichage des Dots
  //
  if (!dots_uptodate)
  {
#if defined(DEBUG)
    Serial.println("draw_grid() - affichage des points");
#endif
    if (grid_dots)
      for (uint8_t dot_idx = 0; dot_idx < NB_H_SEGMENTS; dot_idx++)
        tft.fillRect(dots[dot_idx].start_x * 2, dots[dot_idx].start_y, 4, 3, grid_color);
    else
      for (uint8_t dot_idx = 0; dot_idx < NB_H_SEGMENTS; dot_idx++)
        tft.fillRect(dots[dot_idx].start_x * 2, dots[dot_idx].start_y, 4, 3, background_color);
  }

  //
  // Tracé des segments horizontaux
  //

  if (!h_segs_uptodate)
  {
    h_segs_uptodate = true;
#if defined(DEBUG)
    Serial.println("show_grid() - affichage des segments horizontaux");
#endif
    for (uint8_t h_seg_idx = 0; h_seg_idx < NB_H_SEGMENTS; h_seg_idx++)
      if (h_segments[h_seg_idx].changed)
      {
        // Serial.println("affichage d'un segment h");
        h_segments[h_seg_idx].changed = false;
        if (h_segments[h_seg_idx].displayed)
          tft.fillRect(h_segments[h_seg_idx].start_x * 2, h_segments[h_seg_idx].start_y, 36, 3, grid_color);
        else
          tft.fillRect(h_segments[h_seg_idx].start_x * 2, h_segments[h_seg_idx].start_y, 36, 3, background_color);
      }
  }

  //
  // Tracé des segments verticaux
  //
  if (!v_segs_uptodate)
  {
    v_segs_uptodate = true;
#if defined(DEBUG)
    Serial.println("show_grid() - affichage des segments verticaux");
#endif
    for (uint8_t v_seg_idx = 0; v_seg_idx < NB_V_SEGMENTS; v_seg_idx++)
      if (v_segments[v_seg_idx].changed)
      {
        {
          // Serial.println("affichage d'un segment v");
          v_segments[v_seg_idx].changed = false;
          if (v_segments[v_seg_idx].displayed)
            tft.fillRect(v_segments[v_seg_idx].start_x * 2, v_segments[v_seg_idx].start_y, v_segments_width * 2, 24, grid_color);
          else
            tft.fillRect(v_segments[v_seg_idx].start_x * 2, v_segments[v_seg_idx].start_y, v_segments_width * 2, 24, background_color);
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
#if defined(DEBUG)
  Serial.println("show_chars()");
#endif
  chars_uptodate = true;

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
#if defined(DEBUG)
    Serial.print("char_number ");
    Serial.println(char_number);
#endif
    if (displayed_chars[char_number].changed)
    {
#if defined(DEBUG)
      Serial.println("Changed");
      Serial.print("x = ");
      Serial.print(displayed_chars[char_number].start_x);
      Serial.print(", y = ");
      Serial.println(displayed_chars[char_number].start_y);
#endif
      displayed_chars[char_number].changed = false;
      //
      // "Erase" old char position
      //
      tft.fillRect(
          LEFT_OFFSET + (2 * displayed_chars[char_number].previous_start_x),
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
#if defined(DEBUG)
        Serial.print("row ");
        Serial.println(row);
#endif
        uint8_t cset_byte = CSET(displayed_chars[char_number].cset_start_address + row);
        uint8_t mask = 0x80;
        for (int8_t char_column = 0; char_column < 8; char_column++)
        {
#if defined(DEBUG)
          Serial.print("char_column = ");
          Serial.println(char_column, HEX);
#endif
          if (cset_byte & mask)
          {
#if defined(DEBUG)
            Serial.println("fillRect");
            Serial.print(LEFT_OFFSET + 2 * (displayed_chars[char_number].start_x + char_column));
            Serial.print(", ");
            Serial.print(displayed_chars[char_number].start_y + row * 2);
            Serial.print(", 2, 2, ");
            Serial.print(displayed_chars[char_number].color);
            Serial.println(")");
#endif
            tft.fillRect(
                LEFT_OFFSET + (2 * displayed_chars[char_number].start_x) + (2 * char_column),
                displayed_chars[char_number].start_y + (row * 2),
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

#if defined(DEBUG)
  Serial.println("show_sprites()");
#endif

  sprites_uptodate = true;
  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    if (displayed_sprites[sprite_number].changed)
    {
      displayed_sprites[sprite_number].changed = false;
      //
      // "Erase" old sprite position
      //
      tft.fillRect(
          LEFT_OFFSET + (displayed_sprites[sprite_number].previous_start_x * 2),
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

#if defined(DEBUG)
      Serial.print("show_sprites() - sprite numéro ");
      Serial.println(sprite_number);
#endif
      for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
      {
        uint8_t sprite_data = intel8245_ram[0x80 + sprite_number * 0x08 + sprite_row];
#if defined(DEBUG)
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
            tft.fillRect(
                (LEFT_OFFSET + displayed_sprites[sprite_number].start_x * 2) + (sprite_column * 2 * displayed_sprites[sprite_number].size),
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

/*
 * Detect Collisions
 */
uint8_t detect_collisions()
{
  /*
   * Data structures for sprites collisions
   */

  bool sprites_to_sprites[NB_SPRITES][NB_SPRITES];
  bool sprites_to_vgrid[NB_SPRITES][NB_V_SEGMENTS];
  bool sprites_to_hgrid[NB_SPRITES][NB_H_SEGMENTS];
  bool sprites_to_chars[NB_SPRITES][NB_CHARS];

  for (uint8_t sprite = 0; sprite < NB_SPRITES; sprite++)
  {
    for (uint8_t second_sprite = 0; second_sprite < NB_SPRITES; second_sprite++)
      sprites_to_sprites[sprite][second_sprite] = false;
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
      sprites_to_vgrid[sprite][v_segment] = false;
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
      sprites_to_hgrid[sprite][h_segment] = false;
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

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
      chars_to_vgrid[char_number][v_segment] = false;
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
      chars_to_hgrid[char_number][h_segment] = false;
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
#if defined(DEBUG)
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
      if (v_segments[v_segment].displayed)
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
#if defined(DEBUG)
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
      digest_sprites_to_vgrid[sprite_number] = digest_sprites_to_vgrid[sprite_number] ? true : sprites_to_vgrid[sprite_number][v_segment];
    }
  }

  /*
   * Collisions Sprites to Horizontal Grid
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      if (h_segments[h_segment].displayed)
        if (
            (((displayed_sprites[sprite_number].start_x >= h_segments[h_segment].start_x) &&
              (displayed_sprites[sprite_number].start_x <= h_segments[h_segment].start_x + 15)) ||
             ((displayed_sprites[sprite_number].end_x >= h_segments[h_segment].start_x) &&
              (displayed_sprites[sprite_number].end_x <= h_segments[h_segment].start_x + 15))) &&
            (((displayed_sprites[sprite_number].start_y >= h_segments[h_segment].start_y) &&
              (displayed_sprites[sprite_number].start_y <= h_segments[h_segment].start_y + 2)) ||
             ((displayed_sprites[sprite_number].end_y >= h_segments[h_segment].start_y) &&
              (displayed_sprites[sprite_number].end_y <= h_segments[h_segment].start_y + 2))))
          sprites_to_hgrid[sprite_number][h_segment] = true;
      if (grid_dots)
        if (
            (((displayed_sprites[sprite_number].start_x >= dots[h_segment].start_x) &&
              (displayed_sprites[sprite_number].start_x <= dots[h_segment].start_x + 3)) ||
             ((displayed_sprites[sprite_number].end_x >= dots[h_segment].start_x) &&
              (displayed_sprites[sprite_number].end_x <= dots[h_segment].start_x + 3))) &&
            (((displayed_sprites[sprite_number].start_y >= dots[h_segment].start_y) &&
              (displayed_sprites[sprite_number].start_y <= dots[h_segment].start_y + 2)) ||
             ((displayed_sprites[sprite_number].end_y >= dots[h_segment].start_y) &&
              (displayed_sprites[sprite_number].end_y <= dots[h_segment].start_y + 2))))
          sprites_to_hgrid[sprite_number][h_segment] = true;
    }
  }

  /*
   * Digest Sprites to Horizontal Grid
   */

  for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      digest_sprites_to_hgrid[sprite_number] = digest_sprites_to_hgrid[sprite_number] ? true : sprites_to_hgrid[sprite_number][h_segment];
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
#if defined(DEBUG)
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
      digest_sprites_to_chars[sprite_number] = digest_sprites_to_chars[sprite_number] ? true : sprites_to_chars[sprite_number][char_number];
    }
  }

  /*
   * Collisions Chars to Vertical Grid
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
    {
      if (v_segments[v_segment].displayed)
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
      digest_chars_to_vgrid = digest_chars_to_vgrid ? true : chars_to_vgrid[char_number][v_segment];
    }
  }

  /*
   * Collisions Chars to Horizontal Grid
   */

  for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
  {
    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
    {
      if (h_segments[h_segment].displayed)
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
      if (grid_dots)
        if (
            (((displayed_chars[char_number].start_x >= dots[h_segment].start_x) &&
              (displayed_chars[char_number].start_x <= dots[h_segment].start_x + 15)) ||
             ((displayed_chars[char_number].start_x + 7 >= dots[h_segment].start_x) &&
              (displayed_chars[char_number].start_x + 7 <= dots[h_segment].start_x + 15))) &&
            (((displayed_chars[char_number].start_y >= dots[h_segment].start_y) &&
              (displayed_chars[char_number].start_y <= dots[h_segment].start_y + 2)) ||
             ((displayed_chars[char_number].end_y >= dots[h_segment].start_y) &&
              (displayed_chars[char_number].end_y <= dots[h_segment].start_y + 2))))
        {
          chars_to_hgrid[char_number][h_segment] = true;
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
      digest_chars_to_hgrid = digest_chars_to_hgrid ? true : chars_to_hgrid[char_number][h_segment];
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
#if defined(DEBUG)

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
  }

#endif

  return result;
}

/*
 *
 * draw_display()
 *
 */

void draw_display()
{
#if defined(DEBUG)
  Serial.println("draw_display()");
#endif

  if (!background_uptodate)
  {
    background_uptodate = true;
    tft.fillScreen(background_color);
  }

  if (grid_control && !grid_uptodate)
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
 * init_intel8245()
 */

void init_intel8245()
{
#if defined(DEBUG)
  Serial.println("init_intel8245()");
#endif
  for (uint8_t i = 0x00; i < 0xFF; i++)
    intel8245_ram[i] = 0x00;
  init_grid_elements();
  init_displayed_chars();
  init_displayed_sprites();
}
