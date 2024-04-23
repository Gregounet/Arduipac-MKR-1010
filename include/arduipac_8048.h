#ifndef ARDUIPAC_8048_H
#define ARDUIPAC_8048_H

extern uint8_t horizontal_clock;

extern uint8_t p1;
extern uint8_t p2;

extern uint16_t rom_bank_select;

extern uint32_t bigben;
extern int interest_page ;

extern uint16_t pc ;// TODO remove later

void ext_irq (void);
void init_intel8048 (void);
void exec_8048 (void);

void show_bigsprite() ;

#endif /*ARDUIPAC_8048_H */
