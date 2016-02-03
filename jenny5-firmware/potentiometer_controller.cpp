#include "potentiometer_controller.h"
#include "Arduino.h"
//--------------------------------------------------------------------
t_potentiometer_controller::t_potentiometer_controller(void)
{
  pin = 2;
  low = 2;
  high = 2;
  _home = 2;
}
//--------------------------------------------------------------------
void t_potentiometer_controller::set_params (byte _pin, int _low, int _high, int __home)
{
		pin = _pin;
		low = _low;
   high = _high;
   _home = __home;
}
//--------------------------------------------------------------------
int	t_potentiometer_controller::get_position(void)
{
	return (int)analogRead(pin);
}
//--------------------------------------------------------------------
void t_potentiometer_controller::get_params(byte *_pin, int *_low, int *_high, int *__home)
{
	*_low = low;
  *_high = high;
  *__home = _home;
  *_pin = pin;
}
//--------------------------------------------------------------------
byte t_potentiometer_controller::is_within_limits(void)
{
	int val = get_position();

	return low <= val && val <= high;
}
//--------------------------------------------------------------------
byte t_potentiometer_controller::is_lower_bound_reached(void)
{
  int val = get_position();
  return low > val;
}
//--------------------------------------------------------------------
byte t_potentiometer_controller::is_upper_bound_reached(void)
{
  int val = get_position();
  return high < val;
}
