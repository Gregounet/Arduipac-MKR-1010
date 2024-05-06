#ifndef TRAVOLTA_INPUT_H
#define TRAVOLTA_INPUT_H

#include <stdint.h>

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#define KEYBOARD_R1 2
#define KEYBOARD_R2 3
#define KEYBOARD_R3 4
#define KEYBOARD_R4 5
#define KEYBOARD_C1 6
#define KEYBOARD_C2 7
#define KEYBOARD_C3 8

#define UNO_JOYSTICK 1
#define KEYPAD 1

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

#define UNO_JOYSTICK 1
#define KEYPAD 1

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

//
// Mini ESP32-C3:
//
#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32C3)

#undef UNO_JOYSTICK
#undef KEYPAD

#define JOYSTICK_B0 3
#define JOYSTICK_B1 9
#define JOYSTICK_B2 2
#define JOYSTICK_B3 8
#define JOYSTICK_B4 10

#define BUTTON 1
 
#endif

//
// Arduino Nano ESP32:
//
#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32_NANO)

#undef UNO_JOYSTICK
#define KEYPAD 1

#define JOYSTICK_B0 A1          
#define JOYSTICK_B1 A2
#define JOYSTICK_B2 A0
#define JOYSTICK_B3 A3
#define JOYSTICK_B4 A4

#define KEYBOARD_R1 8
#define KEYBOARD_R2 7
#define KEYBOARD_R3 6
#define KEYBOARD_R4 5

#define KEYBOARD_C1 4
#define KEYBOARD_C2 3
#define KEYBOARD_C3 2
 
#endif

uint8_t read_port2 ();
uint8_t read_bus ();

#endif /* TRAVOLTA_INPUT_H */
