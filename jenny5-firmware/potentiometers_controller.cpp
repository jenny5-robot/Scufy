#include "potentiometers_controller.h"

t_potentiometers_controller::t_potentiometers_controller(byte num_potentiometers, byte *potentiometers_pins)
{
	potentiometers = new Potentiometer*[num_potentiometers];

	for (byte i = 0; i < num_potentiometers; ++i)
		potentiometers[i] = new Potentiometer(potentiometers_pins[i]);
}

int	t_potentiometers_controller::getPotentiometerValue(byte potentiometer_index)
{
	return (int)potentiometers[potentiometer_index];
}
