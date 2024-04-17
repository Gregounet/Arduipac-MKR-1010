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

#define COLLISION_SP0 0x01
#define COLLISION_SP1 0x02
#define COLLISION_SP2 0x04
#define COLLISION_SP3 0x08
#define COLLISION_VGRID 0x10
#define COLLISION_HGRID 0x20
#define COLLISION_CHAR 0x80

#undef DEBUG_STDERR
#define DEBUG_SERIAL
#undef DEBUG_TFT

#define DEBUG_CHARS
#define DEBUG_GRID
#define DEBUG_SPRITES

#undef DEBUG_DETAIL

uint8_t intel8245_ram[256];
uint8_t collision_table[256]; // TODO Va falloir trouver un moyen de remplacer ce tableau ENORME

/*
 * draw_grid()
 *
 */

void draw_grid()
{
  uint8_t mask;  // Masque utilisé pour le décodage des bits de chaque octet
  uint8_t data;  // Octet provenant de la RAM VDC
  uint8_t width; // Largeur d'une colonne

#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
  Serial.print(bigben);
  Serial.println(" - draw_grid()");
#endif

  uint8_t grid_color_index;
  uint16_t grid_color;

  grid_color_index = intel8245_ram[0xA3] & 0x07;
  if (intel8245_ram[0xA3] & 0x40)
    grid_color = LIGHT_COLORS(grid_color_index);
  else
    grid_color = DARK_COLORS(grid_color_index);

  // Bit 6 de 0xA0 controle l'affichage des points de la grille
  if (intel8245_ram[0xA0] & 0x40)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.println("draw_grid() - affichage des points");
#endif
    // j : balayage par lignes de 0 à 8
    for (uint8_t j = 0; j < 9; j++)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
      Serial.print("draw_grid() - affichage des points de la ligne ");
      Serial.println(j);
#endif
      // i : balayage par colonne de 0 à 8
      for (uint8_t i = 0; i < 9; i++)
      {
        // Les lignes sont séparées de 24 pixels
        // Les colonnes sont séparés de 32 pixels.
        graphic_tft.fillRect(20 + 32 * i, 24 + 24 * j, 4, 3, grid_color);
      }
    }
  }

  // TODO optimisation future: aller de 1 à 9 (au lieu de 0 à 8) et éviter ainsi ((j*24)+24)*WIDTH pour avoir à la place j*24*WIDTH
  //      optimisation future: je pense que deux boucles distinctes iraient mieux
  //

  // Tracé des 9 lignes
  // j : balayage des lignes 0 à 8
  for (uint8_t j = 0; j < 9; j++)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.print("draw_grid() - affichage ligne ");
    Serial.println(j);
#endif
    mask = 0x01;
    // i : balayage par colonnes de 0 à 8
    for (uint8_t i = 0; i < 9; i++)
    {
      // 0xC0 - 0xC8 = Lignes, chaque octet représente une colonne
      data = intel8245_ram[0xC0 + i];
      // 0xD0 - 0xD8 = 9ième ligne, seul le bit 0 est utilisé
      if (j == 8)
      {
        data = intel8245_ram[0xD0 + i];
        mask = 0x01;
      }
      if (data & mask)
        graphic_tft.fillRect(20 + 32 * i, 24 + 24 * j, 36, 3, grid_color);
      mask <<= 1;
    }
  }

  // Bit 7 de 0xA0 contrôle la largeur des colonnes
  width = (intel8245_ram[0xA0] & 0x80) ? 32 : 4;
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
  Serial.print("draw_grid() - largeur des colonnes : ");
  Serial.println(width);
#endif

  // Tracé des 10 colonnes verticales
  for (uint8_t x = 0; x < 10; x++)
  {
    // 0xE0 - 0xE9 = Colonnes, chaque octet représente une colonne et chaque bit un segment
    data = intel8245_ram[0xE0 + x];
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
    Serial.print("draw_grid() - affichage de la colonne ");
    Serial.print(x);
    Serial.print(", octet == ");
    Serial.println(data);
#endif
    mask = 0x01;
    for (uint8_t y = 0; y < 8; y++)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
      Serial.print("mask == ");
      Serial.println(mask);
#endif
      if (data & mask)
      {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
        Serial.print("draw_grid() - affichage d'un segment ");
        Serial.println(x);
#endif
        graphic_tft.fillRect(20 + 32 * x, 24 + 24 * y, width, 24, grid_color);
      }
      mask <<= 1;
    }
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
  uint16_t char_color = CHAR_COLORS((char_color_index & 0x07) >> 1);
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
  Serial.println("show_12chars()");
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

#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES) && defined(DEBUG_DETAIL)
#endif
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

    for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
    {
      sprite_data = intel8245_ram[sprite_pattern + sprite_row];
#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES) && defined(DEBUG_DETAIL)
#endif
      if (sprite_number == 0 || sprite_number == 1)
      {
        /*
        Serial.print("show_4sprites() - affichage de la ligne ");
        Serial.print(sprite_row);
        Serial.print(" - data : 0x");
        Serial.println(sprite_data, HEX);
        */
      }

      uint8_t mask = 0x01;

      for (uint8_t sprite_column = 0; sprite_column < 8; sprite_column++)
      {
        // if (sprite_number == 0 || sprite_number == 1)
        {
          if (sprite_data & mask)
          {
            graphic_tft.fillRect(20 + sprite_x + sprite_column * 2, sprite_y + sprite_row * 2, 2, 2, ST77XX_BLACK);
            Serial.print("I");
          }
          else
          {
            graphic_tft.fillRect(20 + sprite_x + sprite_column * 2, sprite_y + sprite_row * 2, 2, 2, ST77XX_WHITE);
            Serial.print(" ");
          }
          mask <<= 1;
        }
      }
      // if (sprite_number == 0 || sprite_number == 1)
      {
        Serial.println();
      }
    }
    sprite_control += 0x04;
    sprite_pattern += 0x08;
  }
  // delay(2000);
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
  Serial.print(bigben);
  Serial.println(" - clear_collision()");
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
  Serial.print(bigben);
  Serial.println(" - draw_display()");
  Serial.print("Registre 0xA0 == 0x");
  Serial.println(intel8245_ram[0xA0], HEX);
#endif
#ifdef DEBUG_TFT
#endif

  uint8_t bg_color_index;
  uint16_t bg_color;

  bg_color_index = (intel8245_ram[0xA3] & 0x38) >> 3;
  if (intel8245_ram[0xA3] & 0x40)
    bg_color = LIGHT_COLORS(bg_color_index);
  else
    bg_color = DARK_COLORS(bg_color_index);

  Serial.print("bg_color_index == ");
  Serial.println(bg_color_index);

  graphic_tft.fillScreen(bg_color);

  // bit 3 == enable grid
  if (intel8245_ram[0xA0] & 0x08)
    draw_grid();

  // bit 5 = enable display
  if (intel8245_ram[0xA0] & 0x20)
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
  Serial.print(bigben);
  Serial.println(" - init_intel8245()");
#endif
#ifdef DEBUG_TFT
#endif
  for (uint8_t i = 0x00; i < 0xFF; i++)
    intel8245_ram[i] = 0x00;
}
