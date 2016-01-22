#ifndef POTENTIOMETER_CONTROLLER_H_
#define POTENTIOMETER_CONTROLLER_H_

#include "Arduino.h"

struct t_limit_pair
{
};

class t_potentiometer_controller
{
private:

	int low;
  int high;
  int _home; // where it should be when we start
	byte pin;

public:
	t_potentiometer_controller();
	void set_params(byte _pin, int _low, int _high, int _home);
  void get_params(byte *_pin, int *low, int *high, int *_home);
 
	int get_position(void);	
	int is_within_limits(void);
};

#endif
