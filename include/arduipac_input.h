#ifndef ARDUIPAC_INPUT_H
#define ARDUIPAC_INPUT_H

#include <stdint.h>

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
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
#endif

//
// Pico:
//
#if (TRAVOLTA_TARGET == TRAVOLTA_PICO)
#define KEYBOARD_R1 11
#define KEYBOARD_R2 12
#define KEYBOARD_R3 13
#define KEYBOARD_R4 14
#define KEYBOARD_C1 8
#define KEYBOARD_C2 9
#define KEYBOARD_C3 10

#define UNO_JOYSTICK_B0 2
#define UNO_JOYSTICK_B1 3
#define UNO_JOYSTICK_B2 4
#define UNO_JOYSTICK_B3 5
#define UNO_JOYSTICK_B4 6

#define UNO_JOYSTICK_SELECT 7
#endif

void write_p1 (uint8_t data);
uint8_t read_p2 ();
uint8_t in_bus ();

#endif /* ARDUIPAC_INPUT_H */
