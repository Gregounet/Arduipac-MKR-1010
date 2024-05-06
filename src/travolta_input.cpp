#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "travolta.h"
#include "travolta_input.h"
#include "travolta_8048.h"

uint8_t
read_port2() // 4x3 Keypad used as a keyboard
{
  uint8_t scan_input;
  uint8_t scan_output = 0x01; // Aucune touche pressée

#if defined(DEBUG)
  Serial.print(" - read_port2() - valeur P2 == 0x");
  Serial.println(port2, HEX);
#endif

  if (!(port1 & 0x04))
  // Lecture du clavier
  {
    scan_input = (port2 & 0x07);

#if defined(DEBUG)
    Serial.print("scan_input == ");
    Serial.println(scan_input);
#endif

#if defined(KEYPAD) // use keypad to emulate keyboard
    uint8_t keyboard[12];

    for (uint8_t i = 0; i < 12; i++)
      keyboard[i] = 0;

    digitalWrite(KEYBOARD_C1, LOW);
    digitalWrite(KEYBOARD_C2, LOW);
    digitalWrite(KEYBOARD_C3, LOW);

    digitalWrite(KEYBOARD_C1, HIGH);
    keyboard[0] = (uint8_t)digitalRead(KEYBOARD_R1);
    keyboard[3] = (uint8_t)digitalRead(KEYBOARD_R2);
    keyboard[6] = (uint8_t)digitalRead(KEYBOARD_R3);
    keyboard[9] = (uint8_t)digitalRead(KEYBOARD_R4);
    digitalWrite(KEYBOARD_C1, LOW);

    digitalWrite(KEYBOARD_C2, HIGH);
    keyboard[1] = (uint8_t)digitalRead(KEYBOARD_R1);
    keyboard[4] = (uint8_t)digitalRead(KEYBOARD_R2);
    keyboard[7] = (uint8_t)digitalRead(KEYBOARD_R3);
    keyboard[10] = (uint8_t)digitalRead(KEYBOARD_R4);
    digitalWrite(KEYBOARD_C2, LOW);

    digitalWrite(KEYBOARD_C3, HIGH);
    keyboard[2] = (uint8_t)digitalRead(KEYBOARD_R1);
    keyboard[5] = (uint8_t)digitalRead(KEYBOARD_R2);
    keyboard[8] = (uint8_t)digitalRead(KEYBOARD_R3);
    keyboard[11] = (uint8_t)digitalRead(KEYBOARD_R4);
    digitalWrite(KEYBOARD_C3, LOW);

    switch (scan_input)
    {
    case 0:
      if (keyboard[10])
        scan_output = 0x00;
      if (keyboard[0])
        scan_output = 0x02;
      if (keyboard[1])
        scan_output = 0x04;
      if (keyboard[2])
        scan_output = 0x06;
      if (keyboard[3])
        scan_output = 0x09;
      if (keyboard[4])
        scan_output = 0x0A;
      if (keyboard[5])
        scan_output = 0x0C;
      if (keyboard[6])
        scan_output = 0x0E;
      break;
    case 1:
      if (keyboard[7])
        scan_output = 0x00;
      if (keyboard[8])
        scan_output = 0x02;
      break;
    case 5:
      if (keyboard[9])
        scan_output = 0x08;
      if (keyboard[11])
        scan_output = 0x0A;
      break;
    }
    #else // use button to emulate a one button keyboard !
    if (digitalRead(BUTTON) == LOW && scan_input == 0) scan_output = 0x02 ;
    #endif
  }

  port2 &= 0x0F;
  port2 |= (scan_output << 4);

#if defined(DEBUG)
  Serial.print("scan_output == ");
  Serial.println(scan_output);
  Serial.print("Retour read_port2() == 0x");
  Serial.println(port2, HEX);
#endif

  return port2;
}

uint8_t
read_bus()
{
  uint8_t data_output = 0xFF;

#if defined(DEBUG)
  Serial.print("read_bus() - P1 == 0x");
  Serial.print(port1, HEX);
  Serial.print(" - P2 == 0x");
  Serial.println(port2, HEX);
#endif

  if ((port1 & 0x18) == 0x18) // Ni le 8245 ni la RAM externe ne sont activés
  {
#if defined(UNO_JOYSTICK)
    if (port2 & 0x04)
      digitalWrite(UNO_JOYSTICK_SELECT, HIGH);
    else
      digitalWrite(UNO_JOYSTICK_SELECT, LOW);

    if (digitalRead(UNO_JOYSTICK_B0) == LOW)
      data_output &= 0xFE;
    if (digitalRead(UNO_JOYSTICK_B1) == LOW)
      data_output &= 0xFD;
    if (digitalRead(UNO_JOYSTICK_B2) == LOW)
      data_output &= 0xFB;
    if (digitalRead(UNO_JOYSTICK_B3) == LOW)
      data_output &= 0xF7;
    if (digitalRead(UNO_JOYSTICK_B4) == LOW)
      data_output &= 0xEF;
#else
    if (digitalRead(JOYSTICK_B0) == LOW)
      data_output &= 0xFE;
    if (digitalRead(JOYSTICK_B1) == LOW)
      data_output &= 0xFD;
    if (digitalRead(JOYSTICK_B2) == LOW)
      data_output &= 0xFB;
    if (digitalRead(JOYSTICK_B3) == LOW)
      data_output &= 0xF7;
    if (digitalRead(JOYSTICK_B4) == LOW)
      data_output &= 0xEF;
#endif
  }

#if defined(DEBUG)
  Serial.print("read_bus() returns 0x");
  Serial.println(data_output, HEX);
#endif

  return data_output;
}
