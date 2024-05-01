#include <Arduino.h>

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "arduipac.h"
#include "arduipac_8245.h"
#include "arduipac_cset.h"
#include "arduipac_colors.h"
#include "arduipac_collisions.h"

// #define DEBUG
// #define DEBUG_GRID
// #define DEBUG_DETAIL

//
// Storage
//

bool grid_control = 0;
bool foreground_control = 0;
bool grid_dots = 0;
uint8_t v_segments_width = 2; // This one REALLY needs to be initialized
uint16_t grid_color = 0;
uint16_t background_color = 0;

bool background_uptodate = 0;
bool sprites_uptodate = 0;
bool chars_uptodate = 0;

// stockage des segments

h_segment_t h_segments[NB_H_SEGMENTS];
dot_t dots[NB_DOTS];
v_segment_t v_segments[NB_V_SEGMENTS];

// stockage des caractères

displayed_char_t displayed_chars[NB_CHARS];

// stockage des sprites

displayed_sprite_t displayed_sprites[NB_SPRITES];


//
// Initialisations
//

void init_grid_elements()
{
    //
    // Coordonnées des 9 rows horizontales et des dots
    //
    for (uint8_t column = 0; column < 9; column++)
    {
#if defined(DEBUG)
        Serial.print("init_grid_elements() - segments horizontaux de la column  ");
        Serial.println(column);
#endif
        for (uint8_t row = 0; row < 9; row++)
        {
            h_segments[column * 9 + row].start_x = 10 + 16 * column;
            h_segments[column * 9 + row].start_y = 24 + 24 * row;
            h_segments[column * 9 + row].changed = false;
            h_segments[column * 9 + row].displayed = false;
            dots[column * 9 + row].start_x = 10 + 16 * column;
            dots[column * 9 + row].start_y = 24 + 24 * row;
        }
    }

    v_segments_width = 2;
    // Default to standard width

    //
    // Coordonnées des 10 columns verticales
    //
    for (uint8_t column = 0; column < 10; column++)
    {
#if defined(DEBUG)
        Serial.print("init_grid_elements() - segments verticaux de la column ");
        Serial.println(column);
#endif
        for (uint8_t row = 0; row < 8; row++)
        {
            v_segments[column * 8 + row].start_x = 10 + 16 * column;
            v_segments[column * 8 + row].end_x = v_segments[column * 8 + row].start_x + v_segments_width - 1;
            v_segments[column * 8 + row].start_y = 24 + 24 * row;
            v_segments[column * 8 + row].changed = false;
            v_segments[column * 8 + row].displayed = false;
        }
    }

#if defined(DEBUG)

    for (int i = 0; i < NB_H_SEGMENTS; i++)
    {
        Serial.print("h_segments[");
        Serial.print(i);
        Serial.print("].start_x = ");
        Serial.print(h_segments[i].start_x);
        Serial.print(", start_y = ");
        Serial.print(h_segments[i].start_y);
        Serial.print(", displayed = ");
        Serial.println(h_segments[i].displayed);
    }

    for (int i = 0; i < NB_V_SEGMENTS; i++)
    {
        Serial.print("v_segments[");
        Serial.print(i);
        Serial.print("].start_x = ");
        Serial.print(v_segments[i].start_x);
        Serial.print(", end_x = ");
        Serial.print(v_segments[i].end_x);
        Serial.print(", start_y = ");
        Serial.print(v_segments[i].start_y);
        Serial.print(", displayed = ");
        Serial.println(v_segments[i].displayed);
    }
#endif
}

void init_displayed_chars()
{
    for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
    {
        displayed_chars[char_number].start_x = 0;
        displayed_chars[char_number].start_y = 0;
        displayed_chars[char_number].height = 0;
        displayed_chars[char_number].end_y = 0;
        displayed_chars[char_number].cset_start_address = 0;
        displayed_chars[char_number].color = 0;
        displayed_chars[char_number].previous_start_x = 0;
        displayed_chars[char_number].previous_start_y = 0;
        displayed_chars[char_number].previous_height = 0;
        displayed_chars[char_number].changed = false;
        displayed_chars[char_number].displayed = false;
    }
}

void init_displayed_sprites()
{
    for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
    {
        displayed_sprites[sprite_number].start_x = 0;
        displayed_sprites[sprite_number].start_y = 0;
        displayed_sprites[sprite_number].size = 1;
        displayed_sprites[sprite_number].shift = 0;
        displayed_sprites[sprite_number].even_shift = 0;
        displayed_sprites[sprite_number].end_x = 0;
        displayed_sprites[sprite_number].end_y = 0;
        displayed_sprites[sprite_number].color = 0;
        displayed_sprites[sprite_number].previous_start_x = 0;
        displayed_sprites[sprite_number].previous_start_y = 0;
        displayed_sprites[sprite_number].previous_size = 0;
        displayed_sprites[sprite_number].changed = false;
        displayed_sprites[sprite_number].displayed = false;
    }
}
