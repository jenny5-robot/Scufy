#ifndef POTENTIOMETER_CONTROLLER_H
#define POTENTIOMETER_CONTROLLER_H

#include "Arduino.h"

class t_potentiometer_controller
{
private:
  byte pin;
  
	int low;
  int high;
  int _home; // where it should be when we start

  int8_t _direction;// 1 - same direction with motor, -1 - different direction

public:
	t_potentiometer_controller();
	void set_params(byte _pin, int _low, int _high, int _home, int8_t _direction);
  void get_params(byte *_pin, int *low, int *high, int *_home, int8_t *_direction);
 
	int get_position(void);	
  int8_t get_direction(void);
  int get_home_position(void);
	byte is_within_limits(void);
  bool is_lower_bound_reached(void);
  bool is_upper_bound_reached(void);
};

#endif
