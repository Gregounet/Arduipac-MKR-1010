#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "arduipac.h"
#include "arduipac_input.h"
#include "arduipac_8048.h"
#include "arduipac_config.h"

// #define DEBUG_SERIAL
// #undef DEBUG_KEYBOARD
// #define DEBUG_JOYSTICK

// TODO: déplacer dans arduipac.c ou bien arduipac_8048.c ou encore arduipac_vmachine.C ce code
// Il n'est pas spécifique aux inputs puisqu'il gère aussi la sélection de la banque de ROMS
//
void write_p1(uint8_t data)
{
#ifdef DEBUG_STDERR
  fprintf(stderr, "write_p1(0x%02X)\n", data);
#endif
#ifdef DEBUG_SERIAL
  Serial.print(" - write_p1(0x");
  Serial.print(data, HEX);
  Serial.println(")");
#endif
#ifdef DEBUG_TFT
  text_print_string("write_p1()");
  delay(TFT_DEBUG_DELAY);
#endif

  p1 = data;
  rom_bank_select = (p1 & 0x01) ? 0x1000 : 0x0000;

#ifdef DEBUG_SERIAL
  Serial.print("rom_bank_select == 0x");
  Serial.println(rom_bank_select, HEX);
#endif
}

uint8_t
read_p2() // 4x3 Keypad used as a keyboard
{
  uint8_t scan_input;
  uint8_t scan_output = 0x01; // Aucune touche pressée

#if defined(DEBUG_STDERR) && defined(DEBUG_KEYBOARD)
  fprintf(stderr, "read_p2()\n");
#endif
#if defined(DEBUG_SERIAL) || defined(DEBUG_KEYBOARD)
  Serial.print(" - read_p2() - valeur P2 == 0x");
  Serial.println(p2, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_KEYBOARD)
  text_print_string("read_p2()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  if (!(p1 & 0x04))
  // Lecture du clavier
  {
    scan_input = (p2 & 0x07);

#if defined(DEBUG_SERIAL) || defined(DEBUG_KEYBOARD)
    Serial.print("scan_input == ");
    Serial.println(scan_input);
#endif

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
  }

  p2 &= 0x0F;
  p2 |= (scan_output << 4);

#if defined(DEBUG_STDERR) && defined(DEBUG_KEYBOARD)
  fprintf(stderr, "Retour read_p2() == 0x%02X\n", p2);
#endif
#if defined(DEBUG_SERIAL) || defined(DEBUG_KEYBOARD)
  Serial.print("scan_output == ");
  Serial.println(scan_output);
  Serial.print("Retour read_p2() == 0x");
  Serial.println(p2, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_KEYBOARD)
#endif

  return p2;
}

uint8_t
in_bus() // 4x3 Keypad used as a joystick
{
  uint8_t data_output = 0xFF;

  uint8_t joystick[12];
  for (uint8_t i = 0; i < 12; i++)
    joystick[i] = 0;

#if defined(DEBUG_STDERR) && defined(DEBUG_JOYSTICK)
  fprintf(stderr, "in_bus()\n");
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_JOYSTICK)
  Serial.print(bigben);
  Serial.print(" - in_bus() - P1 == 0x");
  Serial.print(p1, HEX);
  Serial.print(" - P2 == 0x");
  Serial.println(p2, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_JOYSTICK)
  text_print_string("\nread_p2()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  if ((p1 & 0x18) == 0x18     // Ni le 8245 ni la RAM externe ne sont activés
      && (p2 & 0x07) == 0x00) // Joystick #1
  {
    digitalWrite(JOYSTICK_C1, LOW);
    digitalWrite(JOYSTICK_C2, LOW);
    digitalWrite(JOYSTICK_C3, LOW);

    digitalWrite(JOYSTICK_C1, HIGH);
    joystick[0] = (uint8_t)digitalRead(JOYSTICK_R1);
    joystick[3] = (uint8_t)digitalRead(JOYSTICK_R2);
    joystick[6] = (uint8_t)digitalRead(JOYSTICK_R3);
    joystick[9] = (uint8_t)digitalRead(JOYSTICK_R4);
    digitalWrite(JOYSTICK_C1, LOW);

    digitalWrite(JOYSTICK_C2, HIGH);
    joystick[1] = (uint8_t)digitalRead(JOYSTICK_R1);
    joystick[4] = (uint8_t)digitalRead(JOYSTICK_R2);
    joystick[7] = (uint8_t)digitalRead(JOYSTICK_R3);
    joystick[10] = (uint8_t)digitalRead(JOYSTICK_R4);
    digitalWrite(JOYSTICK_C2, LOW);

    digitalWrite(JOYSTICK_C3, HIGH);
    joystick[2] = (uint8_t)digitalRead(JOYSTICK_R1);
    joystick[5] = (uint8_t)digitalRead(JOYSTICK_R2);
    joystick[8] = (uint8_t)digitalRead(JOYSTICK_R3);
    joystick[11] = (uint8_t)digitalRead(JOYSTICK_R4);
    digitalWrite(JOYSTICK_C3, LOW);

    if (joystick[0])
      data_output = 0xF6;
    if (joystick[1])
      data_output = 0xFE;
    if (joystick[2])
      data_output = 0xFC;
    if (joystick[3])
      data_output = 0xF7;
    if (joystick[5])
      data_output = 0xFD;
    if (joystick[6])
      data_output = 0xF3;
    if (joystick[7])
      data_output = 0xFB;
    if (joystick[8])
      data_output = 0xF9;
    if (joystick[9] || joystick[11])
      data_output &= 0xEF;
  }
#if defined(DEBUG_STDERR) && defined(DEBUG_JOYSTICK)
#endif
#if defined(DEBUG_SERIAL) || defined(DEBUG_JOYSTICK)
  Serial.print("in_bus() returns 0x");
  Serial.println(data_output, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_JOYSTICK)
#endif

  return data_output;
}
