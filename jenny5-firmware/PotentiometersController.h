#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

#include "Potentiometer.h"

class PotentiometersController
{
private:
	Potentiometer 	**potentiometer;

public:
	PotentiometersController(byte *potentiometer_pins, byte num_potentiometers);
	int getPotentiometerValue(byte potentiometer_index);
};

#endif
