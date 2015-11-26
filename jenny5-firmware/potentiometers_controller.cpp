#include "potentiometers_controller.h"
#include "Arduino.h"

t_potentiometers_controller::t_potentiometers_controller(byte num_potentiometers, byte *potentiometer_pins, t_limit_pair *limits)
{
	limits = new t_limit_pair[num_potentiometers];
	pins = new byte[num_potentiometers];

	for (byte i = 0; i < num_potentiometers; ++i)
	{
		pins[i] = potentiometer_pins[i];
		this->limits[i] = limits[i];
	}
}

int	t_potentiometers_controller::getPotentiometerValue(byte potentiometer_index)
{
	return (int)analogRead(this->pins[potentiometer_index]);
}


void t_potentiometers_controller::setPotentiometerLimit(byte index, int low, int high)
{
	this->limits[index].low = low;
	this->limits[index].high = high;
	
}


void t_potentiometers_controller::getPotentiometerLimit(byte index, t_limit_pair *limits)
{
	*limits = this->limits[index];
}

int t_potentiometers_controller::isWithinLimits(byte potentiometer_index)
{
	int val = getPotentiometerValue(potentiometer_index);

	return limits[potentiometer_index].low <= val && val <= limits[potentiometer_index].high;
}