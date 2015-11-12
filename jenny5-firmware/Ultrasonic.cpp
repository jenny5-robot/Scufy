#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int TP, int EP)
{
   pinMode(TP,OUTPUT);
   pinMode(EP,INPUT);
   Trig_pin=TP;
   Echo_pin=EP;
}

long Ultrasonic::Timing()
{
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin, LOW);
  duration = pulseIn(Echo_pin,HIGH);
  return duration;
}

long Ultrasonic::Ranging()
{
  Timing();
  distacne_cm = duration /29 / 2 ;
  return distacne_cm;
}

// Test program.
// Ultrasonic ultrasonic(53,52);

// void setup() {
//   Serial.begin(9600);
//   Serial.print("HC-SR4 testing..");
// delay(1000);

// }

// void loop()
// {
//   //lcd.clear();


//   Serial.print(ultrasonic.Ranging());
//   Serial.print(" cm.\n");

//   delay(100);
// }