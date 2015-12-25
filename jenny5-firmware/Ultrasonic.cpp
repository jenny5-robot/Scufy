#include "Ultrasonic.h"

t_ultrasonic::t_ultrasonic(int _TP, int _EP)
{
   pinMode(_TP, OUTPUT);
   pinMode(_EP, INPUT);
   Trig_pin=_TP;
   Echo_pin=_EP;
}

long t_ultrasonic::Timing()
{
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin, LOW);
  duration = pulseIn(Echo_pin,HIGH);
  return duration;
}

long t_ultrasonic::Ranging()
{
  Timing();
  distance_cm = duration / 29 / 2 ;
  return distance_cm;
}
