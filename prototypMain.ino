#include <Servo.h>
#include <CuteBuzzerSounds.h>

// LEDs
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;
const int ledPin4 = 5;
const int ledPin5 = 6;

// Ultraschall
#define PIN_TRIGGER 9
#define PIN_ECHO     10

// Sonstiges
#define BUZZER_PIN 12
#define BUTTON_PIN 13

int LEDrot = A1;

int LEDgruen = A2;

int LEDblau = A3;

int Helligkeit = 255;  // Zahlenwert zwischen 0 und 255 – gibt die Leuchtstärke der einzelnen Farbe an
const int SENSOR_MAX_RANGE = 300;
unsigned long awayStart = 0;
unsigned long returnStart = 0;
// Servos
Servo ServomotorRed;
Servo ServomotorGreen;

// Zustände
enum STATE
{
    WAIT_FOR_INPUT,
    USER_INFRONT,
    USER_AWAY,
    FINAL
};

STATE currentState = WAIT_FOR_INPUT;

// Zeitsteuerung
const unsigned long Phase1 = 2000;
const unsigned long Phase2 = 4000;
const unsigned long Phase3 = 6000;
const unsigned long Phase4 = 8000;
const unsigned long Phase5 = 10000;

unsigned long startTime = 0;
unsigned long lastMeasure = 0;

unsigned long durationTime = 0;
unsigned int distance = SENSOR_MAX_RANGE + 1;

int awayCounter = 0;

// ======================================================

void setup()
{
    Serial.begin(9600);
    cute.init(BUZZER_PIN);
    ServomotorGreen.attach(7);
    ServomotorRed.attach(8);
  pinMode(LEDblau, OUTPUT);
ServomotorGreen.write(170);
ServomotorRed.write(80);
  pinMode(LEDgruen, OUTPUT);

  pinMode(LEDrot, OUTPUT);
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    resetLEDs();
    digitalWrite(PIN_TRIGGER, LOW);
}

// ======================================================

void loop()
{
    measureDistance();
     if (millis() - lastMeasure >= 50)
    {
        lastMeasure = millis();
        measureDistance();
    }
    switch (currentState)
    {

    // -------------------------
    // Warten bis jemand kommt
    // -------------------------
    case WAIT_FOR_INPUT:
{
    resetLEDs();


    if (digitalRead(BUTTON_PIN) == LOW && distance < 100) // z.B. unter 100 cm
    {
        Serial.println("Benutzer erkannt.");
        startTime = millis();
        currentState = USER_INFRONT;
    }

        break;
}
    // -------------------------
    // Benutzer steht davor
    // -------------------------
   case USER_INFRONT:
{
    
    if (distance >= 300)
    {
        if (awayStart == 0)
        {
            awayStart = millis(); // Zeitpunkt merken
        }

        if (millis() - awayStart >= 200) // 2 Sekunden weg
        {
            Serial.println("Benutzer weg.");
            ServomotorGreen.write(170);
            ServomotorRed.write(80);
            cute.play(S_CONFUSED);
            awayStart = 0;
            currentState = USER_AWAY;
            returnStart = 0;
        }
    }
    else
    {
        // Benutzer wieder da
        awayStart = 0;
    }

    unsigned long elapsed = millis() - startTime;

    if (elapsed >= Phase1) digitalWrite(ledPin1, HIGH);
    if (elapsed >= Phase2) digitalWrite(ledPin2, HIGH);
    if (elapsed >= Phase3) digitalWrite(ledPin3, HIGH);
    if (elapsed >= Phase4) digitalWrite(ledPin4, HIGH);

    if (elapsed >= Phase5)
    {
        digitalWrite(ledPin5, HIGH);
        cute.play(S_HAPPY);
        Serial.println("Test Vor Final");
        currentState = FINAL;
        Serial.println(currentState);
        Serial.println("Test Nach Final");
        break;
    }

    break;
}

    // -------------------------
    // Benutzer hat den Bereich verlassen
    // -------------------------
case USER_AWAY:
{
     ServomotorRed.write(170);
    
    resetLEDs();
      analogWrite(LEDgruen, 0); // gruen einschalten
     analogWrite(LEDrot, Helligkeit); //Rot aus

    if (distance < 300)
    {
        if (returnStart == 0)
        {
            // Zeitpunkt merken, wann der Benutzer zurückgekommen ist
            returnStart = millis();
        }

        // Benutzer war 2 Sekunden durchgehend da
        if (millis() - returnStart >= 500)
        {
            startTime = millis();
            returnStart = 0;
            ServomotorGreen.write(170);
            ServomotorRed.write(80);
            analogWrite(LEDgruen, 0); // gruen aus
     analogWrite(LEDrot, 0); //Rot aus
            currentState = USER_INFRONT;
        }
    }
    else
    {
        // Benutzer ist wieder weggegangen -> Zeit zurücksetzen
        returnStart = 0;
    }

    break;
}
case FINAL:
{
    
    ServomotorGreen.write(75);
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledPin5, HIGH);

    analogWrite(LEDgruen, Helligkeit); // gruen einschalten
     analogWrite(LEDrot, 0); //Rot aus
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        while(digitalRead(BUTTON_PIN) == LOW) {
          delay(10);
        }

        resetLEDs();
        awayStart = 0;
        startTime = 0;
            ServomotorGreen.write(170);
    ServomotorRed.write(90);
        currentState = WAIT_FOR_INPUT;
    }

    break;
}
    }
}

// ======================================================
// Abstand messen
// ======================================================

void measureDistance()
{
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);

    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);

    digitalWrite(PIN_TRIGGER, LOW);

    durationTime = pulseIn(PIN_ECHO, HIGH, 30000); // 30 ms Timeout

    if (durationTime == 0)
    {
        // Kein Echo erhalten
        distance = SENSOR_MAX_RANGE + 1;
    }
    else
    {
        distance = durationTime / 58;
    }

  Serial.print("Distance to object: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println(currentState); 
   
    
}

// ======================================================
// LEDs zurücksetzen
// ======================================================

void resetLEDs()
{
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
      analogWrite(LEDgruen, 0); // gruen aus
     analogWrite(LEDrot, 0); //Rot aus

}

