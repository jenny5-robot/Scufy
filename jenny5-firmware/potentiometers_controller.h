#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

#include "Potentiometer.h"

class t_potentiometers_controller
{
private:
	potentiometer 	**potentiometers;

public:
	t_potentiometers_controller(byte num_potentiometers, byte *potentiometer_pins);
	int getPotentiometerValue(byte potentiometer_index);
};

#endif
