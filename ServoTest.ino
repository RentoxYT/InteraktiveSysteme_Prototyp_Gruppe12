#include <Servo.h>

Servo ServomotorRed;
Servo ServomotorGreen;
int ledPin1 =  7;
// Pin für den Sender
#define PIN_TRIGGER 3
#define PIN_ECHO    2
const int SENSOR_MAX_RANGE = 300; // in cm
unsigned long duration;
unsigned int distance;

void setup()
{
    ServomotorGreen.attach(10);
    ServomotorRed.attach(9);
    Serial.begin(9600);
     pinMode(ledPin1, OUTPUT);
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void loop()
{
   digitalWrite(ledPin1, HIGH);
     digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(PIN_ECHO, HIGH);
  distance = duration/58;


    Serial.println("Distance to object: " + String(distance) + " cm");
  
  if(distance > 70) {
     ServomotorRed.write(170);  // oben
     delay(2000);
     ServomotorRed.write(10); 

  }

  delay(1000);



  
/*
    ServomotorRed.write(10);    // unten
    ServomotorGreen.write(170);
    delay(3000);

    ServomotorRed.write(170);  // oben
    ServomotorGreen.write(10);
    delay(3000);*/
}
