#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int _TP, int _EP)
{
   pinMode(_TP,OUTPUT);
   pinMode(_EP,INPUT);
   Trig_pin=_TP;
   Echo_pin=_EP;
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
  distacne_cm = duration / 29 / 2 ;
  return distacne_cm;
}