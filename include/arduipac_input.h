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

#define JOYSTICK_R1 5
#define JOYSTICK_R2 4
#define JOYSTICK_R3 3
#define JOYSTICK_R4 2
#define JOYSTICK_C1 10
#define JOYSTICK_C2 11
#define JOYSTICK_C3 12

void write_p1 (uint8_t data);
uint8_t read_p2 ();
uint8_t in_bus ();

#endif /* ARDUIPAC_INPUT_H */
