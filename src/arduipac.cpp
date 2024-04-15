#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "arduipac.h"
#include "arduipac_vmachine.h"
#include "arduipac_8048.h"
#include "arduipac_8245.h"
#include "arduipac_bios_rom.h"
#include "arduipac_input.h"
#include "arduipac_config.h"

#define TEXT_TFT_CS 0
#define TEXT_TFT_RST -1
#define TEXT_TFT_DC 1

#define GRAPHIC_TFT_CS 6
#define GRAPHIC_TFT_RST -1
#define GRAPHIC_TFT_DC 7

Adafruit_ST7735 text_tft = Adafruit_ST7735(TEXT_TFT_CS, TEXT_TFT_DC, TEXT_TFT_RST);
Adafruit_ST7789 graphic_tft = Adafruit_ST7789(GRAPHIC_TFT_CS, GRAPHIC_TFT_DC, GRAPHIC_TFT_RST);

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

void graphic_drawtext(char *text)
{
  graphic_tft.fillScreen(ST77XX_BLACK);
  graphic_tft.setTextColor(ST77XX_RED);
  graphic_tft.setCursor(0, 0);
  graphic_tft.print(text);
}

#define ARDUIPAC_VERSION "This is the end"
void setup()
{
char welcome_string[] = "Arduipac MKR Wifi 1010 " ARDUIPAC_VERSION;

  Serial.begin(38400);
  delay(100);

  text_tft.initR(INITR_BLACKTAB);
  text_tft.setSPISpeed(100000000);
  text_tft.fillScreen(ST77XX_BLACK);
  text_tft.setRotation(1);
  text_tft.setTextColor(ST77XX_GREEN);

  delay(TFT_DEBUG_DELAY);

  graphic_tft.init(240, 320);
  graphic_tft.fillScreen(ST77XX_BLACK);
  graphic_tft.setSPISpeed(100000000);
  graphic_tft.setRotation(1);
  graphic_tft.setTextColor(ST77XX_WHITE);
  delay(TFT_DEBUG_DELAY);

  delay(2000) ;
  
#ifdef DEBUG_STDERR
  fprintf(stderr, "%s\n", welcome_string);
#endif
#ifdef DEBUG_SERIAL
  Serial.println(welcome_string);
#endif

#define DEBUG_TFT
#ifdef DEBUG_TFT
  text_print_string(welcome_string);
  delay(TFT_DEBUG_DELAY);
  graphic_drawtext(welcome_string);
  delay(TFT_DEBUG_DELAY);
#endif
#undef DEBUG_TFT

#ifdef DEBUG_STDERR
  fprintf(stderr, "Entering main()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("Entering main()");
#endif
#ifdef DEBUG_TFT
  text_print_string("Entering main()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  customKeypad.begin();

  // collision = NULL;

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching init_intel8245()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching init_intel8245()");
#endif
#ifdef DEBUG_TFT
  text_print_string("main(): launching init_intel8245()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  init_intel8245();

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching init_intel8048()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching init_intel8048()");
#endif
#ifdef DEBUG_TFT
  text_print_string("main(): launching init_intel8048()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  init_intel8048();

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching init_vmachine()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching init_vmachine()");
#endif
#ifdef DEBUG_TFT
  text_print_string("main(): launching init_vmachine()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  init_vmachine();

#ifdef DEBUG_STDERR
  fprintf(stderr, "main(): launching exec_8048()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("main(): launching exec_8048()");
#endif
#ifdef DEBUG_TFT
  text_print_string("main(): launching exec_8048()\n");
  delay(TFT_DEBUG_DELAY);
#endif

  exec_8048();
}

void loop()
{
#ifdef DEBUG_STDERR
  fprintf(stderr, "loop()\n");
#endif
#ifdef DEBUG_SERIAL
  Serial.println("loop()");
#endif
#ifdef DEBUG_TFT
  text_print_string("loop\n");
  delay(TFT_DEBUG_DELAY);
#endif
}
