#include <Arduino.h>

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "arduipac.h"
#include "arduipac_8245.h"
#include "arduipac_cset.h"
#include "arduipac_colors.h"
#include "arduipac_config.h"
#include "arduipac_collisions.h"

// #define DEBUG_SERIAL
// #define DEBUG_GRID
// #define DEBUG_DETAIL

//
// Storage
//

uint8_t grid_control = 0;
uint8_t foreground_control = 0;
uint8_t grid_dots = 0;
uint8_t v_segments_width = 3; // This one REALLY needs to be initialized
uint16_t grid_color = 0;
uint16_t background_color = 0;

uint8_t h_grid_uptodate = 0;
uint8_t v_grid_uptodate = 0;
uint8_t sprites_uptodate = 0;
uint8_t chars_uptodate = 0;

// stockage des segments

h_segment_t h_segments[NB_H_SEGMENTS];
v_segment_t v_segments[NB_V_SEGMENTS];

// stockage des caractères

displayed_char_t displayed_chars[NB_CHARS];

// stockage des sprites

displayed_sprite_t displayed_sprites[NB_SPRITES];

//
// Display elements changing
//

//
// Initialisations
//

void init_grid_elements()
{
    //
    // Coordonnées des 9 lignes horizontales
    //
    for (uint8_t colonne = 0; colonne < 9; colonne++)
    {
#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
        Serial.print("init_grid_elements() - segments horizontaux de la colonne  ");
        Serial.println(colonne);
#endif
        for (uint8_t ligne = 0; ligne < 9; ligne++)
        {
            h_segments[colonne * 9 + ligne].start_x = 10 + 16 * colonne;
            h_segments[colonne * 9 + ligne].start_y = 24 + 24 * ligne;
            h_segments[colonne * 9 + ligne].changed_displayed = 0;
        }
    }

    v_segments_width = 2;
    // Default to standard width

    //
    // Coordonnées des 10 colonnes verticales
    //
    for (uint8_t colonne = 0; colonne < 10; colonne++)
    {

#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)
        Serial.print("init_grid_elements() - segments verticaux de la colonne ");
        Serial.println(colonne);
#endif
        for (uint8_t ligne = 0; ligne < 8; ligne++)
        {
            v_segments[colonne * 8 + ligne].start_x = 10 + 16 * colonne;
            v_segments[colonne * 8 + ligne].end_x = 10 + 16 * colonne + v_segments_width;
            v_segments[colonne * 8 + ligne].start_y = 24 + 24 * ligne;
            v_segments[colonne * 8 + ligne].changed_displayed = 0;
         }
    }

#if defined(DEBUG_SERIAL) && defined(DEBUG_GRID) && defined(DEBUG_DETAIL)

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
        displayed_chars[char_number].end_y = 0;
        displayed_chars[char_number].previous_start_x = 0;
        displayed_chars[char_number].previous_start_y = 0;
        displayed_chars[char_number].previous_end_y = 0;
        displayed_chars[char_number].color = 0;
        displayed_chars[char_number].cset_offset = 0;
        displayed_chars[char_number].height = 0;
        displayed_chars[char_number].changed_displayed = 0;
    }
}

void init_displayed_sprites()
{
    for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
    {
        displayed_sprites[sprite_number].start_x = 0;
        displayed_sprites[sprite_number].end_x = 0;
        displayed_sprites[sprite_number].start_y = 0;
        displayed_sprites[sprite_number].end_y = 0;
        displayed_sprites[sprite_number].previous_start_x = 0;
        displayed_sprites[sprite_number].previous_end_x = 0;
        displayed_sprites[sprite_number].previous_start_y = 0;
        displayed_sprites[sprite_number].previous_end_y = 0;
        displayed_sprites[sprite_number].color = 0;
        displayed_sprites[sprite_number].size = 1;
        displayed_sprites[sprite_number].shift = 0;
        displayed_sprites[sprite_number].even_shift = 0;
        displayed_sprites[sprite_number].changed_displayed = 0;
    }
}

/*
        //
        // Détection des collisions
        //

        for (uint8_t h_segment_number = 0; h_segment_number < number_h_segments;
             h_segment_number++)
    {
        if (
            (
                (displayed_sprites[sprite_number].start_x > displayed_h_segments[h_segment_number].start_x &&
                 displayed_sprites[sprite_number].start_x < displayed_h_segments[h_segment_number].end_x) ||
                (displayed_sprites[sprite_number].end_x > displayed_h_segments[h_segment_number].start_x &&
                 displayed_sprites[sprite_number].end_x < displayed_h_segments[h_segment_number].end_x)) &&
            ((displayed_sprites[sprite_number].start_y > displayed_h_segments[h_segment_number].start_y &&
              displayed_sprites[sprite_number].start_y < displayed_h_segments[h_segment_number].end_y) ||
             (displayed_sprites[sprite_number].end_y > displayed_h_segments[h_segment_number].start_y &&
              displayed_sprites[sprite_number].end_y < displayed_h_segments[h_segment_number].end_y)))
        {
            // il y a collision…
        }
    }

    for (uint8_t v_segment_number = 0; v_segment_number < number_v_segments; v_segment_number++)
    {
        if (
            (
                (displayed_sprites[sprite_number].start_x > displayed_v_segments[v_segment_number].start_x &&
                 displayed_sprites[sprite_number].start_x < displayed_v_segments[v_segment_number].end_x) ||
                (displayed_sprites[sprite_number].end_x > displayed_v_segments[v_segment_number].start_x &&
                 displayed_sprites[sprite_number].end_x < displayed_v_segments[v_segment_number].end_x)) &&
            ((displayed_sprites[sprite_number].start_y > displayed_v_segments[v_segment_number].start_y &&
              displayed_sprites[sprite_number].start_y < displayed_v_segments[v_segment_number].end_y) ||
             (displayed_sprites[sprite_number].end_y > displayed_v_segments[v_segment_number].start_y &&
              displayed_sprites[sprite_number].end_y < displayed_v_segments[v_segment_number].end_y)))
        {
            // il y a collision…
        }
    }

    for (uint8_t char_number = 0; char_number < 28; char_number++)
    {
        {
            if (
                (
                    (displayed_sprites[sprite_number].start_x > displayed_h_segments[h_segment_number].start_x &&
                     displayed_sprites[sprite_number].start_x < displayed_h_segments[h_segment_number].end_x) ||
                    (displayed_sprites[sprite_number].end_x > displayed_h_segments[h_segment_number].start_x &&
                     displayed_sprites[sprite_number].end_x < displayed_h_segments[h_segment_number].end_x)) &&
                ((displayed_sprites[sprite_number].start_y > displayed_h_segments[h_segment_number].start_y &&
                  displayed_sprites[sprite_number].start_y < displayed_h_segments[h_segment_number].end_y) ||
                 (displayed_sprites[sprite_number].end_y > displayed_h_segments[h_segment_number].start_y &&
                  displayed_sprites[sprite_number].end_y < displayed_h_segments[h_segment_number].end_y)))
            {
                // il y a collision…
            }
        }

        for (uint8_t col_sprite_number = 0; col_sprite_number < 4; col_sprite_number++)
        {
            if (sprite_number != col_sprite_number)
            {
                {
                    if (
                        (
                            (displayed_sprites[sprite_number].start_x > displayed_h_segments[h_segment_number].start_x &&
                             displayed_sprites[sprite_number].start_x < displayed_h_segments[h_segment_number].end_x) ||
                            (displayed_sprites[sprite_number].end_x > displayed_h_segments[h_segment_number].start_x &&
                             displayed_sprites[sprite_number].end_x < displayed_h_segments[h_segment_number].end_x)) &&
                        ((displayed_sprites[sprite_number].start_y > displayed_h_segments[h_segment_number].start_y &&
                          displayed_sprites[sprite_number].start_y < displayed_h_segments[h_segment_number].end_y) ||
                         (displayed_sprites[sprite_number].end_y > displayed_h_segments[h_segment_number].start_y &&
                          displayed_sprites[sprite_number].end_y < displayed_h_segments[h_segment_number].end_y)))
                    {
                        // il y a collision…
                    }
                }
            }
        }
    }

//
// Etapes:
//
//
// 3 - gestion des collisions
// 3a. collisions sprites / grille
// 3b. collisions sprites / caractères
// 3c. collisions sprites / sprites
//
// 4 - rendu-compte des collisions
//
// Questions en suspens:
// ? quid des points de la grille ?

*/
