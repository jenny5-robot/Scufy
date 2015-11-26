#ifndef POTENTIOMETERS_CONTROLLER_H_
#define POTENTIOMETERS_CONTROLLER_H_

#include "Arduino.h"

struct t_limit_pair
{
	int low;
	int high;
};

class t_potentiometers_controller
{
private:
	//potentiometer 	**potentiometers;
	int					count;
	t_limit_pair		*limits;
	byte				*pins;

public:
	t_potentiometers_controller(byte num_potentiometers, byte *potentiometer_pins, t_limit_pair *limits);
	int getPotentiometerValue(byte potentiometer_index);
	void setPotentiometerLimit(byte index, int low, int high);
	void getPotentiometerLimit(byte index, t_limit_pair *limits);
	int isWithinLimits(byte potentiometer_index);
};

#endif
