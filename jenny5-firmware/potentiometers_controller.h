#ifndef POTENTIOMETER_CONTROLLER_H_
#define POTENTIOMETER_CONTROLLER_H_

#include "Potentiometer.h"

class t_potentiometers_controller
{
private:
	Potentiometer 	**potentiometers;

public:
	t_potentiometers_controller(byte num_potentiometers, byte *potentiometer_pins);
	int getPotentiometerValue(byte potentiometer_index);
};

#endif
