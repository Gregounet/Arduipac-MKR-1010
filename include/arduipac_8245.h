#ifndef ARDUIPAC_8245_H
#define ARDUIPAC_8245_H

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

extern uint8_t collision_table[];
extern uint8_t intel8245_ram[];

extern uint8_t *col;

void init_intel8245 ();
void draw_display ();

void show_12chars ();
void show_1char (uint8_t x, uint8_t y, uint16_t offset, uint8_t color_index);
void show_4quads ();
void show_1quad (uint8_t);
void show_4sprites ();

void clear_collision ();

#endif /* ARDUIPAC_8245_H */
