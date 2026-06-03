#include <CapacitiveSensor.h>

#define PIN_RED   5
#define PIN_GREEN 6
#define PIN_BLUE  7

CapacitiveSensor cs_4_2 = CapacitiveSensor(4, 2);

const long TOUCH_THRESHOLD = 500; // ggf. anpassen

void setup() {
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  // LED aus
  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 0);
}

void loop() {
  long sensorValue = cs_4_2.capacitiveSensor(30);

  Serial.println(sensorValue);

  if (sensorValue > TOUCH_THRESHOLD) {
    // Türkis einschalten
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 151);
    analogWrite(PIN_BLUE, 157);
  } else {
    // LED ausschalten
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 0);
  }

  delay(50);
}