// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef servos_controller_H
#define servos_controller_H

#include <Arduino.h>
#include <Servo.h>

class t_servos_controller
{
private:
	byte num_motors;
	Servo *servos;
public:
	t_servos_controller();
	void delete_memory(void);
	void set_params(int sensor_index, byte _pin);
	void set_num_servos(byte new_num_servos);
	byte get_num_servos(void);
	byte get_position(byte servo_index);
	void set_position(byte servo_index, byte position);
};

#endif
