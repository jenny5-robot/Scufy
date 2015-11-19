#include "PotentiometersController.h"

PotentiometersController::PotentiometersController(byte *potentiometers_pins, byte num_potentiometers)
{
	potentiometer = new Potentiometer*[num_potentiometers];

	for (byte i = 0; i < num_potentiometers; ++i)
		potentiometer[i] = new Potentiometer(potentiometers_pins[i]);
}

int	PotentiometersController::getPotentiometerValue(byte potentiometer_index)
{
	return (int)potentiometer[potentiometer_index];
}