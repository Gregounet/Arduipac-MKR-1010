#ifndef TRAVOLTA_8048_H
#define TRAVOLTA_8048_H

extern uint8_t horizontal_clock;

extern uint8_t port1;
extern uint8_t port2;

extern uint16_t rom_bank_select;

void ext_irq (void);
void init_intel8048 (void);
void exec_8048 (void);

extern uint32_t bigben;

#endif /*TRAVOLTA_8048_H */
