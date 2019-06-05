// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef AS5147_S_CONTROLLER_H_
#define AS5147_S_CONTROLLER_H_

#include "Arduino.h"
#include "as5147.h"


class t_as5147s_controller
{
private:
	byte num_sensors;
	t_as5147_controller *sensors;

public:
	t_as5147s_controller();

	int get_position(byte sensor_index);
	//	int8_t get_direction(byte potentiometer_index);
	//  int get_home_position(byte potentiometer_index);
	void set_params(byte pot_index, byte pin);
	void get_params(byte pot_index, byte *pin);

	/*
	byte is_within_limits(byte potentiometer_index);
	byte is_lower_bound_reached(byte potentiometer_index);
	byte is_upper_bound_reached(byte potentiometer_index);
	*/
	void set_num_sensors(byte new_num_sensors);
	byte get_num_sensors(void);
};

#endif
