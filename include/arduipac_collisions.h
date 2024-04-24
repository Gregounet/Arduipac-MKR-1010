#ifndef ARDUIPAC_COLLISIONS_H
#define ARDUIPAC_COLLISIONS_H

#include <stdint.h>

//
// Structures
//

// struct dot_t
// { // fixed width and height
//     uint8_t start_x;
//     uint8_t start_y;
// };

struct h_segment_t
{ // fixed width and height
    uint8_t start_x;
    uint8_t start_y;
    uint8_t displayed;
} ;

struct v_segment_t
{ // fixed height, variable width
    uint8_t start_x;
    uint8_t end_x;
    uint8_t start_y;
    uint8_t displayed;
} ;

struct displayed_char_t
{ // variable height, fixed width
    // TODO: la hauteur n'est en fait pas fixée

    uint8_t start_x;
    uint8_t start_y;
    uint8_t end_y;
    uint16_t color;
}; 

struct displayed_sprite_t
{ // variable height and width
    uint8_t start_x;
    uint8_t end_x;
    uint8_t start_y;
    uint8_t end_y;
    uint16_t color;
    uint8_t size; // 1 for normal sized ones, 2 for double-sized ones
    uint8_t shift;
    uint8_t even_shift;
};

//
// Constantes
//

#define NB_H_SEGMENTS 81
#define NB_V_SEGMENTS 80

//
// Storage
//

// stockage des segments

// uint8_t number_h_segments;
// uint8_t number_v_segments;

extern uint8_t v_segments_width;
extern uint16_t segments_color;

extern h_segment_t h_segments[NB_H_SEGMENTS];
extern v_segment_t v_segments[NB_V_SEGMENTS];

// stockage des caractères

extern displayed_char_t displayed_chars[28];

// stockage des sprites

extern displayed_sprite_t displayed_sprites[4];


// Functions

void init_grid_elements();

#endif /* (ARDUIPAC_COLLISIONS_H) */
