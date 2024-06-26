#include <Arduino.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "travolta.h"
#include "travolta_input.h"
#include "travolta_8048.h"

#if defined(EXTERNAL_KEYPADS)
const int input_b0 = INPUT_B0;
const int input_b1 = INPUT_B1;
const int input_b2 = INPUT_B2;
const int input_b4 = INPUT_B4;
const int input_b5 = INPUT_B5;
const int input_b6 = INPUT_B6;
const int input_b7 = INPUT_B7;
#endif

#if defined(EXTERNAL_JOYSTICKS)
#endif

#if defined(DIRECT_JOYSTICK)
const int joystick_b0 = JOYSTICK_B0;
const int joystick_b1 = JOYSTICK_B1;
const int joystick_b2 = JOYSTICK_B2;
const int joystick_b3 = JOYSTICK_B3;
const int joystick_b4 = JOYSTICK_B4;
#endif

#if defined(DIRECT_KEYPAD)
const int keypad_r1 = KEYPAD_R1;
const int keypad_r2 = KEYPAD_R2;
const int keypad_r3 = KEYPAD_R3;
const int keypad_r4 = KEYPAD_R4;
const int keypad_c1 = KEYPAD_C1;
const int keypad_c2 = KEYPAD_C2;
const int keypad_c3 = KEYPAD_C3;
#endif

void init_input_pins()
{
#if defined(EXTERNAL_KEYPADS)
  pinMode(input_b4, INPUT);
  pinMode(input_b5, INPUT);
  pinMode(input_b6, INPUT);
  pinMode(input_b7, INPUT);
  pinMode(input_b0, INPUT);
  pinMode(input_b1, INPUT);
  pinMode(input_b2, INPUT);
#endif

#if defined(DIRECT_KEYPAD)
  pinMode(keypad_r1, INPUT_PULLUP);
  pinMode(keypad_r2, INPUT_PULLUP);
  pinMode(keypad_r3, INPUT_PULLUP);
  pinMode(keypad_r4, INPUT_PULLUP);
  pinMode(keypad_c1, INPUT);
  pinMode(keypad_c2, INPUT);
  pinMode(keypad_c3, INPUT);
#endif

#if defined(BUTTON)
  pinMode(button, INPUT_PULLUP);
#endif

#if defined(EXTERNAL_JOYSTICKS)
  uart_joysticks.begin(9600);
#endif

#if defined(DIRECT_JOYSTICK)
  pinMode(JOYSTICK_B0, INPUT_PULLUP);
  pinMode(JOYSTICK_B1, INPUT_PULLUP);
  pinMode(JOYSTICK_B2, INPUT_PULLUP);
  pinMode(JOYSTICK_B3, INPUT_PULLUP);
  pinMode(JOYSTICK_B4, INPUT_PULLUP);
#endif
}

uint8_t
read_port2()
{
#if defined(DEBUG)
#endif
  Serial.print(" - read_port2() - valeur P2 == 0x");
  Serial.println(port2, HEX);

  if (!(port1 & 0x04))
  // Lecture du clavier
  {
    uint8_t scan_input = (port2 & 0x07);
    uint8_t scan_output = 0x10 | scan_input; // Aucune touche pressée
    uint8_t read_value;

#if defined(DEBUG)
#endif
    Serial.print("scan_input == ");
    Serial.println(scan_input, HEX);

#if defined(EXTERNAL_KEYPADS)
    read_value =
        digitalRead(input_b7) * 0x80 +
        digitalRead(input_b6) * 0x40 +
        digitalRead(input_b5) * 0x20 +
        digitalRead(input_b4) * 0x10 +
        digitalRead(input_b2) * 0x04 +
        digitalRead(input_b1) * 0x02 +
        digitalRead(input_b0) * 0x01;
#if defined(DEBUG)
    Serial.print("read_value == ");
    Serial.println(read_value, HEX);
    Serial.print("read_value & 7 == ");
    Serial.println(read_value & 7);
    Serial.print("read_value & 0x10 == ");
    Serial.println(read_value & 0x10, HEX);
#endif
    if ((read_value & 7) == scan_input && !(read_value & 0x10))
    {
#if defined(DEBUG)
      Serial.println("Caractère reçu");
#endif
      scan_output = read_value;
    }
    port2 = scan_output;

#endif

#if defined(DIRECT_KEYPAD) // use keypad to emulate keyboard
    uint8_t keyboard[12];

    for (uint8_t i = 0; i < 12; i++)
      keyboard[i] = 0;

    pinMode(keypad_c1, OUTPUT);
    digitalWrite(keypad_c1, LOW);
    keyboard[0] = (uint8_t)digitalRead(keypad_r1);
    keyboard[3] = (uint8_t)digitalRead(keypad_r2);
    keyboard[6] = (uint8_t)digitalRead(keypad_r3);
    keyboard[9] = (uint8_t)digitalRead(keypad_r4);
    digitalWrite(keypad_c1, HIGH);
    pinMode(keypad_c1, INPUT);

    pinMode(keypad_c2, OUTPUT);
    digitalWrite(keypad_c2, LOW);
    keyboard[1] = (uint8_t)digitalRead(keypad_r1);
    keyboard[4] = (uint8_t)digitalRead(keypad_r2);
    keyboard[7] = (uint8_t)digitalRead(keypad_r3);
    keyboard[10] = (uint8_t)digitalRead(keypad_r4);
    digitalWrite(keypad_c2, HIGH);
    pinMode(keypad_c2, INPUT);

    pinMode(keypad_c3, OUTPUT);
    digitalWrite(keypad_c3, LOW);
    keyboard[2] = (uint8_t)digitalRead(keypad_r1);
    keyboard[5] = (uint8_t)digitalRead(keypad_r2);
    keyboard[8] = (uint8_t)digitalRead(keypad_r3);
    keyboard[11] = (uint8_t)digitalRead(keypad_r4);
    digitalWrite(keypad_c3, HIGH);
    pinMode(keypad_c3, INPUT);

    for (uint8_t i = 0; i < 12; i++)
    {
      Serial.print("keyboard[");
      Serial.print(i);
      Serial.print("] == ");
      Serial.println(keyboard[i]);
    }

    switch (scan_input)
    {
    case 0:
    {
      if (keyboard[10] == 0)
        scan_output = 0x00;
      if (keyboard[0] == 0)
        scan_output = 0x20;
      if (keyboard[1] == 0)
        scan_output = 0x40;
      if (keyboard[2] == 0)
        scan_output = 0x60;
      if (keyboard[3] == 0)
        scan_output = 0x80;
      if (keyboard[4] == 0)
        scan_output = 0xA0;
      if (keyboard[5] == 0)
        scan_output = 0xC0;
      if (keyboard[6] == 0)
        scan_output = 0xE0;
      break;
    }
    case 1:
    {
      if (keyboard[7] == 0)
        scan_output = 0x01;
      if (keyboard[8] == 0)
        scan_output = 0x21;
      break;
    }
    case 5:
    {
      if (keyboard[9] == 0)
        scan_output = 0x89;
      if (keyboard[11] == 0)
        scan_output = 0xA9;
      break;
    }
    }
    port2 = scan_output;
#if defined(DEBUG)
#endif
    Serial.print("scan_output == ");
    Serial.println(scan_output, HEX);
#endif

#if defined(BUTTON)
    // use button to emulate a one button keyboard !
    if (digitalRead(BUTTON) == LOW && scan_input == 0)
      scan_output = 0x02;
#endif
  }

  Serial.print("Retour read_port2() == 0x");
  Serial.println(port2, HEX);

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

#if defined(EXTERNAL_JOYSTICKS)
    if (port2 & 0x04)
      digitalWrite(UNO_JOYSTICK_SELECT, HIGH);
    else
      digitalWrite(UNO_JOYSTICK_SELECT, LOW);
#endif

#if defined(DIRECT_JOYSTICK)
    if (digitalRead(joystick_b0) == LOW)
      data_output &= 0xFE;
    if (digitalRead(joystick_b1) == LOW)
      data_output &= 0xFD;
    if (digitalRead(joystick_b2) == LOW)
      data_output &= 0xFB;
    if (digitalRead(joystick_b3) == LOW)
      data_output &= 0xF7;
    if (digitalRead(joystick_b4) == LOW)
      data_output &= 0xEF;
#endif
  }

#if defined(DEBUG)
  Serial.print("read_bus() returns 0x");
  Serial.println(data_output, HEX);
#endif

  return data_output;
}
