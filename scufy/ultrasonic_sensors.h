// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------
#ifndef T_ULTRASONIC_SENSORS_CONTROLLER_H
#define T_ULTRASONIC_SENSORS_CONTROLLER_H

#include "Arduino.h"
#include "ultrasonic.h"
 
class t_ultrasonic_sensors_controller
{
public:
	t_ultrasonic *sensors;

	byte num_sensors;

public:
	t_ultrasonic_sensors_controller (void);
 
  void set_num_sensors(byte new_num_sensors);
  void set_sensor_pins(byte sensor_index, byte trig_pins, byte echo_pins);
  void get_sensor_pins(byte sensor_index, byte *trig_pins, byte *echo_pins);
  
  byte get_num_sensors(void);
	void trigger (byte sensor_index);

  void update_results(char *serial_out);
};

extern t_ultrasonic_sensors_controller ultrasonic_sensors_controller;

#endif //T_ULTRASONIC_SENSORS_CONTROLLER_H
