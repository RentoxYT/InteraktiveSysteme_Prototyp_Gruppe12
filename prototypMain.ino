int ledPin1 =  A2; // the number of the LED pin
int ledPin2 =  A3;
int ledPin3 =  A4;
int ledPin4 =  A5;
int ledPin5 =  A6;

unsigned long Phase1 = 15000; // milliseconds of on-time
unsigned long Phase2 = 30000; // milliseconds of off-time
unsigned long Phase3 = 45000; // milliseconds of off-time
unsigned long Phase4 = 60000; // milliseconds of off-time
unsigned long Phase5 = 75000; // milliseconds of off-time
void setup()
{
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
  // check to see if it's time to change the state of the LED
  unsigned long currentMillis = millis();

  if(currentMillis >= Phase1)
  {
   
    digitalWrite(ledPin1, HIGH); // Update the actual LED
  }
  if(currentMillis >= Phase2)
  {
    digitalWrite(ledPin2, HIGH); // Update the actual LED
  }
   if(currentMillis >= Phase3)
  {
    digitalWrite(ledPin3, HIGH); // Update the actual LED
  }
   if(currentMillis >= Phase4)
  {
    digitalWrite(ledPin4, HIGH); // Update the actual LED
  }
   if(currentMillis >= Phase5)
  {
    digitalWrite(ledPin5, HIGH); // Update the actual LED
  }
}
