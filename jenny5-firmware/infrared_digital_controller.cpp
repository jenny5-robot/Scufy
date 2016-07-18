#include "infrared_digital_controller.h"


//--------------------------------------------------------------------------------------------
t_infrared_digital_sensor::t_infrared_digital_sensor(void) 
{
  pin = 2;
}
//--------------------------------------------------------------------------------------------
int t_infrared_digital_sensor::get_value(void)
{
  	return digitalRead(pin);
}
//--------------------------------------------------------------------------------------------
void t_infrared_digital_sensor::set_params(byte _pin)
{
    pin = _pin;
}
//--------------------------------------------------------------------------------------------
void t_infrared_digital_sensor::get_params(byte *_pin)
{
    *_pin = pin;
}
//--------------------------------------------------------------------------------------------