#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>


#include "travolta.h"
#include "travolta_vmachine.h"
#include "travolta_8048.h"
#include "travolta_8245.h"
#include "travolta_bios_rom.h"
#include "travolta_input.h"

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#include <RTCZero.h>
RTCZero rtc;

uint8_t minutes;
uint8_t previous_minutes;
uint8_t seconds;
uint8_t previous_seconds;

#define GRAPHIC_TFT_CS 6
#define GRAPHIC_TFT_RST -1
#define GRAPHIC_TFT_DC 7
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_PICO)
#define GRAPHIC_TFT_CS 17
#define GRAPHIC_TFT_RST 21
#define GRAPHIC_TFT_DC 20
#endif

Adafruit_ST7789 tft = Adafruit_ST7789(GRAPHIC_TFT_CS, GRAPHIC_TFT_DC, GRAPHIC_TFT_RST);

void setup()
{
  Serial.begin(9600);
  delay(100);

  tft.init(240, 320);
  tft.fillScreen(ST77XX_BLACK);
  tft.setSPISpeed(100000000);
  tft.setRotation(1);
  tft.setTextColor(ST77XX_WHITE);
#ifdef TRAVOLTA_INVERT_DISPLAY
  tft.invertDisplay(false);
#endif

#ifdef DEBUG
  Serial.println("Entering main()");
#endif

  pinMode(KEYBOARD_R1, INPUT_PULLDOWN);
  pinMode(KEYBOARD_R2, INPUT_PULLDOWN);
  pinMode(KEYBOARD_R3, INPUT_PULLDOWN);
  pinMode(KEYBOARD_R4, INPUT_PULLDOWN);

  pinMode(KEYBOARD_C1, OUTPUT);
  pinMode(KEYBOARD_C2, OUTPUT);
  pinMode(KEYBOARD_C3, OUTPUT);

  pinMode(UNO_JOYSTICK_B0, INPUT_PULLUP);
  pinMode(UNO_JOYSTICK_B1, INPUT_PULLUP);
  pinMode(UNO_JOYSTICK_B2, INPUT_PULLUP);
  pinMode(UNO_JOYSTICK_B3, INPUT_PULLUP);
  pinMode(UNO_JOYSTICK_B4, INPUT_PULLUP);

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

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
  rtc.begin();
  rtc.setMinutes(0);
  rtc.setSeconds(0);
#endif

#ifdef DEBUG
  Serial.println("main(): launching exec_8048()");
#endif

  exec_8048();
}

void loop()
{
#ifdef DEBUG
  Serial.println("loop()");
#endif
}
