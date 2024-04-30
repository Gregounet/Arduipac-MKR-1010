#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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
in_bus()
{
  uint8_t data_output = 0xFF;

#if defined(DEBUG_STDERR) && defined(DEBUG_JOYSTICK)
  fprintf(stderr, "in_bus()\n");
#endif
#if defined(DEBUG_SERIAL) && defined(DEBUG_JOYSTICK)
  Serial.print("in_bus() - P1 == 0x");
  Serial.print(p1, HEX);
  Serial.print(" - P2 == 0x");
  Serial.println(p2, HEX);
#endif
#if defined(DEBUG_TFT) && defined(DEBUG_JOYSTICK)
  text_print_string("\nread_p2()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  if ((p1 & 0x18) == 0x18) // Ni le 8245 ni la RAM externe ne sont activés
  {
    if (p2 & 0x04)
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
  }

#if defined(DEBUG_STDERR)
#endif
#if defined(DEBUG_SERIAL)
  Serial.print("in_bus() returns 0x");
  Serial.println(data_output, HEX);
#endif
#if defined(DEBUG_TFT)
#endif

  return data_output;
}
