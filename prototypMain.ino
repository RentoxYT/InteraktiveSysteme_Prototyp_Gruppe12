#include <Servo.h>  //Bibliothek Servomotor
#include <CuteBuzzerSounds.h>  //Bibliothek BuzzerSound

// LEDs für den Timer
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;
const int ledPin4 = 5;
const int ledPin5 = 6;

// Ultraschall
#define PIN_TRIGGER 9
#define PIN_ECHO     10

// Buzzer und Button
#define BUZZER_PIN 12
#define BUTTON_PIN 13

//RGB-LED
int LEDrot = A1;
int LEDgruen = A2;
int LEDblau = A3;
int Helligkeit = 255;  // Zahlenwert zwischen 0 und 255 – gibt die Leuchtstärke der einzelnen Farbe an

//Maximale Messdistanz
const int SENSOR_MAX_RANGE = 300;

// Servos
Servo ServomotorRed;
Servo ServomotorGreen;

// Zustände (state-machine)
enum STATE
{
    WAIT_FOR_INPUT,
    USER_INFRONT,
    USER_AWAY,
    FINAL
};

STATE currentState = WAIT_FOR_INPUT;

// Zeitsteuerung
const unsigned long Phase1 = 5000;
const unsigned long Phase2 = 10000;
const unsigned long Phase3 = 15000;
const unsigned long Phase4 = 20000;
const unsigned long Phase5 = 25000;

//Zeitvariablen + Distanz
unsigned long awayStart = 0;
unsigned long returnStart = 0;
unsigned long startTime = 0;
unsigned long lastMeasure = 0;
unsigned long durationTime = 0;
unsigned int distance = SENSOR_MAX_RANGE + 1;

// ======================================================

void setup()
{
    Serial.begin(9600);
    
    //Buzzer initialisieren
    cute.init(BUZZER_PIN);
    
    //Servos anschließen
    ServomotorGreen.attach(7);
    ServomotorRed.attach(8);
    
    //Servomotor Startposition
    ServomotorGreen.write(170);
    ServomotorRed.write(80);
    
    //RGB-Pins Ausgänge
    pinMode(LEDblau, OUTPUT);
    pinMode(LEDgruen, OUTPUT);
    pinMode(LEDrot, OUTPUT);

    //Ultraschallsensor
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    //Timer LED
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);

    //Button
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    //LEDS auschalten/Reseten
    resetLEDs();
    digitalWrite(PIN_TRIGGER, LOW);
}

// ======================================================

void loop()
{
    //Abstand messen
    measureDistance();
    
    //Alle 50ms erneut messen
     if (millis() - lastMeasure >= 50)
    {
        lastMeasure = millis();
        measureDistance();
    }
    switch (currentState)
    {

    // Zustand: Warten auf Eingabe vom Benutzer
    case WAIT_FOR_INPUT:
{
    resetLEDs();

    //Benutzer drück den Button und ist näher als 100cm
    if (digitalRead(BUTTON_PIN) == LOW && distance < 100) 
    {
        Serial.println("Benutzer erkannt.");
        
        //Beginne Timer
        startTime = millis();

        //Wechsel in USER_INFRONT CASE
        currentState = USER_INFRONT;
    }

        break;
}

    // Benutzer wird erkannt

   case USER_INFRONT:
{
    //Wenn der Benutzer sich mehr als 100cm entfernt
    if (distance >= 100)
    {
        if (awayStart == 0)
        {
            awayStart = millis(); // Zeitpunkt merken
        }

        if (millis() - awayStart >= 200) // 2 Sekunden weg
        {
            Serial.println("Benutzer weg.");

            //Servos zurücksetzen
            ServomotorGreen.write(170);
            ServomotorRed.write(80);

            //Sound abspielen
            cute.play(S_CONFUSED);

            //In den USER_AWAY Case wechseln
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

    //Vergangene Zeit seit Start
    unsigned long elapsed = millis() - startTime;

    //TimerAnzeige mit den LEDs
    if (elapsed >= Phase1) digitalWrite(ledPin5, HIGH);
    if (elapsed >= Phase2) digitalWrite(ledPin4, HIGH);
    if (elapsed >= Phase3) digitalWrite(ledPin3, HIGH);
    if (elapsed >= Phase4) digitalWrite(ledPin2, HIGH);

    //Nach 25sek (Demo) fertig
    if (elapsed >= Phase5)
    {
        //Alle Lichter aktivieren, Happy Sound abspielen und in den Final State wechseln
        digitalWrite(ledPin1, HIGH);
        cute.play(S_HAPPY);
        Serial.println("Test Vor Final");
        currentState = FINAL;
        Serial.println(currentState);
        Serial.println("Test Nach Final");
        break;
    }

    break;
}


    // Benutzer hat den Bereich verlassen

case USER_AWAY:
{

    //Roter Servo bewegt seine Flagge
     ServomotorRed.write(170);
    
    resetLEDs();
    analogWrite(LEDgruen, 0); // Grün ausschalten
    analogWrite(LEDrot, Helligkeit); //Rot ein

    //Benutzer kommt zurück
    if (distance < 100)
    {
        if (returnStart == 0)
        {
            // Zeitpunkt merken, wann der Benutzer zurückgekommen ist
            returnStart = millis();
        }

        // Benutzer war 5 Sekunden durchgehend da
        if (millis() - returnStart >= 500)
        {
            //Alles zurücksetzen und von vorne beginnen
            startTime = millis();
            returnStart = 0;
            ServomotorGreen.write(170);
            ServomotorRed.write(80);
            analogWrite(LEDgruen, 0); // Grün aus
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

        //Benutzer hat die Zeit erfolgreich absolviert
case FINAL:
{
    //Grünen Servo Flagge bewegen, alle LEDs anlassen
    ServomotorGreen.write(75);
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledPin5, HIGH);

  
    analogWrite(LEDgruen, Helligkeit); // Grün einschalten
     analogWrite(LEDrot, 0); //Rot aus

    //Reset durch Buttonpress
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        //Warten bis der Button losgelasen wird
        while(digitalRead(BUTTON_PIN) == LOW) {
          delay(10);
        }

        //Alles zurücksetzen und in den Startzustand wechseln
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


// Abstand messen
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
        //Umrechnung in cm
        distance = durationTime / 58;
    }

    //Ausgabe Serial Monitor
    Serial.print("Distance to object: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println(currentState); 
   
    
}


// LEDs zurücksetzen
void resetLEDs()
{
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
    analogWrite(LEDgruen, 0); //Grün aus
    analogWrite(LEDrot, 0); //Rot aus

}
