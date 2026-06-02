int ledPin1 =  2; // the number of the LED pin
int ledPin2 =  3;
int ledPin3 =  4;
int ledPin4 =  5;
int ledPin5 =  6;

int red = 11;
int green = 12;
int blue = 13;

enum STATE {
  
  Wait_For_Input,

  User_Infront,

  User_Away,

  End,
};

STATE currentState = Wait_For_Input;

unsigned long Phase1 = 15000; // milliseconds 
unsigned long Phase2 = 30000; // milliseconds 
unsigned long Phase3 = 45000; // milliseconds 
unsigned long Phase4 = 60000; // milliseconds 
unsigned long Phase5 = 75000; // milliseconds 
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
   switch (currentState) { 
      case Wait_For_Input:
          break;
      case User_Infront:

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
          break;

      case User_Away:
          break;
      case End:
          break;

   }
 
}
