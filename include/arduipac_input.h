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

#define UNO_JOYSTICK_SELECT 0
#define UNO_JOYSTICK_B0 1
// Gnd
#define UNO_JOYSTICK_B1 2
#define UNO_JOYSTICK_B2 3
#define UNO_JOYSTICK_B3 4
#define UNO_JOYSTICK_B4 5
// Gnd
#define KEYBOARD_R1 6
#define KEYBOARD_R2 7
#define KEYBOARD_R3 8
#define KEYBOARD_R4 9
// Gnd
#define KEYBOARD_C1 10
#define KEYBOARD_C2 11
#define KEYBOARD_C3 12
 
#endif

void write_p1 (uint8_t data);
uint8_t read_p2 ();
uint8_t in_bus ();

#endif /* ARDUIPAC_INPUT_H */
