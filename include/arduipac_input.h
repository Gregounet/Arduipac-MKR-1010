#ifndef ARDUIPAC_INPUT_H
#define ARDUIPAC_INPUT_H

#include <stdint.h>

#define KEYBOARD_R1 2
#define KEYBOARD_R2 3
#define KEYBOARD_R3 4
#define KEYBOARD_R4 5
#define KEYBOARD_C1 6
#define KEYBOARD_C2 7
#define KEYBOARD_C3 8

#define UNO_JOYSTICK_B0 9
#define UNO_JOYSTICK_B1 10
#define UNO_JOYSTICK_B2 11
#define UNO_JOYSTICK_B3 12
#define UNO_JOYSTICK_B4 13

#define UNO_JOYSTICK_SELECT 5

void write_p1 (uint8_t data);
uint8_t read_p2 ();
uint8_t in_bus ();

#endif /* ARDUIPAC_INPUT_H */
