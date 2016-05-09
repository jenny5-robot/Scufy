#include "infrared_controller.h"


//--------------------------------------------------------------------------------------------
t_infrared_sensor::t_infrared_sensor(void) 
{
  min_pos = 60;
  max_pos = 900;
  home_pos = 500;
  pin = 2;
}
//--------------------------------------------------------------------------------------------
int t_infrared_sensor::get_signal_strength(void)
{
  	return analogRead(pin);
}
//--------------------------------------------------------------------------------------------
void t_infrared_sensor::set_params(byte _pin, int _min_pos, int _max_pos, int _home_pos)
{
    pin = _pin;
    min_pos = _min_pos;
    max_pos = _max_pos;
    home_pos = _home_pos;
}
//--------------------------------------------------------------------------------------------
void t_infrared_sensor::get_params(byte *_pin, int *_min_pos, int *_max_pos, int *_home_pos)
{
    *_pin = pin;
    *_min_pos = min_pos;
    *_max_pos = max_pos;
    *_home_pos = home_pos;
}
//--------------------------------------------------------------------------------------------
byte t_infrared_sensor::is_within_limits(void)
{
  int pos = get_signal_strength();

  return min_pos <= pos && max_pos >= pos; 
}
//--------------------------------------------------------------------------------------------
int t_infrared_sensor::get_home(void)
{
  return home_pos;
}

