#include "Ultrasonic.h"

//-------------------------------------------------------
t_ultrasonic::t_ultrasonic(void)
{
  Trig_pin = 2;
  Echo_pin = 3;

  sonar = NULL;
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
}
//-------------------------------------------------------
void t_ultrasonic::get_sensor_pins(byte *trig_pin, byte *echo_pin)
{
  *trig_pin = Trig_pin;
  *echo_pin = Echo_pin;
}
//-------------------------------------------------------
