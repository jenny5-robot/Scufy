#include "potentiometer.h"
#include "Arduino.h"
//--------------------------------------------------------------------
t_potentiometer_controller::t_potentiometer_controller(void)
{
  pin = 2;
  low = 2;
  high = 2;
  _home = 2;
  _direction = 1;
}
//--------------------------------------------------------------------
void t_potentiometer_controller::set_params (byte _pin, int _low, int _high, int __home, unsigned char __direction)
{
		pin = _pin;
		low = _low;
   high = _high;
   _home = __home;
   _direction = __direction;
}
//--------------------------------------------------------------------
int	t_potentiometer_controller::get_position(void)
{
	return (int)analogRead(pin);
}
//--------------------------------------------------------------------
void t_potentiometer_controller::get_params(byte *_pin, int *_low, int *_high, int *__home, unsigned char *__direction)
{
	*_low = low;
  *_high = high;
  *__home = _home;
  *_pin = pin;
  *__direction =_direction;
}
//--------------------------------------------------------------------
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
unsigned char t_potentiometer_controller::get_direction(void) 
{
  return _direction;
}
//--------------------------------------------------------------------
int t_potentiometer_controller::get_home_position(void)
{
  return _home;
}
//--------------------------------------------------------------------
