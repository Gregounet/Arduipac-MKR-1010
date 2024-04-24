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

#define DEBUG_SERIAL
#define DEBUG_GRID
#define DEBUG_DETAIL

//
// Storage
//

// stockage des segments

// uint8_t number_h_segments;
// uint8_t number_v_segments;
uint8_t v_segments_width;
uint16_t segments_color;

h_segment_t h_segments[NB_H_SEGMENTS];
v_segment_t v_segments[NB_V_SEGMENTS];

// stockage des caractères

displayed_char_t displayed_chars[28];

// stockage des sprites

displayed_sprite_t displayed_sprites[4];

//
// Display elements changing
//

//
// Initialisation de la grille
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
            h_segments[colonne * 9 + ligne].displayed = 0;
            // graphic_tft.fillRect(20 + 32 * colonne, 24 + 24 * ligne, 36, 3, grid_color);
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
            v_segments[colonne * 8 + ligne].displayed = 0;
            // graphic_tft.fillRect(20 + 32 * colonne, 24 + 24 * ligne, width, 24, grid_color);
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

/*
// Modification des caractères

Pour chaque élément caractère:

{
    displayed_chars[...].start_x
        displayed_chars[...]
            .start_y
                displayed_chars[...]
            .end_y
                displayed_chars[...]
            .color
}

// Modification des sprites

for (uint8_t sprite_number = 0; sprite_number < 4; sprite_number++)
{
    displayed_sprites[sprite_number].size = …;
    displayed_sprites[sprite_number].start_x = …;
    displayed_sprites[sprite_number].end_x = start_x + (size * 8);
    displayed_sprites[sprite_number].start_y = …;
    displayed_sprites[sprite_number].end_y = start_x + (size * 8);
    displayed_sprites[sprite_number].color = … ;
    displayed_sprites[sprite_number].shift = …;
    displayed_sprites[sprite_number].even_shift = …;
}

//
// Affichages
//

for (uint8_t h_segment_number = 0; h_segment_number < number_h_segments; h_segment_number++)
    graphic_tft.fillRect(
        displayed_h_segments[h_segment_number].start_x,
        displayed_h_segments[h_segment_number].start_y,
…,
        3,
        segments_color);

for (uint8_t v_segment_number = 0; v_segment_number < number_v_segments; v_segment_number++)
    graphic_tft.fillRect(
        displayed_v_segments[v_segment_number].start_x,
        displayed_v_segments[v_segment_number].start_y,
        v_segments_width,
… segments_color);

for (uint8_t char_number = 0; char_number < 28; char_number++)
{
<reprendre le code existant>
}

for (uint8_t sprite_number = 0; sprite_number < 4; sprite_number++)
{
    <reprendre le code existant>

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
// 1 - ajout des stockages de données lors des modifications
// 1a. Création des structures de stockage (dans 8245.c)
// 1b. Initialisation des structures - vraiment nécessaire ?
// 1c. Mise à jour des données qd modif
//
// 2 - utilisation de ces structures pour gérer les affichages
// 2a. grille
// 2b. caractères
// 2c. sprites
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
// ? gestion séparée quad caractères ?

*/
