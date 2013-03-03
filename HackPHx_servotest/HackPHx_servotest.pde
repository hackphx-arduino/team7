#include <Servo.h> 

Servo myservo;
Servo myservo2;
Servo myservo3;

const int sw1_pin = 33;
int sw1_state = 0;
const int sw2_pin = 34;
int sw2_state = 0;
const int sw3_pin = 35;
int sw3_state = 0;



void setup()
{
 myservo.attach(30); 
 pinMode(sw1_pin, INPUT); 
 myservo.attach(29); 
 pinMode(sw2_pin, INPUT); 
 myservo.attach(28); 
 pinMode(sw3_pin, INPUT); 
  
}

void loop()
{
  sw1_state = digitalRead(sw1_pin);
  sw2_state = digitalRead(sw2_pin);
  sw3_state = digitalRead(sw3_pin);
  
  if (sw1_state == HIGH){
   myservo.write(100);
   delay(2500);
   myservo.write(50);
  }
  
  if (sw2_state == HIGH){
   myservo2.write(100);
   delay(2500);
   myservo3.write(50);
  }
  
  if (sw3_state == HIGH){
   myservo2.write(100);
   delay(2500);
   myservo3.write(50);
  }
  
  
}
