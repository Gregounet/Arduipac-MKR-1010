#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
// #include <RTCZero.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// RTCZero rtc;

uint8_t minutes;
uint8_t previous_minutes;
uint8_t seconds;
uint8_t previous_seconds;

#include "arduipac.h"
#include "arduipac_vmachine.h"
#include "arduipac_8048.h"
#include "arduipac_8245.h"
#include "arduipac_bios_rom.h"
#include "arduipac_input.h"

#define TEXT_TFT_CS 0
#define TEXT_TFT_RST -1
#define TEXT_TFT_DC 1

// Raspberry Pi Pico:
#define GRAPHIC_TFT_CS 17
#define GRAPHIC_TFT_RST 21
#define GRAPHIC_TFT_DC 20
#define GRAPHIC_TFT_MOSI 19
#define GRAPHIC_TFT_SCLK 18

// Arduino MKR 1010 WiFi:
// #define GRAPHIC_TFT_CS 6
// #define GRAPHIC_TFT_RST -1
// #define GRAPHIC_TFT_DC 7

#ifdef DEBUG_TFT
Adafruit_ST7735 text_tft = Adafruit_ST7735(TEXT_TFT_CS, TEXT_TFT_DC, TEXT_TFT_RST);
#endif

// MKR 1010:
// Adafruit_ST7789 graphic_tft = Adafruit_ST7789(GRAPHIC_TFT_CS, GRAPHIC_TFT_DC, GRAPHIC_TFT_RST);

// Pico:
// Adafruit_ST7789 graphic_tft = Adafruit_ST7789(GRAPHIC_TFT_CS, GRAPHIC_TFT_DC, GRAPHIC_TFT_MOSI,  GRAPHIC_TFT_SCLK, GRAPHIC_TFT_RST);
Adafruit_ST7789 graphic_tft = Adafruit_ST7789(GRAPHIC_TFT_CS, GRAPHIC_TFT_DC, GRAPHIC_TFT_RST);

#ifdef DEBUG_TFT
void text_print_string(char *text)
{
  text_tft.print(text);
}

void text_print_dec(uint32_t number)
{
  text_tft.print(number, DEC);
}

void text_print_hex(uint32_t number)
{
  text_tft.print(number, HEX);
}
#endif

void graphic_drawtext(char *text)
{
  graphic_tft.fillScreen(ST77XX_BLACK);
  graphic_tft.setTextColor(ST77XX_RED);
  graphic_tft.setCursor(0, 0);
  graphic_tft.print(text);
}

char welcome_string[] = "Travolta";

void setup()
{
  Serial.begin(9600);
  delay(100);

#ifdef DEBUG_TFT
  text_tft.initR(INITR_BLACKTAB);
  text_tft.setSPISpeed(100000000);
  text_tft.fillScreen(ST77XX_BLACK);
  text_tft.setRotation(1);
  text_tft.setTextColor(ST77XX_GREEN);
#endif

  graphic_tft.init(240, 320);
  graphic_tft.fillScreen(ST77XX_BLACK);
  // graphic_tft.setSPISpeed(100000000);
  graphic_tft.setRotation(1);
  graphic_tft.setTextColor(ST77XX_WHITE);

  // graphic_drawtext(welcome_string);
  graphic_tft.print("La fievre");
#ifdef DEBUG
  Serial.println(welcome_string);
  Serial.println("Entering main()");
#endif

  pinMode(KEYBOARD_R1, INPUT_PULLDOWN);
  pinMode(KEYBOARD_R2, INPUT_PULLDOWN);
  pinMode(KEYBOARD_R3, INPUT_PULLDOWN);
  pinMode(KEYBOARD_R3, INPUT_PULLDOWN);

  pinMode(KEYBOARD_C1, OUTPUT);
  pinMode(KEYBOARD_C2, OUTPUT);
  pinMode(KEYBOARD_C3, OUTPUT);

  pinMode(UNO_JOYSTICK_B0, INPUT);
  pinMode(UNO_JOYSTICK_B1, INPUT);
  pinMode(UNO_JOYSTICK_B2, INPUT);
  pinMode(UNO_JOYSTICK_B3, INPUT);
  pinMode(UNO_JOYSTICK_B4, INPUT);

  pinMode(UNO_JOYSTICK_SELECT, OUTPUT);

#ifdef DEBUG
  Serial.println("main(): launching init_intel8245()");
#endif

  init_intel8245();

#ifdef DEBUG
  Serial.println("main(): launching init_intel8048()");
#endif

  init_intel8048();

#ifdef DEBUG
  Serial.println("main(): launching init_vmachine()");
#endif

  init_vmachine();

#ifdef DEBUG
  Serial.println("main(): launching exec_8048()");
#endif

  // rtc.begin();
  // rtc.setMinutes(0);
  // rtc.setSeconds(0);

  exec_8048();
}

void loop()
{
#ifdef DEBUG
  Serial.println("loop()");
#endif
}
