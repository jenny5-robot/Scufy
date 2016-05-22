#include "Ultrasonic.h"

//-------------------------------------------------------
t_ultrasonic::t_ultrasonic(void)
{
  Trig_pin = 2;
  Echo_pin = 3;

  last_read_distance = -1;
  trigger_started = false;
}
//-------------------------------------------------------
void t_ultrasonic::create_init(byte trigger_pin, byte echo_pin)
{
  Trig_pin = trigger_pin;
  Echo_pin = echo_pin;

  last_read_distance = -1;
  micros_start = 0;

  pinMode(Trig_pin, OUTPUT);
  pinMode(Echo_pin, INPUT);
  trigger_started = false;
}
//-------------------------------------------------------
void t_ultrasonic::get_sensor_pins(byte *trig_pin, byte *echo_pin)
{
  *trig_pin = Trig_pin;
  *echo_pin = Echo_pin;
}
//-------------------------------------------------------
void t_ultrasonic::trigger(void)
{
  if (!trigger_started) {// don't call trigger again if the echo has not been received.
    last_read_distance = -1;

    digitalWrite(Trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig_pin, LOW);

    first_high_received = false;
    trigger_started = true;
  }
}
//-------------------------------------------------------
bool t_ultrasonic::check_echo(void)
{
  if (trigger_started) {
    int val = digitalRead(Echo_pin);
    if (!first_high_received) {
      if (val == HIGH) {
        first_high_received = true;
        micros_start = micros();
      }
    }
    else {
      unsigned long micros_now = micros();
      if (val == LOW) {
        first_high_received = false;
        last_read_distance = (micros_now - micros_start) / 58;
        if (last_read_distance > 500)
          last_read_distance = 0;
        trigger_started = false;
      }
      else{ // fail safe; do not wait if the echo does not go down in 100 miliseconds
        if (micros_now - micros_start > 100000){
          last_read_distance = 0;
          trigger_started = false;
          first_high_received = false;
        }
      }
    }
    return true;
  }
  else
    return false;
}
//-------------------------------------------------------
int t_ultrasonic::get_last_read_distance(void)
{
  return last_read_distance;
}
//-------------------------------------------------------
