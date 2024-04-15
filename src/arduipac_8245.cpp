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

#undef DEBUG_CHARS
#define DEBUG_GRID
#define DEBUG_SPRITES

uint8_t intel8245_ram[256];
uint8_t collision_table[256]; // TODO Va falloir trouver un moyen de remplacer ce tableau ENORME

void draw_grid()
{
  uint8_t mask;  // Masque utilisé pour le décodage des bits de chaque octet
  uint8_t data;  // Octet provenant de la RAM VDC
  uint8_t width; // Largeur d'une colonne
  // uint8_t color;

#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
  Serial.print(bigben);
  Serial.println(" - draw_grid()");
#endif

  // Bit 6 de 0xA0 controle l'affichage des points de la grille
  if (intel8245_ram[0xA0] & 0x40)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
    Serial.println("draw_grid() - affichage des points");
#endif
    // j : balayage par lignes de 0 à 8
    for (uint8_t j = 0; j < 9; j++)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
      Serial.print("draw_grid() - affichage des points de la ligne ");
      Serial.println(j);
#endif
      // i : balayage par colonne de 0 à 8
      for (uint8_t i = 0; i < 9; i++)
      {
        // Les lignes sont séparées de 24 pixels
        // Les colonnes sont séparés de 32 pixels.
        for (uint8_t k = 0; k < 4; k++)
        {
          graphic_tft.drawPixel(20 + 32 * i + k, 24 + 24 * j + 0, ST77XX_RED);
          graphic_tft.drawPixel(20 + 32 * i + k, 24 + 24 * j + 1, ST77XX_RED);
          graphic_tft.drawPixel(20 + 32 * i + k, 24 + 24 * j + 2, ST77XX_RED);
        }
      }
    }
  }

  // TODO optimisation future: aller de 1 à 9 (au lieu de 0 à 8) et éviter ainsi ((j*24)+24)*WIDTH pour avoir à la place j*24*WIDTH
  //      optimisation future: je pense que deux boucles distinctes iraient mieux
  //

  // Tracé des 9 lignes
  mask = 0x01;
  // j : balayage des lignes 0 à 8
  for (uint8_t j = 0; j < 9; j++)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
    Serial.print("draw_grid() - affichage ligne ");
    Serial.println(j);
#endif
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
        // 32 ou 36 de large ? Un seul pixel de haut ?
        for (uint8_t k = 0; k < 32; k++)
          graphic_tft.drawPixel(20 + 32 * i + k, 24 + 24 * j, ST77XX_RED);
    }
    mask <<= 1;
  }

  // Bit 7 de 0xA0 contrôle la largeur des colonnes
  width = (intel8245_ram[0xA0] & 0x80) ? 32 : 4;
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
  Serial.print("draw_grid() - largeur des colonnes : ");
  Serial.println(width);
#endif

  // Tracé des 10 colonnes verticales
  mask = 0x01;
  for (uint8_t j = 0; j < 10; j++)
  {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID)
    Serial.print("draw_grid() - affichage de la colonne ");
    Serial.println(j);
#endif
    mask = 0x01;
    // 0xE0 - 0xE9 = Colonnes, chaque octet représente une colonne
    data = intel8245_ram[0xE0 + j];
    for (uint8_t i = 0; i < 8; i++)
      if (data & mask)
      {
        for (uint8_t x = 0; x < width; x++)
        {
          for (uint8_t y = 0; y < 24; y++)
            graphic_tft.drawPixel(20 + 32 * i + x, 24 + 24 * j + y, ST77XX_RED);
        }
      }
    mask <<= 1;
  }
}

/*
 * Major system
 *
 */

void show_12chars()
{
  uint8_t char_x;
  uint8_t char_y;
  uint16_t char_offset;
  uint8_t char_color;

#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.println("show_12chars()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif

  for (uint8_t i = 0; i < 12; i++)
  {
    char_x = intel8245_ram[0x10 + (i * 0x04) + 0x01];
    char_y = intel8245_ram[0x10 + (i * 0x04) + 0x00];
    char_offset = (((uint16_t)(intel8245_ram[0x10 + (i * 0x04) + 0x03]) & 0x01) << 8) | (intel8245_ram[0x10 + (i * 0x04) + 0x02]);
    char_color = (intel8245_ram[0x10 + (i * 0x04) + 0x03] & 0x0E) >> 1;
    show_1char(char_x, char_y, char_offset, char_color);
  }
}

void show_1char(uint8_t x, uint8_t y, uint16_t offset, uint8_t color)
{
  int16_t cset_start_address = offset + (y >> 1);
  if (cset_start_address > 512)
    cset_start_address -= 512;

  uint8_t cset_byte;

#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
  fprintf(stderr, "show1_char(): X = %d, Y = %d, indice dans cst = %0x03x, couleur = %d\n", x, y, cset_start_offset, color);
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.print("show1_char() x = 0x");
  Serial.print(x, HEX);
  Serial.print(", y = 0x");
  Serial.print(y, HEX);
  Serial.print(", offset = 0x");
  Serial.print(offset, HEX);
  Serial.print(", indice dans cset = 0x");
  Serial.print(cset_start_address, HEX);
  Serial.print(", char number = 0x");
  Serial.print(cset_start_address / 8, HEX);
  Serial.print(", color = 0x");
  Serial.println(color, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif
  // for (uint8_t char_line = 0; (cset_byte = cset[char_line]) != 0x00; char_line++)
  for (uint8_t char_row = 0; char_row < 8; char_row++)
  {
    cset_byte = CSET(cset_start_address + char_row);
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
    Serial.print("cset_byte[0x");
    Serial.print(char_row, HEX);
    Serial.print("] = 0x");
    Serial.println(cset_byte, HEX);
#endif

    for (int8_t char_column = 7; char_column >= 0; char_column--)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
      Serial.print("char_column = ");
      Serial.println(char_column, HEX);
#endif
      if ((cset_byte >> char_column) & 0x01)
      {
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
        Serial.print("drawPixel(");
        Serial.print(x + (7 - char_column), HEX);
        Serial.print(", ");
        Serial.print(y + char_row, HEX);
        Serial.print(", ");
        Serial.print(color);
        Serial.println(")");
#endif
        graphic_tft.drawPixel(x + (7 - char_column), y + char_row, COLORS(color));
      }
    }
  }
}

/*
 * Quads
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
  for (uint8_t i = 0x40; i < 0x80; i += 0x10)
    show_1quad(i);
}

void show_1quad(uint8_t quad_indx)
{
  uint8_t x;
  uint8_t y;
  uint16_t offset;
  uint8_t color;

#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.println("show_1quad()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif

  x = intel8245_ram[quad_indx + 0x01];
  y = intel8245_ram[quad_indx + 0x00];
  offset = (((uint16_t)(intel8245_ram[quad_indx + 0x03]) & 0x01) << 8) | (intel8245_ram[quad_indx + 0x02]);
  color = (intel8245_ram[quad_indx + 0x03] & 0x0E) >> 1;
  show_1char(x, y, offset, color);

  x = intel8245_ram[quad_indx + 0x05];
  y = intel8245_ram[quad_indx + 0x04];
  offset = (((uint16_t)(intel8245_ram[quad_indx + 0x07]) & 0x01) << 8) | (intel8245_ram[quad_indx + 0x01]);
  color = (intel8245_ram[quad_indx + 0x07] & 0x0E) >> 1;
  show_1char(x, y, offset, color);

  x = intel8245_ram[quad_indx + 0x09];
  y = intel8245_ram[quad_indx + 0x08];
  offset = (((uint16_t)(intel8245_ram[quad_indx + 0x0B]) & 0x01) << 8) | (intel8245_ram[quad_indx + 0x0A]);
  color = (intel8245_ram[quad_indx + 0x0B] & 0x0E) >> 1;
  show_1char(x, y, offset, color);

  x = intel8245_ram[quad_indx + 0x0D];
  y = intel8245_ram[quad_indx + 0x0C];
  offset = (((uint16_t)(intel8245_ram[quad_indx + 0x0F]) & 0x01) << 8) | (intel8245_ram[quad_indx + 0x0E]);
  color = (intel8245_ram[quad_indx + 0x0F] & 0x0E) >> 1;
  show_1char(x, y, offset, color);
}

/*
 * Minor system
 *
 */
void show_4sprites()
{
  uint8_t sprite_control = 0x00;
  uint8_t sprite_pattern = 0x80;

  uint8_t sprite_data;
  uint8_t sprite_x;
  uint8_t sprite_y;
  uint8_t sprite_color;

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
    sprite_x = intel8245_ram[sprite_control + 0x00];
    sprite_y = intel8245_ram[sprite_control + 0x01];
    sprite_color = (intel8245_ram[sprite_control + 0x02] & 0x34) >> 3;

#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES)
    Serial.print("show_4sprites() - sprite numéro ");
    Serial.print(sprite_number);
    Serial.print(" , x = ");
    Serial.print(sprite_x);
    Serial.print(" , y = ");
    Serial.print(sprite_y);
    Serial.print(" , color = ");
    Serial.println(sprite_color);
#endif

    for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_SPRITES)
      Serial.print("show_4sprites() - affichage de la ligne ");
      Serial.println(sprite_row);
#endif
      sprite_data = intel8245_ram[sprite_pattern + sprite_row];
      uint8_t mask = 0x80;

      for (uint8_t sprite_column = 0; sprite_column < 8; sprite_column++)
      {
        if (sprite_data & mask)
          graphic_tft.drawPixel(sprite_x + sprite_column, sprite_y + sprite_row, COLORS(sprite_color));
        mask >>= 1;
      }
    }
    sprite_control += 0x04;
    sprite_pattern += 0x08;
  }
}

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

void draw_display()
{
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
  Serial.print(bigben);
  Serial.println(" - draw_display()");
  Serial.print("intel8245_ram[0xA0] = ");
  Serial.println(intel8245_ram[0xA0], HEX);
#endif
#ifdef DEBUG_TFT
#endif

  graphic_tft.fillScreen(ST77XX_BLACK);

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
