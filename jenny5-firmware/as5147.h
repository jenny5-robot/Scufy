#ifndef AS5147_CONTROLLER_H
#define AS5147_CONTROLLER_H

#include "Arduino.h"
#include <SPI.h>

#define SO 23
#define SI 22
#define CLK 21


class t_as5147_controller
{
private:
	byte pin;
public:
	t_as5147_controller();
	void set_params(byte _pin);
	void get_params(byte *_pin);

	int get_position(void);
	int8_t get_direction(void);
	/*
	int get_home_position(void);
	byte is_within_limits(void);
	bool is_lower_bound_reached(void);
	bool is_upper_bound_reached(void);
	*/
};

#endif
