#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <Adafruit_Keypad.h>

#include "arduipac.h"
#include "arduipac_input.h"
#include "arduipac_8048.h"
#include "arduipac_config.h"


// Test

#define R1 0
#define R2 21
#define R3 20
#define R4 19
#define C1 18
#define C2 17
#define C3 16

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
// #define DEBUG_SERIAL
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

    if (scan_input < 0x06) // Seulement 5 lignes du clavier sont scannées
    {
      if (scan_input == 0)
      {
#define DEBUG_SERIAL
#ifdef DEBUG_SERIAL
        Serial.println("Lecture ligne 0 du clavier");
#endif
        customKeypad.tick();
        if (customKeypad.available())
        {
          keypadEvent e = customKeypad.read();
          if (e.bit.EVENT == KEY_JUST_PRESSED)
          {
            // Serial.print("Touche pressée");
            // Serial.println((char)e.bit.KEY);

            switch ((char)e.bit.KEY)
              {
              case '1':
                scan_output = 0x01;
                break;
              case '2':
                scan_output = 0x02;
                break;
              }
          }
          p2 &= 0x0F;
          p2 |= scan_output << 5;
        }
      }
    }
    else
      p2 |= 0xF0;
  }
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
