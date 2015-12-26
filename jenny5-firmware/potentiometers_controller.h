#ifndef POTENTIOMETERS_CONTROLLER_H_
#define POTENTIOMETERS_CONTROLLER_H_

#include "Arduino.h"

struct t_limit_pair
{
	int low;
	int high;
  int _home; // where it should be when we start
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
	int get_position(byte potentiometer_index);
	void set_limits(byte index, int low, int high, int _home);
	void get_limits(byte index, int *low, int *high, int *_home);
	int isWithinLimits(byte potentiometer_index);
};

#endif
