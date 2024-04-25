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
    uint8_t changed_displayed;
};

struct v_segment_t
{ // fixed height, variable width
    uint8_t start_x;
    uint8_t end_x;
    uint8_t start_y;
    uint8_t changed_displayed;
};

struct displayed_char_t
{ // variable height, fixed width
 
    uint8_t start_x;
    uint8_t start_y;
    uint8_t end_y;
    uint8_t height;
    uint16_t cset_offset;
    uint16_t color;
    uint8_t previous_start_x;
    uint8_t previous_start_y;
    uint8_t previous_height;
    uint8_t changed_displayed;
};

struct displayed_sprite_t
{ // variable height and width
    uint8_t start_x;
    uint8_t end_x;
    uint8_t start_y;
    uint8_t end_y;
    uint8_t size; // 1 for normal sized ones, 2 for double-sized ones
    uint8_t shift;
    uint8_t even_shift;
    uint16_t color;
    uint8_t previous_start_x;
    uint8_t previous_start_y;
    uint8_t previous_size;
    uint8_t changed_displayed;
};

//
// Constantes
//

#define NB_H_SEGMENTS 81
#define NB_V_SEGMENTS 80
#define NB_CHARS 28
#define NB_SPRITES 4

//
// Storage
//

extern uint8_t grid_control;
extern uint8_t foreground_control;
extern uint8_t grid_dots;
extern uint8_t v_segments_width; // This one REALLY needs to be initialized
extern uint16_t grid_color;
extern uint16_t background_color;

extern uint8_t background_uptodate;
extern uint8_t h_grid_uptodate;
extern uint8_t v_grid_uptodate;
extern uint8_t sprites_uptodate;
extern uint8_t chars_uptodate;


// stockage des segments

extern h_segment_t h_segments[NB_H_SEGMENTS];
extern v_segment_t v_segments[NB_V_SEGMENTS];

// stockage des caractères

extern displayed_char_t displayed_chars[NB_CHARS];

// stockage des sprites

extern displayed_sprite_t displayed_sprites[NB_SPRITES];

// Functions

void init_grid_elements();
void init_displayed_chars();
void init_displayed_sprites();

#endif /* (ARDUIPAC_COLLISIONS_H) */
