// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------


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

#include "memoryfree.h"

t_steppers_controller steppers_controller;
t_servos_controller servos_controller;

#ifdef USE_TB6612FNG
t_dc_motors_controller_TB6612FNG dc_motors_controller_TB6612FNG;
#endif

#ifdef USE_POTENTIOMETERS
t_potentiometers_controller potentiometers_controller;
#endif
t_as5147s_controller as5147s_controller;
t_ultrasonic_sensors_controller ultrasonic_sensors_controller;
t_infrared_analog_sensors_controller infrared_analog_sensors_controller;
t_buttons_controller buttons_controller;
t_tera_ranger_one_controller tera_ranger_one_controller;
t_tera_ranger_one_lidar *tera_ranger_one_lidar;

char is_command_running;

const char* firmware_version = "2019.05.01.0"; // year.month.day.build number

#define MAX_BUFFER_LENGTH 65

char current_buffer[MAX_BUFFER_LENGTH];

//#define DEBUG
//#define PRINT_HELP

//unsigned long time;

bool first_start;

//--------------------------------------------------------------------------------------------
void setup()
{
	first_start = 0;
	
	current_buffer[0] = 0;

	tera_ranger_one_lidar = NULL;

	Serial.begin(56700); //Open Serial connection
	/*
	while (!Serial);
	while (Serial.available() > 0) 
		char t = Serial.read();

	Serial.write("T#");// initialization is over; must check for T# string (which is the alive test)
	Serial.flush();
	*/
  /*
	Serial.write("Commands are:");
	Serial.println(F("T# // test connection. Returns T#."));
	Serial.println(F("SMx y# // Moves stepper motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement. Outputs SMx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go."));
	Serial.println(F("MDx y# // Moves DC motor x for y miliseconds. Outputs MDx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the time to go."));
	Serial.println(F("MVx y# // Moves servo motor x for y steps."));
	Serial.println(F("HSx# // Moves stepper motor x to home position. The first sensor in the list of sensors will establish the home position. The motor does nothing if no sensor is attached. Returns HSx#."));
	Serial.println(F("HDx# // Moves DC motor x to home position. The first sensor in the list of sensors must be the button which establish the home position. The motor does nothing if no sensor is attached. Returns HDx#."));
	Serial.println(F("HVx# // Moves servo motor x to home position.  Returns HVx#."));

	Serial.println(F("SDx#  // Disables stepper motor x. Outputs SDx# when done."));
	Serial.println(F("DDx#  // Disables dc motor x. Outputs DDx# when done."));

	Serial.println(F("Lx#  // Locks stepper motor x. Outputs Lx# when done."));

	Serial.println(F("SSx s a# // Sets speed of stepper motor x to s and the acceleration to a."));
	Serial.println(F("SDx s# // Sets speed of motor x to s."));
	Serial.println(F("SPx min max home# // Sets the parameters of a potentiometer. Min and max are the limits where it can move and home is from where we bring the robot when we start."));

	Serial.println(F("ASx n Py Bz ... # // Attach to stepper motor x a list of n sensors (like Potentiometer y, Button z etc)."));
	Example: AS0 1 A0 280 320 300 1#
	Serial.println(F("ADx n Py Bz ... # // Attach to dc motor x a list of n sensors (like Potentiometer y, Button z etc)."));
	Serial.println(F("AVx n Py Bz ... # // Attach to servo motor x a list of n sensors (like Potentiometer y, Button z etc)."));

	Serial.println(F("RUx# // Read the distance as measured by the ultrasonic sensor x. Outputs RUx d# when the reading has been completed. Note that this works in \"background\" and can output after some time (and in the meantime the program can do something else."));
	Serial.println(F("RBx# // Read the status of the button x. Outputs RBx s# where s is either 0 or 1."));
	Serial.println(F("RPx# // Read the position of the potentiometer x. Outputs RPx p#"));
	Serial.println(F("RIx# // Read the value of infrared sensor x. Outputs RIx v#"));
	Serial.println(F("RTx# // Read the value of Tera Ranger One sensor. Outputs RTx v#"));
	Serial.println(F("RAx# // Read the value of AS5147 sensor. Outputs RAx v#"));
	Serial.println(F("RMx# // Free memory. Outputs RM v#"));

	Serial.println(F("GSx# // Gets the parameters for stepper motor x: speed acceleration num_sensors_attached. Outputs GSx s a 1#"));
	Serial.println(F("GDx# // Gets the parameters for dc motor x: speed num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GDx s a 1 0 0#"));
	Serial.println(F("GPx# // Gets the parameters for potentiometer x: min max home direction. Outputs GPx n u h d#"));
	Serial.println(F("GUx# // Gets the parameters for ultrasound x: trig_pin echo_pin. Outputs GUx t e#"));
	Serial.println(F("GBx# // Gets the parameters for button x: out_pin direction. Outputs GBx p d#"));
	Serial.println(F("GAx# // Gets the parameters for AS5147 x: CS pin. Outputs GAx p d#"));

	Serial.println(F("CS n d1 s1 e1 d2 s2 e2# // Creates the stepper motors controller and set some of its parameters. n is the number of motors, d, s, e are dir, step and enable pins. Outputs CS# when done."));
	CS 3 5 4 12 7 6 12 9 8 12#
	Serial.println(F("CD n p1 d11 d12 e1 p2 d21 d22 e2# // Creates the dc motors controller and set some of its parameters. n is the number of motors, p is the pwm_pin, d1 and d2 are the direction pins and e is the enable pins. Outputs CD# when done."));
	Serial.println(F("CV n# // Creates the servo motors controller and set some of its parameters. n is the number of motors, Outputs CV# when done."));

	Serial.println(F("CP n p1 l1 h1 _h1 _d1 p2 l2 h2 _h2 _d2# // Creates the potentiometers controller and set some of its parameters. n is the number of potentiometers, p is the output pin, l, h and _h are bottom, upper and home position, _d is the directon of the sensor relative to the direction in which the motor is moving. Outputs CP# when done."));
	Serial.println(F("CU n t1 e1 t2 e2# // Creates the ultrasonic controller and set some of its parameters. n is the number of sonars, t and e are trigger and echo pins. Outputs CU# when done."));
	Serial.println(F("CIA n pin1 min1 max1 home1 dir1 pin2 min2 max2 home2 dir2# // Creates the infrared controller and set some of its parameters. n is the number of infrared sensors, pin is the analog pin index and min, max and home are the lower, upper bounds and home position of this sensor. Outputs CIA# when done."));
	Serial.println(F("CID n pin1 pin2# // Creates the infrared digital controller and set some of its parameters. n is the number of infrared digital sensors, pin1 is the analog pin index. Outputs CID# when done."));
	Serial.println(F("CB n p1 p2# // Creates the buttons controller and set some of its parameters. n is the number of button sensors, p is the digital pin. Outputs CB# when done."));
	Serial.println(F("CTR# // Creates the Tera Ranger One controller. Outputs CTR# when done."));
	Serial.println(F("CA n p1 p2 p3# // Creates the AS5147 controller. n is the number of sensors, p* are pins. Outputs CA# when done."));

// Example: CA 3 18 19 20#
	- CL creates LIDAR
	Serial.println(F("CV n p1 p2 p3# // Creates the servo motors controller and set its pins. n is the number of motors, p* are pins. CV# when done."));

	Serial.println(F("V# // Outputs version string. eg: 2016.06.12.0#"));

	Serial.println(F("Motor index is between 0 and num_motors - 1"));

	Serial.println();
  */
}
//--------------------------------------------------------------
void write_info(char *str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		if (str[i] == '#')
			Serial.write('$');
		else
			Serial.write(str[i]);

}
//--------------------------------------------------------------------------------------------
void parse_and_execute_stepper_motor_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	tmp_serial_out[0] = 0;

	if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') {
		// move motor
		int motor_index, num_steps;
		int num_consumed;
		int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_index, &num_steps, &num_consumed);
		if (num_read != 2) {
			sprintf(tmp_serial_out, "E#");
			i += 2;
			return;
		}
			steppers_controller.move_motor(motor_index, num_steps);
			is_command_running = 1;
			i += 2 + num_consumed;
	}
	else
		if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') {
			// disable motor
			int motor_index;
			int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
			if (num_read != 1) {
				sprintf(tmp_serial_out, "E#");
				i += 2;
				return;
			}
				steppers_controller.disable(motor_index);
				sprintf(tmp_serial_out, "SD%d#", motor_index);
				i += 4;
		}
		else
			if (tmp_str[i + 1] == 'L' || tmp_str[i + 1] == 'l') {
				// locks motor
				int motor_index;
				int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
				if (num_read != 1) {
					sprintf(tmp_serial_out, "E#");
					i += 2;
					return;
				}
					steppers_controller.lock(motor_index);
					sprintf(tmp_serial_out, "SL%d#", motor_index);
					i += 4;
				
			}
			else
				if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') {
					// stepper motor speed and acceleration
					int motor_index;
					int motor_speed, motor_acceleration;
					int num_consumed;
					int num_read = sscanf(tmp_str + i + 2, "%d%d%d%n", &motor_index, &motor_speed, &motor_acceleration, &num_consumed);
					if (num_read != 3) {
						sprintf(tmp_serial_out, "E#");
						i += 2;
						return;
					}
						steppers_controller.set_speed_and_acceleration(motor_index, motor_speed, motor_acceleration);
						sprintf(tmp_serial_out, "SS%d#", motor_index);
						i += 2 + num_consumed;
					
				}
				else
					if (tmp_str[i + 1] == 'H' || tmp_str[i + 1] == 'h') {
						// home
						int motor_index;
						int num_consumed;
						int num_read = sscanf(tmp_str + i + 2, "%d%n", &motor_index, &num_consumed);
						if (num_read != 1) {
							sprintf(tmp_serial_out, "E#");
							i += 2;
							return;
						}
							steppers_controller.go_home(motor_index, &as5147s_controller,
#ifdef USE_POTENTIOMETERS
								&potentiometers_controller,
#endif
								&buttons_controller);
							i += 4;
						
					}
					else
						if (tmp_str[i + 1] == 'G' || tmp_str[i + 1] == 'g') {
							// move motor to sensor position
							int motor_index, sensor_position;
							int num_consumed;
							int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_index, &sensor_position, &num_consumed);
							if (num_read != 2) {
								sprintf(tmp_serial_out, "E#");
								i += 2;
								return;
							}
								steppers_controller.go_to_sensor_position(motor_index, &as5147s_controller,
#ifdef USE_POTENTIOMETERS
									&potentiometers_controller,
#endif
									sensor_position);
								is_command_running = 1;
								i += 2 + num_consumed;
						}
						else
							if (tmp_str[i + 1] == 'T' || tmp_str[i + 1] == 't') {
								// stop
								int motor_index;
								int num_consumed;
								int num_read = sscanf(tmp_str + i + 2, "%d%n", &motor_index, &num_consumed);
								if (num_read != 1) {
									sprintf(tmp_serial_out, "E#");
									i += 1;
									return;
								}
									steppers_controller.stop(motor_index);
									sprintf(tmp_serial_out, "ST%d#", motor_index);
									i += 4;
								
							}
							else
								i++;
}
//----------------------------------------------------------------------
#ifdef USE_TB6612FNG
void parse_and_execute_DC_motor_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	tmp_serial_out[0] = 0;
	if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') { // move motor
		int motor_index, num_miliseconds;
		int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_miliseconds);
		if (num_read == 2) {
			dc_motors_controller_TB6612FNG.move_motor(motor_index, num_miliseconds);
			is_command_running = 1;
			i += 5;
		}
		else
			i++;// error on incomplete string (does nothing)
	}
	// disables motor
	else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // disables DC motor
		int motor_index;
		int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
		dc_motors_controller_TB6612FNG.disable(motor_index);
		sprintf(tmp_serial_out, "DD%d#", motor_index);
		i += 4;
	}

	else
		i++; // incomplete string
}
#endif
//----------------------------------------------------------------------
void parse_and_execute_servo_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	tmp_serial_out[0] = 0;

	if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') {
		// move motor
		int motor_index, position;
		int num_consumed;
		int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_index, &position, &num_consumed);
		if (num_read != 2) {
			sprintf(tmp_serial_out, "E#");
			i += 1;
			return;
		}

		servos_controller.set_position(motor_index, position);
		is_command_running = 1;
		i += 2 + num_consumed;
	}
}
//----------------------------------------------------------------------
void parse_and_execute_LIDAR_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	tmp_serial_out[0] = 0;
	if (tmp_str[i + 1] == 'G' || tmp_str[i + 1] == 'g') {// lidar go
		tera_ranger_one_lidar->start();
		i += 3;
	}
	else
		if (tmp_str[i + 1] == 'H' || tmp_str[i + 1] == 'h') {// lidar halt
			tera_ranger_one_lidar->stop();
			i += 3;
		}
		else
			if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') {// set speed and acceleration
				int motor_speed, motor_acceleration;
				int num_consumed;
				int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_speed, &motor_acceleration, &num_consumed);
				if (num_read != 2) {
					sprintf(tmp_serial_out, "E#");
					i += 2;
					return;
				}

					tera_ranger_one_lidar->set_motor_speed_and_acceleration(motor_speed, motor_acceleration);
					i += 2 + num_consumed;
			}
			else// unknown command
				i += 2;
}
//--------------------------------------------------
void parse_and_execute_attach_sensors_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	tmp_serial_out[0] = 0;

	int motor_index, num_sensors;
	char motor_type = tmp_str[i + 1];
	int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_sensors);
	// Serial.println(num_sensors);
	if (num_read != 2) {
		sprintf(tmp_serial_out, "E#");
		i += 2;
		return;
	}
	if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
		steppers_controller.set_num_attached_sensors(motor_index, num_sensors);

	else
#ifdef USE_TB6612FNG
		if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
			dc_motors_controller_TB6612FNG.set_num_attached_sensors(motor_index, num_sensors);
#endif
	;
	// now I have to add sensors one by one
	int j = i + 4;
	i += 5;
	int k = 0;
	while (k < num_sensors) {
		if (tmp_str[j] == 'P' || tmp_str[j] == 'p') {
			int sensor_index;
			int _low, _high, _home;
			int _direction;
			int num_consumed;

			int num_read = sscanf(tmp_str + j + 1, "%d%d%d%d%d%n", &sensor_index, &_low, &_high, &_home, &_direction, &num_consumed);
			if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
				steppers_controller.add_sensor(motor_index, POTENTIOMETER, sensor_index, _low, _high, _home, _direction);
			else
#ifdef USE_TB6612FNG
				if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
					dc_motors_controller_TB6612FNG.add_sensor(motor_index, POTENTIOMETER, sensor_index);
#endif
			;
			j += 1 + num_consumed;
			k++;
		}
		else if (tmp_str[j] == 'B' || tmp_str[j] == 'b') {//buttons
			int sensor_index;
			int _direction;
			int num_consumed;
			int num_read = sscanf(tmp_str + j + 1, "%d%d%n", &sensor_index, &_direction, &num_consumed);
			if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
				steppers_controller.add_sensor(motor_index, BUTTON, sensor_index, 0, 0, 0, _direction);
			else
#ifdef USE_TB6612FNG
				if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
					dc_motors_controller_TB6612FNG.add_sensor(motor_index, BUTTON, sensor_index);
#endif
			;
			j += 1 + num_consumed;
			k++;
		}
		else
			if (tmp_str[j] == 'I' || tmp_str[j] == 'i') {// infrared
				int sensor_index;
				int num_read = sscanf(tmp_str + j + 1, "%d", &sensor_index);
				if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
					steppers_controller.add_sensor(motor_index, INFRARED_ANALOG, sensor_index, 0, 0, 0, 0);
				else
#ifdef USE_TB6612FNG
					if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
						dc_motors_controller_TB6612FNG.add_sensor(motor_index, INFRARED_ANALOG, sensor_index);
#endif
				;
				j += 2;
				k++;
			}
			else
				if (tmp_str[j] == 'A' || tmp_str[j] == 'a') {
					int sensor_index;
					int _low, _high, _home;
					int _direction;
					int num_consumed;

					int num_read = sscanf(tmp_str + j + 1, "%d%d%d%d%d%n", &sensor_index, &_low, &_high, &_home, &_direction, &num_consumed);
					if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
						steppers_controller.add_sensor(motor_index, AS5147, sensor_index, _low, _high, _home, _direction);
					else
#ifdef USE_TB6612FNG
						if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
							dc_motors_controller_TB6612FNG.add_sensor(motor_index, AS5147, sensor_index);
#endif
					;
					j += 1 + num_consumed;
					k++;
				}
				else
					j++;
	}
	sprintf(tmp_serial_out, "A%d#", motor_index);
	i = j + 1;
	// Serial.println(i);
}
//--------------------------------------------------
void parse_and_execute_get_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	tmp_serial_out[0] = 0;

	if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') { // gets stepper motor speed and acceleration num sensors attached
		int motor_index;
		byte num_sensors_attached;
		float motor_speed, motor_acceleration;
		int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
		steppers_controller.get_speed_and_acceleration(motor_index, &motor_speed, &motor_acceleration);
		num_sensors_attached = steppers_controller.get_num_attached_sensors(motor_index);
		sprintf(tmp_serial_out, "GS%d %d %d %d#", motor_index, (int)motor_speed, (int)motor_acceleration, num_sensors_attached);
		i += 4;
	}
	else
#ifdef USE_TB6612FNG

		if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // gets stepper motor speed and acceleration
			int motor_index;
			byte motor_speed;
			int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
			dc_motors_controller_TB6612FNG.get_speed(motor_index, &motor_speed);
			sprintf(tmp_serial_out, "GD%d %d#", motor_index, motor_speed);
			i += 4;
		}
		else
#endif
#ifdef USE_POTENTIOMETERS

			if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') { // get potentiometer min max home dir
				int pot_index;
				//          int pot_min, pot_max, pot_home;
				//		  int8_t pot_dir;
				byte pot_pin;
				int num_read = sscanf(tmp_str + i + 2, "%d", &pot_index);
				potentiometers_controller.get_params(pot_index, &pot_pin);
				sprintf(tmp_serial_out, "GP%d %d#", pot_index, (int)pot_pin);
				i += 5;
			}
			else
#endif
				if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') { // get ultrasound trig_pin echo_pin
					int ultrasound_index;
					byte trig_pin, echo_pin;
					int num_read = sscanf(tmp_str + i + 2, "%d", &ultrasound_index);
					ultrasonic_sensors_controller.get_sensor_pins(ultrasound_index, &trig_pin, &echo_pin);
					sprintf(tmp_serial_out, "GU%d %d %d#", ultrasound_index, trig_pin, echo_pin);
					i += 4;
				}
				else if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') { // get button pin direction
					int button_index;
					byte _pin;

					int num_read = sscanf(tmp_str + i + 2, "%d", &button_index);
					buttons_controller.get_params(button_index, &_pin);
					sprintf(tmp_serial_out, "GB%d %d#", button_index, _pin);
					i += 2;
				}
				else
					i++;// incomplete string
}
//--------------------------------------------------
void parse_and_execute_read_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
#ifdef USE_POTENTIOMETERS
	// potentiometer
	if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') {
		int sensor_index;
		int num_read = sscanf(tmp_str + i + 2, "%d", &sensor_index);
		int sensor_value = potentiometers_controller.get_position(sensor_index);
		sprintf(tmp_serial_out, "P%d %d#", sensor_index, sensor_value);
		i += 3;
	}
	else
#endif
		// AS5147
		if (tmp_str[i + 1] == 'A' || tmp_str[i + 1] == 'a') {
			int sensor_index;
			int num_read = sscanf(tmp_str + i + 2, "%d", &sensor_index);
			int sensor_value = as5147s_controller.get_position(sensor_index);
			sprintf(tmp_serial_out, "RA%d %d#", sensor_index, sensor_value);
			i += 3;
		}
		else
			// button
			if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') {
				int sensor_index;
				int num_read = sscanf(tmp_str + i + 2, "%d", &sensor_index);
				// Serial.println(sensor_index);
				int sensor_value = buttons_controller.get_state(sensor_index);
				sprintf(tmp_serial_out, "RB%d %d#", sensor_index, sensor_value);
				i += 3;
			}
			else
				// ultrasonic
				if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') {
					int sensor_index;
					int num_read = sscanf(tmp_str + i + 2, "%d", &sensor_index);
					ultrasonic_sensors_controller.trigger(sensor_index);
					i += 3;
				}
				else
					// infrared
					if (tmp_str[i + 1] == 'I' || tmp_str[i + 1] == 'i') {
						int sensor_index;
						int num_read = sscanf(tmp_str + i + 2, "%d", &sensor_index);
						int sensor_value = infrared_analog_sensors_controller.get_signal_strength(sensor_index);
						sprintf(tmp_serial_out, "RI%d %d#", sensor_index, sensor_value);
						i += 3;
					}
					else
						// Tera Ranger One
						if (tmp_str[i + 1] == 'T' || tmp_str[i + 1] == 't') {
							tera_ranger_one_controller.trigger();
							i += 2;
						}
						else
							if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') {
								sprintf(tmp_serial_out, "RM%d#", freeMemory());
								i += 2;
							}
}
//--------------------------------------------------
void parse_and_execute_create_stepper_motors_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	if (!steppers_controller.is_running()) {
		//stepper_motors_controller.delete_memory();
		int num_motors = 0;

		int num_consumed = 0;
		int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);

		int total_num_consumed = 3 + num_consumed;
		bool new_num_motors = steppers_controller.num_motors != num_motors;
		steppers_controller.set_num_motors(num_motors);
		for (int k = 0; k < num_motors; k++) {
			int _step_pin, _dir_pin, _enable_pin;
			int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%d%d%n", &_dir_pin, &_step_pin, &_enable_pin, &num_consumed);
			if (new_num_motors)
				steppers_controller.set_pins(k, _dir_pin, _step_pin, _enable_pin);
			total_num_consumed += num_consumed + 1;
		}
		steppers_controller.disable_all();
		sprintf(tmp_serial_out, "CS#");
		i += total_num_consumed;
	}
}
//--------------------------------------------------
void parse_and_execute_create_servo_motors_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_motors = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);

	int total_num_consumed = 3 + num_consumed;
	bool new_num_motors = servos_controller.get_num_servos() != num_motors;
	servos_controller.set_num_servos(num_motors);
	for (int k = 0; k < num_motors; k++) {
		int _pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%n", &_pin, &num_consumed);
		if (new_num_motors)
			servos_controller.set_params(k, _pin);
		total_num_consumed += num_consumed + 1;
	}
	sprintf(tmp_serial_out, "CV#");
	i += total_num_consumed;
}
//--------------------------------------------------
#ifdef USE_TB6612FNG
void parse_and_execute_create_DC_motors_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	if (!dc_motors_controller_TB6612FNG.is_running()) {
		int num_motors = 0;

		int num_consumed = 0;
		int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);

		int total_num_consumed = 3 + num_consumed;
		dc_motors_controller_TB6612FNG.set_num_motors(num_motors);
		for (int k = 0; k < num_motors; k++) {
			int pwm_pin, dir_pin1, dir_pin2, enable_pin;
			int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%d%d%d%n", &pwm_pin, &dir_pin1, &dir_pin2, &enable_pin, &num_consumed);
			dc_motors_controller_TB6612FNG.set_motor_pins(k, pwm_pin, dir_pin1, dir_pin2, enable_pin);
			total_num_consumed += num_consumed + 1;
		}

		sprintf(tmp_serial_out, "CD#");
		i += total_num_consumed;
	}
}
#endif
//--------------------------------------------------
void parse_and_execute_create_ultrasonic_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_sonars = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_sonars, &num_consumed);

	int total_num_consumed = 3 + num_consumed;
	ultrasonic_sensors_controller.set_num_sensors(num_sonars);
	for (int k = 0; k < num_sonars; k++) {
		int _trig_pin, _echo_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%d%n", &_trig_pin, &_echo_pin, &num_consumed);
		ultrasonic_sensors_controller.set_sensor_pins(k, _trig_pin, _echo_pin);
		total_num_consumed += num_consumed + 1;
	}

	sprintf(tmp_serial_out, "CU#");
	i += total_num_consumed;
}
//--------------------------------------------------
#ifdef USE_POTENTIOMETERS
void parse_and_execute_create_potentiometer_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_potentiometers = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_potentiometers, &num_consumed);

	int total_num_consumed = 3 + num_consumed;
	potentiometers_controller.set_num_sensors(num_potentiometers);
	for (int k = 0; k < num_potentiometers; k++) {
		int _out_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%n", &_out_pin, &num_consumed);
		potentiometers_controller.set_params(k, _out_pin);
		total_num_consumed += num_consumed + 1;
	}
	sprintf(tmp_serial_out, "CP#");
	i += total_num_consumed;
}
#endif

//--------------------------------------------------
void parse_and_execute_create_as5147_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_as5147 = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_as5147, &num_consumed);

	int total_num_consumed = 3 + num_consumed;
	as5147s_controller.set_num_sensors(num_as5147);

	pinMode(SI, OUTPUT);
	pinMode(SO, INPUT);
	pinMode(CLK, OUTPUT);

	for (int k = 0; k < num_as5147; k++) {
		int _out_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%n", &_out_pin, &num_consumed);
		as5147s_controller.set_params(k, _out_pin);
		total_num_consumed += num_consumed + 1;
	}

	sprintf(tmp_serial_out, "CA#");
	i += total_num_consumed;
}
//--------------------------------------------------
void parse_and_execute_create_infrared_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_infrareds = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_infrareds, &num_consumed);

	int total_num_consumed = 3 + num_consumed;
	infrared_analog_sensors_controller.set_num_sensors(num_infrareds);
	for (int k = 0; k < num_infrareds; k++) {
		int out_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%n", &out_pin, &num_consumed);
		infrared_analog_sensors_controller.set_params(k, out_pin);
		total_num_consumed += num_consumed + 1;
	}
	sprintf(tmp_serial_out, "CI#");
	i += total_num_consumed;
}
//--------------------------------------------------
void parse_and_execute_create_buttons_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_buttons = 0;
	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%d%n", &num_buttons, &num_consumed);

	int total_num_consumed = 3 + num_consumed;
	buttons_controller.set_num_sensors(num_buttons);
	for (int k = 0; k < num_buttons; k++) {
		int _pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%n", &_pin, &num_consumed);
		buttons_controller.set_params(k, _pin);
		total_num_consumed += num_consumed + 1;
	}
	sprintf(tmp_serial_out, "CB#");
	i += total_num_consumed;
}
//--------------------------------------------------
void parse_and_execute_create_LIDAR_commands(char* tmp_str, byte str_length, byte &i, char *tmp_serial_out)
{
	int num_consumed = 0;
	int _dir_pin, _step_pin, _enable_pin, _infrared_pin;
	int num_read = sscanf(tmp_str + i + 2, "%d%d%d%d%n", &_dir_pin, &_step_pin, &_enable_pin, &_infrared_pin, &num_consumed);
	tera_ranger_one_lidar = new t_tera_ranger_one_lidar(_dir_pin, _step_pin, _enable_pin, _infrared_pin);

	sprintf(tmp_serial_out, "CL#");
	i += num_consumed + 2;
}
//--------------------------------------------------
void parse_and_execute_commands(char* tmp_str, byte str_length, char *serial_out)
{
	byte i = 0;
	serial_out[0] = 0;
	char tmp_serial_out[100];
	tmp_serial_out[0] = 0;

	while (i < str_length) {
		// can be more than 1 command in a string, so I have to check again for a letter
		if ((tmp_str[i] >= 'A' && tmp_str[i] <= 'Z') || (tmp_str[i] >= 'a' && tmp_str[i] <= 'z')) {

			if (tmp_str[i] == 'S' || tmp_str[i] == 's') { // stepper motor
				parse_and_execute_stepper_motor_commands(tmp_str, str_length, i, tmp_serial_out);
				strcat(serial_out, tmp_serial_out);
				continue;
			}

			// dc motor
#ifdef USE_TB6612FNG
			if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // DC motor
				parse_and_execute_DC_motor_commands(tmp_str, str_length, i, tmp_serial_out);
				strcat(serial_out, tmp_serial_out);
				continue;
			}
#endif
			if (tmp_str[i] == 'R' || tmp_str[i] == 'r') {
				parse_and_execute_read_commands(tmp_str, str_length, i, tmp_serial_out);
				strcat(serial_out, tmp_serial_out);
				continue;
			} // end reading
			// LIDAR
			if (tmp_str[i] == 'L' || tmp_str[i] == 'l') {// lidar operations
				parse_and_execute_LIDAR_commands(tmp_str, str_length, i, tmp_serial_out);
				strcat(serial_out, tmp_serial_out);
				continue;
			}

			/*
			// go home
			  else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // go DC home
				int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
				dc_motors_controller_TB6612FNG.go_home(motor_index, &buttons_controller);
			  }
			  is_command_running = 1;
			  i += 4;
			  continue;
			}
			*/
			// sets something
	  /*
			if (tmp_str[i] == 'S' || tmp_str[i] == 's') {
			  else if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') { // potentiometer min max home
				int pot_index, pot_min, pot_max, pot_home, pot_pin, pot_dir;
				int num_read = sscanf(tmp_str + i + 2, "%d%d%d%d%d%d", &pot_index, &pot_pin, &pot_min, &pot_max, &pot_home, &pot_dir);
				potentiometers_controller.set_params(pot_index, pot_pin, pot_min, pot_max, pot_home, pot_dir);
				i += 7;
			  }
			  else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // dc motor speed
				int motor_index;
				int motor_speed;
				int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &motor_speed);
				dc_motors_controller_TB6612FNG.set_speed(motor_index, motor_speed);
				i += 5;
			  }
			  continue;
			}
	  */
			if (tmp_str[i] == 'A' || tmp_str[i] == 'a') { // attach sensors to motors
				parse_and_execute_attach_sensors_commands(tmp_str, str_length, i, tmp_serial_out);
				strcat(serial_out, tmp_serial_out);
				continue;
			}
			// for debugging purpose
			if (tmp_str[i] == 'G' || tmp_str[i] == 'g') {
				parse_and_execute_get_commands(tmp_str, str_length, i, tmp_serial_out);
				strcat(serial_out, tmp_serial_out);
				continue;
			}
			// test connection
			if (tmp_str[i] == 'T' || tmp_str[i] == 't') {
				sprintf(tmp_serial_out, "T#");
				strcat(serial_out, tmp_serial_out);
				i += 2;
				continue;
			}

			if (tmp_str[i] == 'V' || tmp_str[i] == 'v') {
				// do things with servos
				if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') {
					byte j = i + 1;
					parse_and_execute_stepper_motor_commands(tmp_str, str_length, j, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					i = j;
					continue;
				}
				else {
					// version number
					sprintf(tmp_serial_out, "V%s#", firmware_version);
					strcat(serial_out, tmp_serial_out);
					i += 2;
					continue;
				}
			}

			// create something
			if (tmp_str[i] == 'C' || tmp_str[i] == 'c') {
				// create a list of stepper motors
				if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') {
					parse_and_execute_create_stepper_motors_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
#ifdef USE_TB6612FNG
				// create a list of DC motors
				if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') {
					parse_and_execute_create_DC_motors_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
#endif
				// create a list of servo motors
				if (tmp_str[i + 1] == 'V' || tmp_str[i + 1] == 'v') {
					parse_and_execute_create_servo_motors_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
				// create a list of ultrasonic sensors
				if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') {
					parse_and_execute_create_ultrasonic_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
#ifdef USE_POTENTIOMETERS
				// create a list of potentiometers
				if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') {
					parse_and_execute_create_potentiometer_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
#endif
				// create a list of AS5147
				if (tmp_str[i + 1] == 'A' || tmp_str[i + 1] == 'a') {
					parse_and_execute_create_as5147_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}

				// create a list of infrared sensors
				if (tmp_str[i + 1] == 'I' || tmp_str[i + 1] == 'i') {
					parse_and_execute_create_infrared_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}

				// create a list of button sensors
				if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') {
					parse_and_execute_create_buttons_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}

				// create Tera Ranger One controller
				if (tmp_str[i + 1] == 'T' || tmp_str[i + 1] == 't') {
					tera_ranger_one_controller.create_init();
					sprintf(tmp_serial_out, "CT#");
					strcat(serial_out, tmp_serial_out);
					i += 3;
					continue;
				}

				// creates a LIDAR
				if (tmp_str[i + 1] == 'L' || tmp_str[i + 1] == 'l') {
					parse_and_execute_create_LIDAR_commands(tmp_str, str_length, i, tmp_serial_out);
					strcat(serial_out, tmp_serial_out);
					continue;
				}

				i++; // incomplete string
				continue;
			}
			else
					i++;// incomplete string
		}
		else
			i++; // not a letter, move to the next one

	}
}
//--------------------------------------------------------------------------------------------
//Main loop
void loop()
{
	char serial_out[MAX_BUFFER_LENGTH];
	serial_out[0] = 0;

	// check to see if there are new results from ultrasonic sensor
	ultrasonic_sensors_controller.update_results(serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		Serial.flush();
	}

	tera_ranger_one_controller.update_results(serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		Serial.flush();
	}

	if (Serial.available() || current_buffer[0]) {
		byte num_read = 0;
		char serial_buffer[65];

		//num_read = Serial.readBytes(serial_buffer, 64); //Read up to 64 bytes

		int incomingByte = Serial.read();
		if (incomingByte != -1) {
			//if (incomingByte >= 'A' && incomingByte <= 'z' || incomingByte >= '0' && incomingByte <= '9' || incomingByte == ' ' || incomingByte == '#') {
				serial_buffer[0] = incomingByte;
				num_read = 1;
			//}
			//Serial.write("aici");
		}
		serial_buffer[num_read] = 0;// terminate the string
		if (serial_buffer[0] || current_buffer[0]) {
			if (serial_buffer[0])
				if (strlen(serial_buffer) + strlen(current_buffer) < MAX_BUFFER_LENGTH) // concat only if it fits
					strcat(current_buffer, serial_buffer);
			
			if (current_buffer[0]) {
				Serial.write("I ");
				Serial.flush();
				write_info(current_buffer);
				Serial.flush();
				Serial.write("/");
				Serial.flush();
				write_info(serial_buffer);
				Serial.flush();
				Serial.write("#");
				Serial.flush();
			}
			
#ifdef DEBUG
			Serial.write("initial buffer is=");
			Serial.write(current_buffer);
			Serial.println();
#endif

			// parse from the beginning until I find a M, D, L, S, A, P, B, U, G, T
			int current_buffer_length = strlen(current_buffer);
//			bool command_found = false;
			for (int i = 0; i < current_buffer_length; i++)
				if ((current_buffer[i] >= 'A' && current_buffer[i] <= 'Z') || (current_buffer[i] >= 'a' && current_buffer[i] <= 'z')) { // a command
				  // find the terminal character #
					int j = current_buffer_length - 1;
					for (; j > i && current_buffer[j] != '#'; j--);// parse (from the end) until I find the termination char
					if (j > i) {

#ifdef DEBUG
						char tmp_str[64];
						strncpy(tmp_str, current_buffer + i, j - i);
						tmp_str[j - i] = 0;
						Serial.write("current command is=");
						Serial.write(tmp_str);
						Serial.println();
#endif
				//		command_found = true;
							Serial.write("I ");
							Serial.write(current_buffer + i, j - i);
							Serial.write("-#");
							Serial.flush();

							parse_and_execute_commands(current_buffer + i, j - i, serial_out);
						if (serial_out[0]) {
							while (!Serial);
							Serial.write(serial_out);
							Serial.flush();
						}


						// remove the current executed command
						strcpy(current_buffer, current_buffer + j + 1);// not sure if this is good due to overlaps

#ifdef DEBUG
						Serial.write("buffer left=");
						Serial.write(current_buffer);
						Serial.write("\n----------------\n");
						//Serial.println(strlen(current_buffer)); // buffer length
#endif

						break; //for i
					}
					else { // the string is not completed ... so I must wait for more...
						Serial.write("I break not found#");
						Serial.flush();
						break; // for i
					}
				}
			//if (strlen(current_buffer) && !command_found)// wrong string
				//current_buffer[0] = 0;
		}
	}
	steppers_controller.run_motors(&as5147s_controller,
#ifdef USE_POTENTIOMETERS		
		&potentiometers_controller,
#endif		
		&buttons_controller, serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		Serial.flush();
		serial_out[0] = 0;
	}
#ifdef USE_TB6612FNG
	dc_motors_controller_TB6612FNG.update_motors(&buttons_controller, serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		serial_out[0] = 0;
	}
#endif
	if (tera_ranger_one_lidar) {
		tera_ranger_one_lidar->run_motor(serial_out);
		if (serial_out[0]) {
			Serial.write(serial_out);
			Serial.flush();
			serial_out[0] = 0;
		}
	}
}
//------------------------------------------------------------------------------
