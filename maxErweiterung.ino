int red = 11;
int green = 12;
int blue = 13;

int ledPin1 =  2; // the number of the LED pin
int ledPin2 =  3;
int ledPin3 =  4;
int ledPin4 =  5;
int ledPin5 =  6;

enum State {
  
  WAIT_FOR_INPUT,

  USER_INFRONT,

  USER_AWAY,

  END,
};

State currentState = USER_INFRONT;

int trigger = 9;
int echo = 10;
int cm = 1234;

unsigned long Phase1 = 2000; // milliseconds of on-time
unsigned long Phase2 = 4000; // milliseconds of off-time
unsigned long Phase3 = 6000; // milliseconds of off-time
unsigned long Phase4 = 8000; // milliseconds of off-time
unsigned long Phase5 = 10000; // milliseconds of off-time

void setup()
{
  pinMode(red, OUTPUT);
  // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);

  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
}

void loop() {

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

  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigger, LOW);
  cm = pulseIn(echo, HIGH) / 58;
  cm = (int(cm * 100)) / 100;

 if(cm <= 50){
    currentState = USER_INFRONT;
    Serial.print("In front ");
  }
  if(cm > 50){
    currentState = USER_AWAY;
    Serial.print("Away ");
  }

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  if(currentState == USER_AWAY){
    digitalWrite(red, HIGH);
  }
  if(currentState == USER_INFRONT){
    digitalWrite(red, LOW);
  }

  delay(1000);
}