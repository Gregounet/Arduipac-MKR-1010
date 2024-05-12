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
#endif

Adafruit_ST7789 tft = Adafruit_ST7789(graphic_tft_cs, graphic_tft_dc, graphic_tft_rst);

void setup()
{
#if defined(DEBUG)
  Serial.begin(9600);
  delay(100);
#endif

#if defined(DEBUG)
  Serial.println("Entering setup()");
#endif

  tft.init(240, 320);
  tft.fillScreen(ST77XX_BLACK);
  tft.setSPISpeed(100000000);
  tft.setRotation(1);
  tft.setTextColor(ST77XX_WHITE);
#ifdef TRAVOLTA_INVERT_DISPLAY
  tft.invertDisplay(false);
#endif

init_input_pins();

#if defined(DEBUG)
  Serial.println("main(): launching init_intel8245()");
#endif

  init_intel8245();

#if defined(DEBUG)
  Serial.println("main(): launching init_intel8048()");
#endif

  init_intel8048();

#if defined(DEBUG)
  Serial.println("main(): launching init_vmachine()");
#endif

  init_vmachine();

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
  rtc.begin();
  rtc.setMinutes(0);
  rtc.setSeconds(0);
#endif

#if defined(DEBUG)
  Serial.println("main(): launching exec_8048()");
#endif

  exec_8048();
}

void loop()
{
#if defined(DEBUG)
  Serial.println("loop()");
#endif
}
