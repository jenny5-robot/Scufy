#include "Ultrasonic.h"

//-------------------------------------------------------
t_ultrasonic::t_ultrasonic(void)
{
  Trig_pin = 2;
  Echo_pin = 3;

  sonar = NULL;

  last_read_distance = -1;
}
//-------------------------------------------------------
t_ultrasonic::~t_ultrasonic(void)
{
  if (sonar){
    delete[] sonar;  
    sonar = NULL;
  }
}
//-------------------------------------------------------
void t_ultrasonic::create_init(byte trigger_pin, byte echo_pin)
{
   Trig_pin = trigger_pin;
   Echo_pin = echo_pin;
   if (sonar)
     delete sonar;
     
   sonar = new NewPing(trigger_pin, echo_pin, 200);
   last_read_distance = -1;
}
//-------------------------------------------------------
void t_ultrasonic::get_sensor_pins(byte *trig_pin, byte *echo_pin)
{
  *trig_pin = Trig_pin;
  *echo_pin = Echo_pin;
}
//-------------------------------------------------------
void t_ultrasonic::trigger(void (*echoCheck)(void))
{
  last_read_distance = -1;
  sonar->ping_timer(echoCheck);
}
//-------------------------------------------------------
int t_ultrasonic::get_last_read_distance(void)
{
  // cannot be read twice !
  int tmp_value = last_read_distance;
  last_read_distance = -1;
  return tmp_value;
}
//-------------------------------------------------------
void t_ultrasonic::set_distance(int new_computed_distance)
{
  last_read_distance = new_computed_distance;
}
//-------------------------------------------------------
