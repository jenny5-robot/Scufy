// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#include "infrared_analog.h"

//--------------------------------------------------------------------------------------------
t_infrared_analog_sensor::t_infrared_analog_sensor(void) 
{
  pin = 2;
}
//--------------------------------------------------------------------------------------------
int t_infrared_analog_sensor::get_signal_strength(void)
{
  	return analogRead(pin);
}
//--------------------------------------------------------------------------------------------
void t_infrared_analog_sensor::set_params(byte _pin)
{
    pin = _pin;
}
//--------------------------------------------------------------------------------------------
void t_infrared_analog_sensor::get_params(byte *_pin)
{
    *_pin = pin;
}
//--------------------------------------------------------------------------------------------