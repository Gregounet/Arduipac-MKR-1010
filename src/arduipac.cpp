#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "arduipac_vmachine.h"
#include "arduipac_8048.h"
#include "arduipac_8245.h"
#include "arduipac_bios_rom.h"

#undef DEBUG_TFT
#define DEBUG_SERIAL
#undef DEBUG_STDERR

#define DEBUG 0

#define TFT_CS 6
#define TFT_RST -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 7

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void testdrawtext(char *text, uint16_t color)
{
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void setup()
{
  Serial.begin(9600);
  delay(1000);

#ifdef DEBUG_STDERR
  fprintf(stderr, "Arduipac MKR Wifi 1010 0.0\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("Arduipac MKR Wifi 1010 0.0");
#endif
  delay(1000);

  tft.init(240, 320);
  delay(1000);

  tft.fillScreen(ST77XX_BLACK);
  testdrawtext("Arduipac MKR Wifi 1010 0.0", ST77XX_WHITE);
  delay(1000);

  // tft.setSPISpeed(40000000);

#ifdef DEBUG_STDERR
  fprintf(stderr, "Entering main()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("Entering main()");
#endif

  // collision = NULL;

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching init_intel8225()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching init_intel8225()");
#endif

  init_intel8225();

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching init_intel8048()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching init_intel8048()");
#endif

  init_intel8048();

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching init_vmachine()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching init_vmachine()");
#endif

  init_vmachine();

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching exec_8048()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching exec_8048()");
#endif

  exec_8048();
}

void loop()
{
  Serial.println("Looping, really ?");
  delay(2000);
}
