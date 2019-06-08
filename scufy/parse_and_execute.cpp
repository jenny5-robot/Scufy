#include "parse_and_execute.h"


//----------------------------------------------------------------------
void parse_and_execute_stepper_motor_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out, 
	t_steppers_controller &steppers_controller,
	t_as5147s_controller& as5147s_controller,
	t_buttons_controller &buttons_controller)
{
	tmp_serial_out[0] = 0;

	if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') {
		// move motor
		uint8_t motor_index;
		int num_steps;
		int num_consumed;
		int num_read = sscanf(tmp_str + i + 2, "%hhu%d%n", &motor_index, &num_steps, &num_consumed);
		if (num_read != 2) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}
		steppers_controller.move_motor(motor_index, num_steps);
		//is_command_running = 1;
		i += 2 + num_consumed;
	}
	else
		if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') {
			// disable motor
			uint8_t motor_index;
			int num_read = sscanf(tmp_str + i + 2, "%hhu", &motor_index);
			if (num_read != 1) {
				strcpy(tmp_serial_out, "E#");
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
				uint8_t motor_index;
				int num_read = sscanf(tmp_str + i + 2, "%hhu", &motor_index);
				if (num_read != 1) {
					strcpy(tmp_serial_out, "E#");
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
					uint8_t motor_index;
					int motor_speed, motor_acceleration;
					int num_consumed;
					int num_read = sscanf(tmp_str + i + 2, "%hhu%d%d%n", &motor_index, &motor_speed, &motor_acceleration, &num_consumed);
					if (num_read != 3) {
						strcpy(tmp_serial_out, "E#");
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
						uint8_t motor_index;
						int num_consumed;
						int num_read = sscanf(tmp_str + i + 2, "%hhu%n", &motor_index, &num_consumed);
						if (num_read != 1) {
							strcpy(tmp_serial_out, "E#");
							i += 2;
							return;
						}
						steppers_controller.go_home(motor_index, &as5147s_controller,
#ifdef USE_POTENTIOMETERS
							& potentiometers_controller,
#endif
							& buttons_controller);
						i += 4;

					}
					else
						if (tmp_str[i + 1] == 'G' || tmp_str[i + 1] == 'g') {
							// move motor to sensor position
							uint8_t motor_index;
							int sensor_position;
							int num_consumed;
							int num_read = sscanf(tmp_str + i + 2, "%hhu%d%n", &motor_index, &sensor_position, &num_consumed);
							if (num_read != 2) {
								strcpy(tmp_serial_out, "E#");
								i += 2;
								return;
							}
							steppers_controller.go_to_sensor_position(motor_index, &as5147s_controller,
#ifdef USE_POTENTIOMETERS
								& potentiometers_controller,
#endif
								sensor_position);
							//is_command_running = 1;
							i += 2 + num_consumed;
						}
						else
							if (tmp_str[i + 1] == 'T' || tmp_str[i + 1] == 't') {
								// stop
								uint8_t motor_index;
								int num_consumed;
								int num_read = sscanf(tmp_str + i + 2, "%hhu%n", &motor_index, &num_consumed);
								if (num_read != 1) {
									strcpy(tmp_serial_out, "E#");
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
void parse_and_execute_DC_motor_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out)
{
	tmp_serial_out[0] = 0;
	if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') { // move motor
		int motor_index, num_miliseconds;
		int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_miliseconds);
		if (num_read == 2) {
			dc_motors_controller_TB6612FNG.move_motor(motor_index, num_miliseconds);
			//is_command_running = 1;
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
void parse_and_execute_servo_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out, t_servos_controller &servos_controller)
{
	tmp_serial_out[0] = 0;

	if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') {
		// move motor
		uint8_t motor_index;
		int position;
		int num_consumed;
		int num_read = sscanf(tmp_str + i + 2, "%hhu%d%n", &motor_index, &position, &num_consumed);
		if (num_read != 2) {
			strcpy(tmp_serial_out, "E#");
			i += 1;
			return;
		}

		servos_controller.set_position(motor_index, position);
		//is_command_running = 1;
		i += 2 + num_consumed;
	}
}
//----------------------------------------------------------------------
void parse_and_execute_LiDAR_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out, 
	t_tera_ranger_one_lidar_controller*tera_ranger_one_lidar)
{
	tmp_serial_out[0] = 0;
	if (tmp_str[i + 1] == 'G' || tmp_str[i + 1] == 'g') {// LiDAR go
		tera_ranger_one_lidar->start();
		i += 3;
	}
	else
		if (tmp_str[i + 1] == 'H' || tmp_str[i + 1] == 'h') {// LiDAR halt
			tera_ranger_one_lidar->stop();
			sprintf(tmp_serial_out, "LH#");
			i += 3;
		}
		else
			if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') {// set speed and acceleration
				int motor_speed, motor_acceleration;
				int num_consumed;
				int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_speed, &motor_acceleration, &num_consumed);
				if (num_read != 2) {
					strcpy(tmp_serial_out, "E#");
					i += 2;
					return;
				}

				tera_ranger_one_lidar->set_motor_speed_and_acceleration(motor_speed, motor_acceleration);
				i += 2 + num_consumed;
				sprintf(tmp_serial_out, "LS#");
			}
			else// unknown command
				i += 2;
}
//--------------------------------------------------
void parse_and_execute_attach_sensors_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out, t_steppers_controller &steppers_controller)
{
	tmp_serial_out[0] = 0;

	uint8_t motor_index, num_sensors;
	char motor_type = tmp_str[i + 1];
	int num_read = sscanf(tmp_str + i + 2, "%hhu%hhu", &motor_index, &num_sensors);
	// Serial.println(num_sensors);
	if (num_read != 2) {
		strcpy(tmp_serial_out, "E#");
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
			uint8_t sensor_index;
			int _low, _high, _home;
			int8_t same_direction_as_motor;
			int num_consumed;

			int num_read = sscanf(tmp_str + j + 1, "%hhu%d%d%d%hhd%n", &sensor_index, &_low, &_high, &_home, &same_direction_as_motor, &num_consumed);
			if (num_read != 5) {
				strcpy(tmp_serial_out, "E#");
				i += 2;
				return;
			}
			if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
				steppers_controller.add_sensor(motor_index, POTENTIOMETER, sensor_index, _low, _high, _home, same_direction_as_motor);
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
			uint8_t sensor_index;
			int8_t _direction;
			int num_consumed;
			int num_read = sscanf(tmp_str + j + 1, "%hhu%hhd%n", &sensor_index, &_direction, &num_consumed);
			if (num_read != 2) {
				strcpy(tmp_serial_out, "E#");
				i += 2;
				return;
			}
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
				uint8_t sensor_index;
				int num_read = sscanf(tmp_str + j + 1, "%hhu", &sensor_index);
				if (num_read != 1) {
					strcpy(tmp_serial_out, "E#");
					i += 2;
					return;
				}
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
					uint8_t sensor_index;
					int _low, _high, _home;
					int8_t same_direction_as_motor;
					int num_consumed;

					int num_read = sscanf(tmp_str + j + 1, "%hhu%d%d%d%hhd%n", &sensor_index, &_low, &_high, &_home, &same_direction_as_motor, &num_consumed);
					if (num_read != 5) {
						strcpy(tmp_serial_out, "E#");
						i += 2;
						return;
					}
					if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
						steppers_controller.add_sensor(motor_index, AS5147, sensor_index, _low, _high, _home, same_direction_as_motor);
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
void parse_and_execute_get_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out)
{
	/*
	tmp_serial_out[0] = 0;

	if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') { // gets stepper motor speed and acceleration num sensors attached
		int8_t motor_index;
		byte num_sensors_attached;
		float motor_speed, motor_acceleration;
		int num_read = sscanf(tmp_str + i + 2, "%hhu", &motor_index);
		if (num_read != 1) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}		steppers_controller.get_speed_and_acceleration(motor_index, &motor_speed, &motor_acceleration);
		num_sensors_attached = steppers_controller.get_num_attached_sensors(motor_index);
		sprintf(tmp_serial_out, "GS%d %d %d %u#", motor_index, (int)motor_speed, (int)motor_acceleration, num_sensors_attached);
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
					uint8_t ultrasound_index;
					byte trig_pin, echo_pin;
					int num_read = sscanf(tmp_str + i + 2, "%hhu", &ultrasound_index);
					if (num_read != 1) {
						strcpy(tmp_serial_out, "E#");
						i += 2;
						return;
					}
					ultrasonic_sensors_controller.get_sensor_pins(ultrasound_index, &trig_pin, &echo_pin);
					sprintf(tmp_serial_out, "GU%d %u %u#", ultrasound_index, trig_pin, echo_pin);
					i += 4;
				}
				else if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') { // get button pin direction
					uint8_t button_index;
					byte _pin;

					int num_read = sscanf(tmp_str + i + 2, "%hhu", &button_index);
					if (num_read != 1) {
						strcpy(tmp_serial_out, "E#");
						i += 2;
						return;
					}
					buttons_controller.get_params(button_index, &_pin);
					sprintf(tmp_serial_out, "GB%d %u#", button_index, _pin);
					i += 2;
				}
				else
					i++;// incomplete string
					*/
}
//--------------------------------------------------
void parse_and_execute_read_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	#ifdef USE_POTENTIOMETERS
		t_potentiometers_controller &potentiometers_controller,
	#endif
	t_as5147s_controller &as5147s_controller,
	t_ultrasonic_sensors_controller &ultrasonic_sensors_controller,
	t_infrared_analog_sensors_controller &infrared_analog_sensors_controller,
	t_buttons_controller &buttons_controller,
	t_tera_ranger_one_controller &tera_ranger_one_controller
)
{
#ifdef USE_POTENTIOMETERS
	// potentiometer
	if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') {
		uint8_t sensor_index;
		int num_read = sscanf(tmp_str + i + 2, "%hhu", &sensor_index);
		int sensor_value = potentiometers_controller.get_position(sensor_index);
		sprintf(tmp_serial_out, "P%d %d#", sensor_index, sensor_value);
		i += 3;
	}
	else
#endif
		// AS5147
		if (tmp_str[i + 1] == 'A' || tmp_str[i + 1] == 'a') {
			uint8_t sensor_index;
			int num_read = sscanf(tmp_str + i + 2, "%hhu", &sensor_index);
			if (num_read != 1) {
				strcpy(tmp_serial_out, "E#");
				i += 2;
				return;
			}
			int sensor_value = as5147s_controller.get_position(sensor_index);
			sprintf(tmp_serial_out, "RA%d %d#", sensor_index, sensor_value);
			i += 3;
		}
		else
			// button
			if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') {
				uint8_t sensor_index;
				int num_read = sscanf(tmp_str + i + 2, "%hhu", &sensor_index);
				if (num_read != 1) {
					strcpy(tmp_serial_out, "E#");
					i += 2;
					return;
				}
				// Serial.println(sensor_index);
				int sensor_value = buttons_controller.get_state(sensor_index);
				sprintf(tmp_serial_out, "RB%d %d#", sensor_index, sensor_value);
				i += 3;
			}
			else
				// ultrasonic
				if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') {
					uint8_t sensor_index;
					int num_read = sscanf(tmp_str + i + 2, "%hhu", &sensor_index);
					if (num_read != 1) {
						strcpy(tmp_serial_out, "E#");
						i += 2;
						return;
					}
					ultrasonic_sensors_controller.trigger(sensor_index);
					i += 3;
				}
				else
					// infrared
					if (tmp_str[i + 1] == 'I' || tmp_str[i + 1] == 'i') {
						uint8_t sensor_index;
						int num_read = sscanf(tmp_str + i + 2, "%hhu", &sensor_index);
						if (num_read != 1) {
							strcpy(tmp_serial_out, "E#");
							i += 2;
							return;
						}
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
void parse_and_execute_create_stepper_motors_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_steppers_controller &steppers_controller)
{
	if (!steppers_controller.is_running()) {
		//stepper_motors_controller.delete_memory();
		uint8_t num_motors = 0;

		int num_consumed = 0;
		int num_read = sscanf(tmp_str + i + 3, "%hhu%n", &num_motors, &num_consumed);
		if (num_read != 1) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}

		int total_num_consumed = 3 + num_consumed;
		bool new_num_motors = steppers_controller.num_motors != num_motors;
		steppers_controller.set_num_motors(num_motors);
		for (int k = 0; k < num_motors; k++) {
			uint8_t _step_pin, _dir_pin, _enable_pin;
			int num_read = sscanf(tmp_str + i + total_num_consumed, "%hhu%hhu%hhu%n", &_dir_pin, &_step_pin, &_enable_pin, &num_consumed);
			if (num_read != 3) {
				strcpy(tmp_serial_out, "E#");
				i += 2;
				return;
			}
			if (new_num_motors)
				steppers_controller.set_pins(k, _dir_pin, _step_pin, _enable_pin);
			total_num_consumed += num_consumed + 1;
		}
		steppers_controller.disable_all();
		strcpy(tmp_serial_out, "CS#");
		i += total_num_consumed;
	}
}
//--------------------------------------------------
void parse_and_execute_create_servo_motors_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_servos_controller &servos_controller)
{
	uint8_t num_motors = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%hhu%n", &num_motors, &num_consumed);
	if (num_read != 1) {
		strcpy(tmp_serial_out, "E#");
		i += 2;
		return;
	}

	int total_num_consumed = 3 + num_consumed;
	bool new_num_motors = servos_controller.get_num_servos() != num_motors;
	servos_controller.set_num_servos(num_motors);
	for (int k = 0; k < num_motors; k++) {
		uint8_t _pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%hhu%n", &_pin, &num_consumed);
		if (num_read != 1) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}
		if (new_num_motors)
			servos_controller.set_params(k, _pin);
		total_num_consumed += num_consumed + 1;
	}
	strcpy(tmp_serial_out, "CV#");
	i += total_num_consumed;
}
//--------------------------------------------------
#ifdef USE_TB6612FNG
void parse_and_execute_create_DC_motors_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out)
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
void parse_and_execute_create_ultrasonic_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_ultrasonic_sensors_controller &ultrasonic_sensors_controller)
{
	uint8_t num_sonars = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%hhu%n", &num_sonars, &num_consumed);
	if (num_read != 1) {
		strcpy(tmp_serial_out, "E#");
		i += 2;
		return;
	}

	int total_num_consumed = 3 + num_consumed;
	ultrasonic_sensors_controller.set_num_sensors(num_sonars);
	for (int k = 0; k < num_sonars; k++) {
		uint8_t _trig_pin, _echo_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%hhu%hhu%n", &_trig_pin, &_echo_pin, &num_consumed);
		if (num_read != 2) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}
		ultrasonic_sensors_controller.set_sensor_pins(k, _trig_pin, _echo_pin);
		total_num_consumed += num_consumed + 1;
	}

	strcpy(tmp_serial_out, "CU#");
	i += total_num_consumed;
}
//--------------------------------------------------
#ifdef USE_POTENTIOMETERS
void parse_and_execute_create_potentiometer_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_potentiometers_controller &potentiometers_controller)
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
void parse_and_execute_create_as5147_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_as5147s_controller &as5147s_controller)
{
	uint8_t num_as5147 = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%hhu%n", &num_as5147, &num_consumed);
	if (num_read != 1) {
		strcpy(tmp_serial_out, "E#");
		i += 2;
		return;
	}

	int total_num_consumed = 3 + num_consumed;
	as5147s_controller.set_num_sensors(num_as5147);

	pinMode(SI, OUTPUT);
	pinMode(SO, INPUT);
	pinMode(CLK, OUTPUT);

	for (int k = 0; k < num_as5147; k++) {
		uint8_t _out_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%hhu%n", &_out_pin, &num_consumed);
		if (num_read != 1) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}
		as5147s_controller.set_params(k, _out_pin);
		total_num_consumed += num_consumed + 1;
	}

	strcpy(tmp_serial_out, "CA#");
	i += total_num_consumed;
}
//--------------------------------------------------
void parse_and_execute_create_infrared_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_infrared_analog_sensors_controller &infrared_analog_sensors_controller)
{
	uint8_t num_infrareds = 0;

	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%hhu%n", &num_infrareds, &num_consumed);
	if (num_read != 1) {
		strcpy(tmp_serial_out, "E#");
		i += 2;
		return;
	}

	int total_num_consumed = 3 + num_consumed;
	infrared_analog_sensors_controller.set_num_sensors(num_infrareds);
	for (int k = 0; k < num_infrareds; k++) {
		uint8_t out_pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%hhu%n", &out_pin, &num_consumed);
		if (num_read != 1) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}
		infrared_analog_sensors_controller.set_params(k, out_pin);
		total_num_consumed += num_consumed + 1;
	}
	strcpy(tmp_serial_out, "CI#");
	i += total_num_consumed;
}
//--------------------------------------------------
void parse_and_execute_create_buttons_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_buttons_controller &buttons_controller)
{
	uint8_t num_buttons = 0;
	int num_consumed = 0;
	int num_read = sscanf(tmp_str + i + 3, "%hhu%n", &num_buttons, &num_consumed);
	if (num_read != 1) {
		strcpy(tmp_serial_out, "E#");
		i += 2;
		return;
	}

	int total_num_consumed = 3 + num_consumed;
	buttons_controller.set_num_sensors(num_buttons);
	for (int k = 0; k < num_buttons; k++) {
		uint8_t _pin;
		int num_read = sscanf(tmp_str + i + total_num_consumed, "%hhu%n", &_pin, &num_consumed);
		if (num_read != 1) {
			strcpy(tmp_serial_out, "E#");
			i += 2;
			return;
		}
		buttons_controller.set_params(k, _pin);
		total_num_consumed += num_consumed + 1;
	}
	strcpy(tmp_serial_out, "CB#");
	i += total_num_consumed;
}
//--------------------------------------------------
void parse_and_execute_create_LiDAR_commands(char* tmp_str, byte str_length, byte& i, char* tmp_serial_out,
	t_tera_ranger_one_lidar_controller*& tera_ranger_one_lidar)
{
	int num_consumed = 0;
	uint8_t _dir_pin, _step_pin, _enable_pin, _infrared_pin;
	int num_read = sscanf(tmp_str + i + 2, "%hhu%hhu%hhu%hhu%n", &_dir_pin, &_step_pin, &_enable_pin, &_infrared_pin, &num_consumed);
	if (num_read != 4) {
		strcpy(tmp_serial_out, "E#");
		i += 2;
		return;
	}
	tera_ranger_one_lidar = new t_tera_ranger_one_lidar_controller(_dir_pin, _step_pin, _enable_pin, _infrared_pin);

	strcpy(tmp_serial_out, "CL#");
	i += num_consumed + 2;
}
//--------------------------------------------------
void parse_and_execute_commands(char* tmp_str, byte str_length, char* serial_out,
	t_steppers_controller& steppers_controller,
	t_servos_controller servos_controller,

#ifdef USE_TB6612FNG
	t_dc_motors_controller_TB6612FNG& dc_motors_controller_TB6612FNG,
#endif

#ifdef USE_POTENTIOMETERS
	t_potentiometers_controller& potentiometers_controller,
#endif
	t_as5147s_controller& as5147s_controller,
	t_ultrasonic_sensors_controller& ultrasonic_sensors_controller,
	t_infrared_analog_sensors_controller& infrared_analog_sensors_controller,
	t_buttons_controller& buttons_controller,
	t_tera_ranger_one_controller& tera_ranger_one_controller,
	t_tera_ranger_one_lidar_controller* tera_ranger_one_lidar,
	const char* firmware_version
)
{
	byte i = 0;
	serial_out[0] = 0;
	char tmp_serial_out[100];
	tmp_serial_out[0] = 0;

	while (i < str_length) {
		// can be more than 1 command in a string, so I have to check again for a letter
		if ((tmp_str[i] >= 'A' && tmp_str[i] <= 'Z') || (tmp_str[i] >= 'a' && tmp_str[i] <= 'z')) {

			if (tmp_str[i] == 'S' || tmp_str[i] == 's') { // stepper motor
				parse_and_execute_stepper_motor_commands(tmp_str, str_length, i, tmp_serial_out, 
					steppers_controller, as5147s_controller, buttons_controller);
				strcat(serial_out, tmp_serial_out);
				continue;
			}

			// dc motor
#ifdef USE_TB6612FNG
			if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // DC motor
				parse_and_execute_DC_motor_commands(tmp_str, str_length, i, tmp_serial_out, dc_motors_controller_TB6612FNG);
				strcat(serial_out, tmp_serial_out);
				continue;
			}
#endif
			if (tmp_str[i] == 'R' || tmp_str[i] == 'r') {
				parse_and_execute_read_commands(tmp_str, str_length, i, tmp_serial_out, 
					as5147s_controller, ultrasonic_sensors_controller, infrared_analog_sensors_controller, buttons_controller,
					tera_ranger_one_controller);
				strcat(serial_out, tmp_serial_out);
				continue;
			} // end reading
			// LIDAR
			if (tmp_str[i] == 'L' || tmp_str[i] == 'l') {// lidar operations
				parse_and_execute_LiDAR_commands(tmp_str, str_length, i, tmp_serial_out, tera_ranger_one_lidar);
				strcat(serial_out, tmp_serial_out);
				continue;
			}

			/*
			// go home
			  else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // go DC home
				int num_read = sscanf(tmp_str + i + 2, "%d", &motor_index);
				dc_motors_controller_TB6612FNG.go_home(motor_index, &buttons_controller);
			  }
			 // is_command_running = 1;
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
				parse_and_execute_attach_sensors_commands(tmp_str, str_length, i, tmp_serial_out, steppers_controller);
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
					parse_and_execute_stepper_motor_commands(tmp_str, str_length, j, tmp_serial_out, 
						steppers_controller, as5147s_controller, buttons_controller);
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
					parse_and_execute_create_stepper_motors_commands(tmp_str, str_length, i, tmp_serial_out, steppers_controller);
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
					parse_and_execute_create_servo_motors_commands(tmp_str, str_length, i, tmp_serial_out, servos_controller);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
				// create a list of ultrasonic sensors
				if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') {
					parse_and_execute_create_ultrasonic_commands(tmp_str, str_length, i, tmp_serial_out, 
						ultrasonic_sensors_controller);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
#ifdef USE_POTENTIOMETERS
				// create a list of potentiometers
				if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') {
					parse_and_execute_create_potentiometer_commands(tmp_str, str_length, i, tmp_serial_out, 
						potentiometers_controller);
					strcat(serial_out, tmp_serial_out);
					continue;
				}
#endif
				// create a list of AS5147
				if (tmp_str[i + 1] == 'A' || tmp_str[i + 1] == 'a') {
					parse_and_execute_create_as5147_commands(tmp_str, str_length, i, tmp_serial_out, as5147s_controller);
					strcat(serial_out, tmp_serial_out);
					continue;
				}

				// create a list of infrared sensors
				if (tmp_str[i + 1] == 'I' || tmp_str[i + 1] == 'i') {
					parse_and_execute_create_infrared_commands(tmp_str, str_length, i, tmp_serial_out, infrared_analog_sensors_controller);
					strcat(serial_out, tmp_serial_out);
					continue;
				}

				// create a list of button sensors
				if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') {
					parse_and_execute_create_buttons_commands(tmp_str, str_length, i, tmp_serial_out, buttons_controller);
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
					parse_and_execute_create_LiDAR_commands(tmp_str, str_length, i, tmp_serial_out, tera_ranger_one_lidar);
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
