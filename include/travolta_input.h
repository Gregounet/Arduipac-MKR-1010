#ifndef TRAVOLTA_INPUT_H
#define TRAVOLTA_INPUT_H

#include <stdint.h>

#if defined(EXTERNAL_KEYPADS) || defined(EXTERNAL_JOYSTICKS)
#include <SoftwareSerial.h>
#endif

#if defined(EXTERNAL_KEYPADS) // External keypads

extern const int keypads_rx;
extern const int keypads_tx;

extern SoftwareSerial uart_keypads;

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32_NANO)
#define KEYPADS_RX 6
#define KEYPADS_TX 7
#endif

#else // Direct keypad

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#define KEYPAD_R1 2
#define KEYPAD_R2 3
#define KEYPAD_R3 4
#define KEYPAD_R4 5
#define KEYPAD_C1 6
#define KEYPAD_C2 7
#define KEYPAD_C3 8
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_PICO)
#define KEYPAD_R1 6
#define KEYPAD_R2 7
#define KEYPAD_R3 8
#define KEYPAD_R4 9
#define KEYPAD_C1 10
#define KEYPAD_C2 11
#define KEYPAD_C3 12
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32_NANO)
#define KEYPAD_R1 8
#define KEYPAD_R2 7
#define KEYPAD_R3 6
#define KEYPAD_R4 5
#define KEYPAD_C1 4
#define KEYPAD_C2 3
#define KEYPAD_C3 2
#endif

extern const int keypad_r1 ;
extern const int keypad_r2 ;
extern const int keypad_r3 ;
extern const int keypad_c1 ;
extern const int keypad_c2 ;
extern const int keypad_c3 ;
extern const int keypad_c4 ;

#endif // End of keypad(s) choices

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32C3)
#define BUTTON 1
const int button = BUTTON;
#endif


#if defined(EXTERNAL_JOYSTICKS)

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32_NANO)
#define JOYSTICKS_RX 12
#define JOYSTICKS_TX 11
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_PICO)
#endif

const int joysticks_rx = JOYSTICKS_RX ;
const int joysticks_tx = JOYSTICKS_TX ;

extern SoftwareSerial uart_joysticks;

#else // Direct joystick

#if (TRAVOLTA_TARGET == TRAVOLTA_MKR1010)
#define JOYSTICK_B0 9
#define JOYSTICK_B1 10
#define JOYSTICK_B2 11
#define JOYSTICK_B3 12
#define JOYSTICK_B4 13
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_PICO)
#define JOYSTICK_B0 1
#define JOYSTICK_B1 2
#define JOYSTICK_B2 3
#define JOYSTICK_B3 4
#define JOYSTICK_B4 5
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32_NANO)
#define JOYSTICK_B0 A1          
#define JOYSTICK_B1 A2
#define JOYSTICK_B2 A0
#define JOYSTICK_B3 A3
#define JOYSTICK_B4 A4
#endif

#if (TRAVOLTA_TARGET == TRAVOLTA_ESP32C3)
#define JOYSTICK_B0 3
#define JOYSTICK_B1 9
#define JOYSTICK_B2 2
#define JOYSTICK_B3 8
#define JOYSTICK_B4 10
#endif

extern const int joystick_b0;
extern const int joystick_b1;
extern const int joystick_b2;
extern const int joystick_b3;
extern const int joystick_b4;

#endif // End of joysticks choice

void init_input_pins(void);
uint8_t read_port2 (void);
uint8_t read_bus (void);

#endif /* TRAVOLTA_INPUT_H */
