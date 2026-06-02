#include <CapacitiveSensor.h>

int ledPin1 =  2; // the number of the LED pin
int ledPin2 =  3;
int ledPin3 =  4;
int ledPin4 =  5;
int ledPin5 =  6;

int red = 11;
int green = 12;
int blue = 13;
unsigned long startTime = 0;
CapacitiveSensor   cs_7_8 = CapacitiveSensor(7,8);

enum STATE {
  
  WAIT_FOR_INPUT,

  USER_INFRONT,

  USER_AWAY,

  END,
};

STATE currentState = WAIT_FOR_INPUT;

unsigned long Phase1 = 2000; // milliseconds 
unsigned long Phase2 = 4000; // milliseconds 
unsigned long Phase3 = 8000; // milliseconds 
unsigned long Phase4 = 10000; // milliseconds 
unsigned long Phase5 = 12000; // milliseconds 


void setup()
{
   Serial.begin(9600);
   cs_7_8.set_CS_AutocaL_Millis(0xFFFFFFFF); 
  // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
}

void loop()
{


   switch (currentState) { 
      case WAIT_FOR_INPUT:
          resetLEDs();
          long sensorValue = cs_7_8.capacitiveSensor(30);
          Serial.print("TouchSensor: ");
          Serial.println(sensorValue);

         if(sensorValue > 0)
          {
              startTime = millis();
              currentState = USER_INFRONT;
          }
          break;
          
          
           
      case USER_INFRONT:

          // check to see if it's time to change the state of the LED
          unsigned long elapsed = millis() - startTime;


          if(elapsed >= Phase1)
          {
          
            digitalWrite(ledPin1, HIGH); // Update the actual LED
          }
          if(elapsed >= Phase2)
          {
            digitalWrite(ledPin2, HIGH); // Update the actual LED
          }
          if(elapsed >= Phase3)
          {
            digitalWrite(ledPin3, HIGH); // Update the actual LED
          }
          if(elapsed >= Phase4)
          {
            digitalWrite(ledPin4, HIGH); // Update the actual LED
          }
          if(elapsed >= Phase5)
          {
            digitalWrite(ledPin5, HIGH); // Update the actual LED
            currentState = END;
          }
          break;

      case USER_AWAY:
          break;
      case END:
          break;

   }


 
}

   void resetLEDs()
{
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
}
