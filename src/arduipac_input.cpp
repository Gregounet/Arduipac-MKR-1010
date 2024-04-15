#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <Adafruit_Keypad.h>

#include "arduipac.h"
#include "arduipac_input.h"
#include "arduipac_8048.h"
#include "arduipac_config.h"

#define R1 21
#define R2 20
#define R3 19
#define R4 18
#define C1 17
#define C2 16
#define C3 15

#undef DEBUG_KEYPAD

const byte ROWS = 4; // rows
const byte COLS = 3; // columns

char keys[ROWS][COLS] = {
    {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3};

Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void write_p1(uint8_t data)
{
#ifdef DEBUG_STDERR
  fprintf(stderr, "write_p1(0x%02X)\n", data);
#endif
#ifdef DEBUG_SERIAL
  Serial.print(bigben);
  Serial.print(" - write_p1(0x");
  Serial.print(data, HEX);
  Serial.println(")");
#endif
#ifdef DEBUG_TFT
  text_print_string("write_p1()");
  delay(TFT_DEBUG_DELAY);
#endif

  p1 = data;
}

uint8_t
read_p2()
{
  uint8_t scan_input;
  uint8_t scan_output;

#ifdef DEBUG_STDERR
  fprintf(stderr, "read_p2()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.print(bigben);
  Serial.print(" - read_p2() - valeur P2 == ");
  Serial.println(p2, HEX);
#endif
#ifdef DEBUG_TFT
  text_print_string("read_p2()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  if (!(p1 & 0x04)) // Lecture du clavier
  {
    scan_input = (p2 & 0x07);
    scan_output = 0xFF;
    if (scan_input == 0)
    {
#ifdef DEBUG_KEYPAD
      Serial.print(bigben);
      Serial.println(" - Lecture ligne 0 du clavier");
#endif
      while (scan_output == 0xFF)
      {
#ifdef DEBUG_KEYPAD
        Serial.println("Attente d'un événement");
#endif
        customKeypad.tick();
        int nb_evenements = customKeypad.available();
#ifdef DEBUG_KEYPAD
        Serial.print("nb_evenements == ");
        Serial.println(nb_evenements);
#endif
        for (int i = 0; i < nb_evenements; i++)
        {
#ifdef DEBUG_KEYPAD
          Serial.println("Lecture d'un événement");
#endif
          keypadEvent e = customKeypad.read();
#ifdef DEBUG_KEYPAD
          Serial.println("Evénement lu");
#endif
          if (e.bit.EVENT == KEY_JUST_PRESSED)
          {
#ifdef DEBUG_KEYPAD
            Serial.println("Touche pressée");
#endif
            switch ((char)e.bit.KEY)
            {
            case '1':
#ifdef DEBUG_KEYPAD
              Serial.println("Touche 1");
#endif
              scan_output = 0x01;
              break;
            case '2':
#ifdef DEBUG_KEYPAD
              Serial.println("Touche 2");
#endif
              scan_output = 0x02;
              break;
            }
          };
        }
        delay(1000);
      }
      p2 &= 0x0F;
      p2 |= scan_output << 5;
    }
  }
  else
    p2 |= 0xF0;
  return p2;
}

uint8_t
in_bus()
{
  uint8_t data = 0;
  uint8_t scan_input = 0;
  uint8_t mode = 0;
  uint8_t jn = 0;

#ifdef DEBUG_STDERR
  fprintf(stderr, "in_bus()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.print(bigben);
  Serial.println(" - in_bus()");
#endif
#ifdef DEBUG_TFT
  text_print_string("\nread_p2()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  if ((p1 & 0x08) && (p1 & 0x10)) // Ni le 8245 ni la RAM externe ne sont activés TODO: remplacer par p1 & 0x18 == 0x18
  {
    if (!(p1 & 0x04))
      scan_input = (p2 & 0x07); // On cherche bien à lire les données du clavier. p2 & 0x07 sont les données précedement ecrites dans p2 pour scanner le clavier
    data = 0xFF;
    data &= 0xFE; // up
    /*
       Données des joysticks
       data &= 0xFE; // up
       data &= 0xFD; // right
       data &= 0xFB; // down
       data &= 0xF7; // left
       data &= 0xEF; // both buttons
     */
  }
  return data;
}
