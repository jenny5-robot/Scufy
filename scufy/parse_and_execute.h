#ifndef PARSE_AND_EXECUTE_H
#define PARSE_AND_EXECUTE_H

#include <Wire.h>
#include "config.h"

#include "ultrasonic_sensors.h"
#ifdef USE_POTENTIOMETERS
#include "potentiometers.h"
#endif
#include "as5147s.h"
#include "servos.h"
#include "stepper_motors.h"
#include "buttons.h"
#include "jenny5_types.h"
#include "infrared_analog_sensors.h"
#include "tera_ranger_one.h"
#ifdef USE_TB6612FNG
#include "dc_motors_TB6612FNG.h"
#endif
#include "tera_ranger_one_lidar.h"
#include "parse_and_execute.h"

#include "memoryfree.h"


void parse_and_execute_commands(char* tmp_str, byte str_length, char* serial_out, 
	t_steppers_controller &steppers_controller,
	t_servos_controller servos_controller,

	#ifdef USE_TB6612FNG
	t_dc_motors_controller_TB6612FNG &dc_motors_controller_TB6612FNG,
	#endif

	#ifdef USE_POTENTIOMETERS
	t_potentiometers_controller &potentiometers_controller,
	#endif
	t_as5147s_controller &as5147s_controller,
	t_ultrasonic_sensors_controller &ultrasonic_sensors_controller,
	t_infrared_analog_sensors_controller &infrared_analog_sensors_controller,
	t_buttons_controller &buttons_controller,
	t_tera_ranger_one_controller &tera_ranger_one_controller,
	t_tera_ranger_one_lidar* tera_ranger_one_lidar,
	const char* firmware_version
);


#endif