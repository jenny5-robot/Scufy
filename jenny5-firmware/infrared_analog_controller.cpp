#include "infrared_analog_controller.h"


//--------------------------------------------------------------------------------------------
t_infrared_analog_sensor::t_infrared_analog_sensor(void) 
{
  pin = 2;
  
  min_pos = 60;
  max_pos = 900;
  home_pos = 500;
  
  _direction = 1;
}
//--------------------------------------------------------------------------------------------
int t_infrared_analog_sensor::get_signal_strength(void)
{
  	return analogRead(pin);
}
//--------------------------------------------------------------------------------------------
void t_infrared_analog_sensor::set_params(byte _pin, int _min_pos, int _max_pos, int _home_pos, int __direction)
{
    pin = _pin;
    min_pos = _min_pos;
    max_pos = _max_pos;
    home_pos = _home_pos;
    _direction = __direction;
}
//--------------------------------------------------------------------------------------------
void t_infrared_analog_sensor::get_params(byte *_pin, int *_min_pos, int *_max_pos, int *_home_pos, int *__direction)
{
    *_pin = pin;
    *_min_pos = min_pos;
    *_max_pos = max_pos;
    *_home_pos = home_pos;
    *__direction = _direction;
}
//--------------------------------------------------------------------------------------------
byte t_infrared_analog_sensor::is_within_limits(void)
{
  int pos = get_signal_strength();

  return min_pos <= pos && max_pos >= pos; 
}
//--------------------------------------------------------------------------------------------
int t_infrared_analog_sensor::get_home_position(void)
{
  return home_pos;
}
//--------------------------------------------------------------------------------------------
bool t_infrared_analog_sensor::is_lower_bound_reached(void)
{
  int val = get_signal_strength();
  return min_pos > val;
}
//--------------------------------------------------------------------
bool t_infrared_analog_sensor::is_upper_bound_reached(void)
{
  int val = get_signal_strength();
  return max_pos < val;
}
//--------------------------------------------------------------------
int t_infrared_analog_sensor::get_direction(void) 
{
  return _direction;
}
//--------------------------------------------------------------------
/*
bool t_infrared_sensor::is_going_home(void)
{
  return going_home;
}
//--------------------------------------------------------------------
void t_infrared_sensor::set_going_home(bool value)
{
  going_home = value;
}
//--------------------------------------------------------------------
*/
