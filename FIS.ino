
#include "FIS.h"

// Only used for sprintf
#include <stdio.h>
PulseSensorPlayground pulseSensor;
// Setup DHT22
DHT22 myDHT22(DHT22_PIN);
// Setup timer
auto timer = timer_create_default();
ReadData DataBuffer;
byte samplesUntilReport;
void setup(void)
{
  // start serial port
#if (TEST_MODE == true)
  Serial.begin(9600);
  Serial.println("Pornire citiri");
#endif

  // Setup PULSE
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);
  pulseSensor.setThreshold(THRESHOLD);
#if (TEST_MODE == true)
  if (!pulseSensor.begin()) {

    //PulseSensor initialization failed,

    for (;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(100);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(100 );
    }
  }
#endif
  timer.every(TIME_FRAME, ReadSensors);
}

void loop(void)
{
  timer.tick(); // tick the timer
  if (pulseSensor.sawNewSample()) {
   // delayMicroseconds(500);

  }

}



#if (TEST_MODE == true)
void print_DataBuffer(ReadData DataBuffer) {
  Serial.println("Datele din buffer sunt:");
  if (DataBuffer.temperature_error_code == DHT_ERROR_NONE) {
    Serial.print("Temperatura: ");
    Serial.print(DataBuffer.temperature_value);
    Serial.println("C ");
  }
  else {
    Serial.print(" Eroare la citirea temperaturii, cod eroare : ");
    Serial.println(DataBuffer.temperature_error_code);
  }
  if (DataBuffer.humidity_error_code == DHT_ERROR_NONE) {
    Serial.print("Umiditate: ");
    Serial.print(DataBuffer.humidity_value);
    Serial.println("%");
  }
  else {
    Serial.print(" Eroare la citirea umiditatii, cod eroare : ");
    Serial.println(DataBuffer.humidity_error_code);
  }
  if (DataBuffer.pulse_error_code == PULSE_ERROR_NONE) {
    Serial.print("Puls: ");
    Serial.print(DataBuffer.pulse_value);
    Serial.println(" /minut");
  }
  else {
    Serial.print(" Eroare la citirea pulsului, cod eroare : ");
    Serial.println(DataBuffer.pulse_error_code);
  }



}

#endif

bool ReadSensors(void) {
  DHT22_ERROR_t errorCode;



#if (TEST_MODE == true)
  Serial.println("Citire date...");
#endif

  errorCode = myDHT22.readData();

  DataBuffer.temperature_value = myDHT22.getTemperatureC();
  DataBuffer.humidity_value = myDHT22.getHumidity();
  DataBuffer.temperature_error_code = errorCode;
  DataBuffer.humidity_error_code = errorCode;

  if (pulseSensor.sawStartOfBeat()) {
    DataBuffer.pulse_value = pulseSensor.getBeatsPerMinute();
    DataBuffer.pulse_error_code = PULSE_ERROR_NONE;
    pulseSensor.outputBeat();
  }
  else {
    DataBuffer.pulse_error_code = PULSE_NOT_PRESENT;
  }

  print_DataBuffer(DataBuffer);
  return true;
}
