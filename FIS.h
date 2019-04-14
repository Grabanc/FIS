#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
#include <timer.h>
#include "DHT22.h" 
#define TEST_MODE true

 // The temp/humidity sensor can only be read from every 1-2s, and requires a minimum 2s warm-up after power-on.
#define TIME_FRAME 4000                    // time between 2 consecutive reads in ms

// Data wire is plugged into port 7 on the Arduino
// Connected 1K resistor between VCC and the data pin (strong pullup)

#define DHT22_PIN 7

/*
   Pinout:
     PULSE_INPUT_PIN = Analog Input. Connected to the pulse sensor purple (signal) wire.
     PULSE_BLINK = digital Output. Connected to an LED (and 220 ohm resistor)
      that will flash on each detected pulse.
     PULSE_FADE = digital Output. PWM pin onnected to an LED (and resistor)
      that will smoothly fade with each pulse.
      NOTE: PULSE_FADE must be a pin that supports PWM. Do not use
      pin 9 or 10, because those pins' PWM interferes with the sample timer.
*/
#define PULSE_PIN A0
#define PULSE_BLINK 13    // Pin 13 is the on-board LED
#define PULSE_FADE 5
#define THRESHOLD 550   // Adjust this number to avoid noise when idle 


typedef struct{
    int temperature_value;
    int temperature_error_code;
    int humidity_value;
    int humidity_error_code;
    int pulse_value;
    int pulse_error_code;
    
} ReadData;

typedef enum
{
  PULSE_ERROR_NONE = 0,
  PULSE_NOT_PRESENT
} PULSE_ERROR_t;
