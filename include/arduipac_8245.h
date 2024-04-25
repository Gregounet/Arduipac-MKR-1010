#ifndef ARDUIPAC_8245_H
#define ARDUIPAC_8245_H

extern uint8_t collision_table[];
extern uint8_t intel8245_ram[];

void init_intel8245 ();
void draw_display ();

void show_chars ();
void show_sprites ();

void clear_collision ();

#endif /* ARDUIPAC_8245_H */
