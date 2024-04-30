#ifndef ARDUIPAC_INPUT_H
#define ARDUIPAC_INPUT_H

#include <stdint.h>

#define KEYBOARD_R1 21
#define KEYBOARD_R2 20
#define KEYBOARD_R3 19
#define KEYBOARD_R4 18
#define KEYBOARD_C1 17
#define KEYBOARD_C2 16
#define KEYBOARD_C3 15

#define UNO_JOYSTICK_B0 0
#define UNO_JOYSTICK_B1 1
#define UNO_JOYSTICK_B2 2
#define UNO_JOYSTICK_B3 3
#define UNO_JOYSTICK_B4 4

#define UNO_JOYSTICK_SELECT 5

void write_p1 (uint8_t data);
uint8_t read_p2 ();
uint8_t in_bus ();

#endif /* ARDUIPAC_INPUT_H */
