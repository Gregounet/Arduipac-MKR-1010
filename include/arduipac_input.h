#ifndef ARDUIPAC_INPUT_H
#define ARDUIPAC_INPUT_H

#include <stdint.h>
#include <Adafruit_Keypad.h>

void write_p1 (uint8_t data);
uint8_t read_p2 ();
uint8_t in_bus ();

extern Adafruit_Keypad arduipac_keyboard;
extern Adafruit_Keypad arduipac_joystick;

#endif /* ARDUIPAC_INPUT_H */
