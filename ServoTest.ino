#include <Servo.h>

Servo Servomotor;

void setup()
{
    Servomotor.attach(10);
}

void loop()
{
    Servomotor.write(90);    // unten
    delay(3000);

    Servomotor.write(180);  // oben
    delay(3000);
}