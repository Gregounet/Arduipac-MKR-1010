#ifndef TRAVOLTA_DISPLAY_H
#define TRAVOLTA_DISPLAY_H

#include <stdint.h>

//
// Structures
//

struct dot_t
{ // fixed width and height
    uint8_t start_x;
    uint8_t start_y;
};

struct h_segment_t
{ // fixed width and height
    uint8_t start_x;
    uint8_t start_y;
    bool changed;
    bool displayed;
};

struct v_segment_t
{ // fixed height, variable width
    uint8_t start_x;
    uint8_t end_x;
    uint8_t start_y;
    bool changed;
    bool displayed;
};

struct displayed_char_t
{ // variable height, fixed width
    uint8_t start_x;
    uint8_t start_y;
    uint8_t end_y;
    uint16_t cset_start_address;
    uint8_t height;
    uint16_t color;
    uint8_t previous_start_x;
    uint8_t previous_start_y;
    uint8_t previous_height;
    bool changed;
    // bool displayed;
};

struct displayed_sprite_t
{ // variable height and width
    uint8_t start_x;
    uint8_t end_x;
    uint8_t start_y;
    uint8_t end_y;
    uint8_t size; // 1 for normal sized ones, 2 for double-sized ones
    bool shift;
    bool even_shift;
    uint16_t color;
    uint8_t previous_start_x;
    uint8_t previous_start_y;
    uint8_t previous_size;
    bool changed;
};

//
// Constantes
//

#define NB_H_SEGMENTS 81
#define NB_V_SEGMENTS 80
#define NB_CHARS 28
#define NB_SPRITES 4

#define LEFT_OFFSET 10

//
// Storage
//

extern bool grid_control;
extern bool foreground_control;
extern bool grid_dots;

extern uint8_t v_segments_width; 

extern uint16_t grid_color;
extern uint16_t background_color;

extern bool grid_uptodate;
extern bool h_segs_uptodate;
extern bool v_segs_uptodate;
extern bool dots_uptodate;
extern bool background_uptodate;
extern bool sprites_uptodate;
extern bool chars_uptodate;

// stockage des segments

extern h_segment_t h_segments[NB_H_SEGMENTS];
extern dot_t dots[NB_H_SEGMENTS];
extern v_segment_t v_segments[NB_V_SEGMENTS];

// stockage des caractères

extern displayed_char_t displayed_chars[NB_CHARS];

// stockage des sprites

extern displayed_sprite_t displayed_sprites[NB_SPRITES];

// Functions

void init_grid_elements();
void init_displayed_chars();
void init_displayed_sprites();

#endif /* (TRAVOLTA_DISPLAY_H) */
