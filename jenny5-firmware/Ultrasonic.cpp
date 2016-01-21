#include "Ultrasonic.h"

//-------------------------------------------------------
t_ultrasonic::t_ultrasonic(int trigger_pin, int echo_pin)
{
   pinMode(trigger_pin, OUTPUT);
   pinMode(echo_pin, INPUT);
   Trig_pin=trigger_pin;
   Echo_pin=echo_pin;
}
//-------------------------------------------------------
unsigned long t_ultrasonic::trigger(void)
{
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin, LOW);
  unsigned long duration = pulseIn(Echo_pin, HIGH, 20);// wait for no more than 20 miliseconds ... otherwise I block the program too much
  return duration;
}
//-------------------------------------------------------
unsigned long t_ultrasonic::get_distance(void)
{
  unsigned long duration = trigger();
  unsigned long distance_cm = duration / 29 / 2 ;
  return distance_cm;
}
//-------------------------------------------------------
