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

uint8_t intel8245_ram[256];   // TODO Je peux réduire ceci à 132 octets: 0x00 - 0X7F et 0xA0 à 0xA3
uint8_t collision_table[256]; // Va falloir trouver un moyen de remplacer ce tableau ENORME

void draw_grid()
{
  uint8_t mask; // Masque utilisé pour le décodage des bits de chaque octet
  uint8_t data; // Octets de la RAM vidéo
  int x;
  int width;
  uint8_t color;

  /*
    if (intel8245_ram[0xA0] & 0x40)
    { // Bit 6 de 0xA0 controle l'affichage des points de la grille
      for (int j = 0; j < 9; j++)
      {                                          // j : balayage par lignes de 0 à 8
        line_pnt = (j * 24 + 24) * BITMAP_WIDTH; // Les lignes sont séparées de 24 pixels. line_pnt pointe sur le début de la ligne concernée.
        for (int i = 0; i < 9; i++)
        {                                     // i : balayage par colonne de 0 à 8
          dot_pnt = line_pnt + (i * 32) + 20; // dot_pnt pointe sur le point ciblé. Les points sont séparés de 32 pixels.
          bmp[dot_pnt] = 1;
          // Points de 4*3 ?
        }
      }
    }

    // TODO optimisation future: aller de 1 à 9 (au lieu de 0 à 8) et éviter ainsi ((j*24)+24)*WIDTH pour avoir à la place j*24*WIDTH
    //      optimisation future: je pense que deux boucles distinctes iraient mieux
    //
    mask = 0x01; // Tracé des 9 lignes horizontales.
    for (uint8_t j = 0; j < 9; j++)
    { // j : balayage des lignes 0 à 8
      line_pnt = (j * 24 + 24) * BITMAP_WIDTH;
      for (uint8_t i = 0; i < 9; i++)
      { // i : balayage par colonnes de 0 à 8
        dot_pnt = line_pnt + (i * 32) + 20;
        data = intel8245_ram[0xC0 + i]; // 0xC0 - 0xC8 = Lignes horizontales, chaque octet représente une colonne
        if (j == 8)
        {
          data = intel8245_ram[0xD0 + i]; // 0xD0 - 0xD8 = 9ième ligne horizontale, seul le bit 0 est utile
          mask = 1;
        }
        if (data & mask)
          for (uint8_t k = 0; k < 32; k++)
            bmp[dot_pnt + k] = 1; // 32 ou 36 de large ?
      }
      mask <<= 1;
    }

    mask = 0x01; // Tracé des 10 lignes verticales
    width = 4;
    if (intel8245_ram[0xA0] & 0x80)
      width = 32; // Bit 7 de 0xA0 contrôle la largeur des lignes verticales
    for (int j = 0; j < 10; j++)
    { // Cette fois il semble qu'on balaye par colonne d'abord
      line_pnt = (j * 32);
      mask = 0x01;
      data = intel8245_ram[0xE0 + j]; // 0xE0 - 0xE9 = Lignes verticales, chaque octet représente une ligne
      for (x = 0; x < 8; x++)
      { // x serait un numéro de ligne !!! Va falloir réorganiser tout ça !!!
        dot_pnt = line_pnt + (((x * 24) + 24) * BITMAP_WIDTH) + 20;
        if (data & mask)
        {
          for (uint8_t i = 0; i < 24; i++)
          {
          }
        }
        mask <<= 1;
      }
    }
  */
}

/*
 * Chars
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
  for (uint8_t char_line = 0; char_line < 8; char_line++)
  {
    cset_byte = CSET(cset_start_address + char_line);
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
    Serial.print("cset_byte[0x");
    Serial.print(char_line, HEX);
    Serial.print("] = 0x");
    Serial.println(cset_byte, HEX);
#endif

    for (int8_t char_col = 7; char_col >= 0; char_col--)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
      Serial.print("char_col = ");
      Serial.println(char_col, HEX);
#endif
      if ((cset_byte >> char_col) & 0x01)
      {
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
        Serial.print("drawPixel(");
        Serial.print(x + (7 - char_col), HEX);
        Serial.print(", ");
        Serial.print(y + char_line, HEX);
        Serial.print(", ");
        Serial.print(color);
        Serial.println(")");
#endif
        graphic_tft.drawPixel(
            (uint16_t)(x + (7 - char_col)),
            (uint16_t)(y + char_line),
            colors[color]);
      }
    }
  }
}

/*
 * Chars
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

void show_4sprites()
{
#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.println("show_4sprites()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif
}

void clear_collision()
{
#if defined(DEBUG_STDERR) && defined(DEBUG_CHARS)
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_CHARS)
  Serial.print(bigben);
  Serial.println(" - clear_collision()");
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_CHARS)
#endif
  collision_table[0x01] = 0;
  collision_table[0x02] = 0;
  collision_table[0x04] = 0;
  collision_table[0x08] = 0;
  collision_table[0x10] = 0;
  collision_table[0x20] = 0;
  collision_table[0x40] = 0;
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
  if (intel8245_ram[0xA0] & 0x08)
    draw_grid();
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
  Serial.println("init_intel8245()");
#endif
#ifdef DEBUG_TFT
#endif
  for (uint8_t i = 0x00; i < 0xFF; i++)
    intel8245_ram[i] = 0x00;
}
