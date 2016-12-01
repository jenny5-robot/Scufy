#include "potentiometer.h"
#include "Arduino.h"
//--------------------------------------------------------------------
t_potentiometer_controller::t_potentiometer_controller(void)
{
  pin = 2;
}
//--------------------------------------------------------------------
void t_potentiometer_controller::set_params (byte _pin)
{
		pin = _pin;
}
//--------------------------------------------------------------------
int	t_potentiometer_controller::get_position(void)
{
	return (int)analogRead(pin);
}
//--------------------------------------------------------------------
void t_potentiometer_controller::get_params(byte *_pin)
{
  *_pin = pin;
}
//--------------------------------------------------------------------
/*
byte t_potentiometer_controller::is_within_limits(void)
{
	int val = get_position();

	return low <= val && val <= high;
}
//--------------------------------------------------------------------
bool t_potentiometer_controller::is_lower_bound_reached(void)
{
  int val = get_position();
  return low > val;
}
//--------------------------------------------------------------------
bool t_potentiometer_controller::is_upper_bound_reached(void)
{
  int val = get_position();
  return high < val;
}
//--------------------------------------------------------------------
int8_t t_potentiometer_controller::get_direction(void)
{
  return _direction;
}
//--------------------------------------------------------------------
int t_potentiometer_controller::get_home_position(void)
{
  return _home;
}
//--------------------------------------------------------------------
*/