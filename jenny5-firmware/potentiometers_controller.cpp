#include "potentiometers_controller.h"
#include "Arduino.h"
//--------------------------------------------------------------------
t_potentiometers_controller::t_potentiometers_controller(byte num_potentiometers, byte *potentiometer_pins, t_limit_pair *limits)
{
	this->limits = new t_limit_pair[num_potentiometers];
	pins = new byte[num_potentiometers];

	for (byte i = 0; i < num_potentiometers; ++i)
	{
		pins[i] = potentiometer_pins[i];
		this->limits[i] = limits[i];
	}
}
//--------------------------------------------------------------------
int	t_potentiometers_controller::get_position(byte potentiometer_index)
{
	return (int)analogRead(this->pins[potentiometer_index]);
}
//--------------------------------------------------------------------
void t_potentiometers_controller::set_limits(byte potentiometer_index, int low, int high, int _home)
{
	limits[potentiometer_index].low = low;
	limits[potentiometer_index].high = high;
  limits[potentiometer_index]._home = _home;
}
//--------------------------------------------------------------------
void t_potentiometers_controller::get_limits(byte potentiometer_index, int *low, int *high, int *_home)
{
	*low = this->limits[potentiometer_index].low;
  *high = limits[potentiometer_index].high;
  *_home = limits[potentiometer_index]._home;
}
//--------------------------------------------------------------------
int t_potentiometers_controller::isWithinLimits(byte potentiometer_index)
{
	int val = get_position(potentiometer_index);

	return limits[potentiometer_index].low <= val && val <= limits[potentiometer_index].high;
}
//--------------------------------------------------------------------
