// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef buttons_controller_H
#define buttons_controller_H

#include <Arduino.h>

class t_buttons_controller
{
	private:
		byte num_sensors;
		byte *buttons_pins;
	public:

		t_buttons_controller();
		void set_params(int sensor_index, byte _pin);
		void get_params(int sensor_index, byte *_pin);
		void set_num_sensors(byte new_num_sensors);
		byte get_num_sensors(void);
		bool get_state(byte button_index);
};

#endif
