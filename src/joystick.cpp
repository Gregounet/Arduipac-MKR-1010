#include <Arduino.h>

#define JOYSTICK_FIRE_PIN 1  // Red wire
#define JOYSTICK_LEFT_PIN 2  // Orange
#define JOYSTICK_RIGHT_PIN 3 // Green
#define JOYSTICK_UP_PIN 4    // White
#define JOYSTICK_DOWN_PIN 5  // Yellow

void setup()
{
  Serial.begin(9600);
  delay(1000);

  pinMode(JOYSTICK_FIRE_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_LEFT_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_RIGHT_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_UP_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_DOWN_PIN, INPUT_PULLUP);
}

void loop()
{
  uint8_t scan_out = 0x00;

  if (digitalRead(JOYSTICK_FIRE_PIN) == LOW)
    scan_out |= 0x10;
  if (digitalRead(JOYSTICK_LEFT_PIN) == LOW)
    scan_out |= 0x01;
  if (digitalRead(JOYSTICK_RIGHT_PIN) == LOW)
    scan_out |= 0x02;
  if (digitalRead(JOYSTICK_UP_PIN) == LOW)
    scan_out |= 0x04;
  if (digitalRead(JOYSTICK_DOWN_PIN) == LOW)
    scan_out |= 0x08;

  Serial.println(scan_out, HEX);
  switch (scan_out & 0x0F)
  {
  case 0x01:
    Serial.print("Left");
    break;
  case 0x02:
    Serial.print("Right");
    break;
  case 0x04:
    Serial.print("Up");
    break;
  case 0x08:
    Serial.print("Down");
    break;
  case 0x05:
    Serial.print("Left up");
    break;
  case 0x06:
    Serial.print("Right up");
    break;
  case 0x09:
    Serial.print("Left down");
    break;
  case 0x0A:
    Serial.print("Right down");
    break;
  case 0x00:
    Serial.print("Neutral");
    break;
  default:
    Serial.print("Bug bug");
    break;
  };

  if ((scan_out & 0x10) == 0x10)
    Serial.println(" + fire");
  else
    Serial.println();

  delay(100);
}
