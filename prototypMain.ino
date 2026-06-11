#include <Servo.h>
#include <CuteBuzzerSounds.h>
// -------------------------
// LED-Pins
// -------------------------
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;
const int ledPin4 = 5;
const int ledPin5 = 6;

// -------------------------
// Ultraschallsensor
// -------------------------
#define PIN_TRIGGER 7
#define PIN_ECHO 8
#define BUZZER_PIN 12
const int SENSOR_MAX_RANGE = 300; // cm

unsigned long durationTime = 0;
unsigned int distance = SENSOR_MAX_RANGE + 1;

// -------------------------
// Servos
// -------------------------
Servo ServomotorRed;
Servo ServomotorGreen;

// -------------------------
// Zustände
// -------------------------
enum STATE
{
    WAIT_FOR_INPUT,
    USER_INFRONT,
    USER_AWAY
};

STATE currentState = WAIT_FOR_INPUT;

// -------------------------
// Zeitpunkte (ms)
// -------------------------
const unsigned long Phase1 = 2000;
const unsigned long Phase2 = 4000;
const unsigned long Phase3 = 6000;
const unsigned long Phase4 = 8000;
const unsigned long Phase5 = 10000;

unsigned long startTime = 0;
unsigned long awayStartTime = 0;


bool servoPos = false;
unsigned long servoTimer = 0;
// ======================================================

void setup()
{
    Serial.begin(9600);
    cute.init(BUZZER_PIN);
    ServomotorGreen.attach(10);
    ServomotorRed.attach(9);

    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);

    resetLEDs();

    digitalWrite(PIN_TRIGGER, LOW);
}

// ======================================================

void loop()
{
    switch (currentState)
    {

    // -------------------------
    // Warten bis jemand kommt
    // -------------------------
    case WAIT_FOR_INPUT:

        resetLEDs();

        measureDistance();

        // Hier ggf. Grenzwert anpassen (z.B. 50 cm)
        if (distance < 300)
        {
            Serial.println("Benutzer erkannt.");

            startTime = millis();

            currentState = USER_INFRONT;
        }

        break;

    // -------------------------
    // Benutzer steht davor
    // -------------------------
    case USER_INFRONT:
    {

        measureDistance();

        if (distance >= 300)
        {
            Serial.println("Benutzer weg.");
             awayStartTime = millis();
            currentState = USER_AWAY;
        

            break;
        }

        unsigned long elapsed = millis() - startTime;

        if (elapsed >= Phase1)
            digitalWrite(ledPin1, HIGH);

        if (elapsed >= Phase2)
            digitalWrite(ledPin2, HIGH);

        if (elapsed >= Phase3)
            digitalWrite(ledPin3, HIGH);

        if (elapsed >= Phase4)
            digitalWrite(ledPin4, HIGH);

        if (elapsed >= Phase5) {
            digitalWrite(ledPin5, HIGH);
            cute.play(S_HAPPY);
            delay(2000);
            ServomotorGreen.write(170);  // oben
            delay(2000);
            ServomotorGreen.write(10);

        }
            
        break;
    }

    // -------------------------
    // Benutzer hat den Bereich verlassen
    // -------------------------
    case USER_AWAY:

    resetLEDs();

    if (millis() - servoTimer >= 2000)
    {
        servoTimer = millis();
        cute.play(S_SAD);
        delay(2000);
        servoPos = !servoPos;

        if (servoPos)
            ServomotorRed.write(170);
        else
            ServomotorRed.write(10);
    }

    if (millis() - awayStartTime >= 10000)
    {
        ServomotorRed.write(10);   // Servo am Ende unten lassen
        currentState = WAIT_FOR_INPUT;
    }

    break;
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

    // HC-SR04 braucht etwas Zeit bis zur nächsten Messung
    delay(600);
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
}
