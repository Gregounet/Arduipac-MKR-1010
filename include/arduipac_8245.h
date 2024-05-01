#ifndef ARDUIPAC_8245_H
#define ARDUIPAC_8245_H

extern uint8_t intel8245_ram[];

void init_intel8245 ();
void draw_display ();

uint8_t detect_collisions();

#endif /* ARDUIPAC_8245_H */
