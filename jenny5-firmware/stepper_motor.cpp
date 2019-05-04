// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#include "stepper_motor.h"
#include "jenny5_types.h"
#include "utils.h"

//-------------------------------------------------------------------------------
t_stepper_motor_controller::t_stepper_motor_controller(void)
{
	//  dir_pin = 2;
	//  step_pin = 3;
	enable_pin = 4;

	sensors_count = 0;
	sensors = NULL;

	motor_running = 0;
	stepper = NULL;

	going_to_position = false;
	sensor_stop_position = NULL;
}
//-------------------------------------------------------------------------------
t_stepper_motor_controller::~t_stepper_motor_controller(void)
{
	if (stepper)
		delete stepper;

	if (sensors)
		delete[] sensors;

	if (sensor_stop_position)
		delete[] sensor_stop_position;

	sensors_count = 0;

	motor_running = 0;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::create_init(byte _dir, byte _step, byte _enable, float default_motor_speed, float default_motor_acceleration)
{
	enable_pin = _enable;

	if (stepper)
		delete stepper;

	if (sensors)
		delete[] sensors;

	if (sensor_stop_position)
		delete[] sensor_stop_position;

	sensors_count = 0;

	motor_running = 0;
	going_to_position = false;

	stepper = new SpeedyStepper();
	stepper->connectToPins(_step, _dir);
	stepper->setSpeedInStepsPerSecond(default_motor_speed);
	stepper->setAccelerationInStepsPerSecondPerSecond(default_motor_acceleration);
	stepper->setCurrentPositionInSteps(0);

	digitalWrite(_step, LOW);
	digitalWrite(_dir, LOW);
	pinMode(enable_pin, OUTPUT);
	digitalWrite(enable_pin, HIGH); // all motors are disabled now
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::move_motor(int num_steps)
{
	stepper->setCurrentPositionInSteps(0);
	digitalWrite(enable_pin, LOW); // turn motor on
	stepper->setupMoveInSteps(num_steps); //move num_steps
	motor_running = 1;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::move_motor_to(int _position)
{
	digitalWrite(enable_pin, LOW); // turn motor on
	stepper->setupMoveInSteps(_position); //move num_steps; !!!!!!!!!!!!!!!!!!!!! this is not absolute
	motor_running = 1;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::set_motor_speed(float _motor_speed)
{
	stepper->setSpeedInStepsPerSecond(_motor_speed);
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::set_motor_acceleration(float _motor_acceleration)
{
	stepper->setAccelerationInStepsPerSecondPerSecond(_motor_acceleration);
	// motor_acceleration = _motor_acceleration;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::set_motor_speed_and_acceleration(float _motor_speed, float _motor_acceleration)
{
	stepper->setSpeedInStepsPerSecond(_motor_speed);
	stepper->setAccelerationInStepsPerSecondPerSecond(_motor_acceleration);
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::disable_motor(void)
{
	digitalWrite(enable_pin, HIGH); // disable motor
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::lock_motor(void)
{
	digitalWrite(enable_pin, LOW); // enable motor
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::add_sensor(byte sensor_type, byte sensor_index, int _min_pos, int _max_pos, int _home_pos, int8_t __direction)
{
	sensors[sensors_count].type = sensor_type;
	sensors[sensors_count].index = sensor_index;

	sensors[sensors_count].min_pos = _min_pos;
	sensors[sensors_count].max_pos = _max_pos;
	sensors[sensors_count].home_pos = _home_pos;
	sensors[sensors_count]._direction = __direction;
	sensors_count++;
}
//-------------------------------------------------------------------------------
byte t_stepper_motor_controller::get_num_attached_sensors(void)
{
	return sensors_count;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::set_num_attached_sensors(byte num_sensors)
{
	if (sensors_count != num_sensors) {
		// clear memory if the array has a different size
		if (sensors) {
			delete[] sensors;
			sensors = NULL;
		}

		if (sensor_stop_position) {
			delete[] sensor_stop_position;
			sensor_stop_position = NULL;
		}

		if (num_sensors > 0) {
			sensors = new t_sensor_info[num_sensors]; // allocate memory for them
			sensor_stop_position = new int[num_sensors];
		}
	}
	sensors_count = 0; // actual number of sensors
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::set_motor_running(byte is_running)
{
	motor_running = is_running;
}
//-------------------------------------------------------------------------------
byte t_stepper_motor_controller::is_motor_running(void)
{
	return motor_running;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::get_sensor(byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index, int *_min_pos, int *_max_pos, int *_home_pos, int8_t *__direction)
{
	*sensor_type = sensors[sensor_index_in_motor_list].type;
	*sensor_index = sensors[sensor_index_in_motor_list].index;

	*_min_pos = sensors[sensor_index_in_motor_list].min_pos;
	*_max_pos = sensors[sensor_index_in_motor_list].max_pos;
	*_home_pos = sensors[sensor_index_in_motor_list].home_pos;
	*__direction = sensors[sensor_index_in_motor_list]._direction;
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::get_motor_speed_and_acceleration(float *_motor_speed, float *_motor_acceleration)
{
	if (stepper) {
		*_motor_acceleration = 0;
		*_motor_speed = 0;
	}
	else {
		*_motor_speed = 0;
		*_motor_acceleration = 0;
	}
}
//-------------------------------------------------------------------------------
byte t_stepper_motor_controller::run_motor(t_as5147s_controller *as5147s_controller, 
#ifdef USE_POTENTIOMETERS
	t_potentiometers_controller *potentiometers_control,
#endif
	t_buttons_controller *buttons_controller, int& dist_left_to_go)
{
	// returns MOTOR_STILL_RUNNING if is still running
	// returns MOTOR_DOES_NOTHING if it does nothing
	// returns MOTOR_JUST_STOPPED if it has just stopped; in dist_to_go we have what is left to run (or 0)

	bool limit_reached = false;
	long distance_to_go = stepper->get_distanceToGo();

	if (distance_to_go) {
		for (byte j = 0; j < sensors_count; j++) {
			byte sensor_index = sensors[j].index;
			byte sensor_type = sensors[j].type;

			switch (sensor_type) {
#ifdef USE_POTENTIOMETERS
			case POTENTIOMETER:
			{
				int8_t potentiometer_direction = sensors[j]._direction;

				int val = potentiometers_control->get_position(sensor_index);

				if (sensors[j].min_pos > val) {
					if (distance_to_go * potentiometer_direction < 0) {
						limit_reached = true;
					}
				}
				if (sensors[j].max_pos < val) {
					if (distance_to_go * potentiometer_direction > 0) {
						limit_reached = true;
					}
				}

				if (going_to_position) {
					// must stop to home
					int pot_position = potentiometers_control->get_position(sensor_index);
					int pot_stop_position = sensor_stop_position[j];
					int8_t pot_direction = sensors[j]._direction;

					if (pot_direction == 1) {
						if (distance_to_go > 0) {
							if (pot_position >= pot_stop_position)
								limit_reached = true;
						}
						else { // distance to go is negative 
							if (pot_position <= pot_stop_position)
								limit_reached = true;
						}
					}
					else {// pot direction == -1
						if (distance_to_go > 0) {
							if (pot_position <= pot_stop_position)
								limit_reached = true;
						}
						else { // distance to go is negative 
							if (pot_position >= pot_stop_position)
								limit_reached = true;
						}
					}
				}
			} // end case potentiometer
			break;
#endif
			case AS5147:
			{
				int8_t as5147_direction = sensors[j]._direction;

				int as5147_current_position = as5147s_controller->get_position(sensor_index);

				if (as5147_direction == 1) { // motor and sensors values have the same sign
					if (distance_to_go > 0) {
						if (sensors[j].min_pos < sensors[j].max_pos) {
							if (as5147_current_position > sensors[j].max_pos)
								limit_reached = true;
							else
								; // no limit reached ... so do nothing
						}
						else { // max < min ... so it overides 360
							if (as5147_current_position > sensors[j].max_pos &&
								abs(as5147_current_position - sensors[j].max_pos) < abs(as5147_current_position - sensors[j].min_pos))
								limit_reached = true;
							else
								; // no limit reached ... so do nothing
						}
					}
					else
						if (distance_to_go < 0) {
							if (sensors[j].min_pos < sensors[j].max_pos) {
								if (as5147_current_position < sensors[j].min_pos)
									limit_reached = true;
								else
									; // no limit reached ... so do nothing
							}
							else { // max < min ... so it overides 360
								if (as5147_current_position < sensors[j].min_pos &&
									abs(as5147_current_position - sensors[j].min_pos) < abs(as5147_current_position - sensors[j].max_pos))
									limit_reached = true;
								else
									; // no limit reached ... so do nothing
							}
						}
				}
				else { // as5147_direction == -1 // motor and sensors values have different signs
					if (distance_to_go > 0) {// if motors increase, the sensor decreases
						if (sensors[j].min_pos < sensors[j].max_pos) {
							if (as5147_current_position < sensors[j].min_pos)
								limit_reached = true;
							else
								; // no limit reached ... so do nothing
						}
						else { // max < min ... so it overides 360
							if (as5147_current_position < sensors[j].min_pos &&
								abs(as5147_current_position - sensors[j].min_pos) < abs(as5147_current_position - sensors[j].max_pos))
								limit_reached = true;
							else
								; // no limit reached ... so do nothing
						}
					}
					else
						if (distance_to_go < 0) {
							if (sensors[j].min_pos < sensors[j].max_pos) {
								if (as5147_current_position > sensors[j].max_pos)
									limit_reached = true;
								else
									; // no limit reached ... so do nothing
							}
							else { // max < min ... so it overides 360
								if (as5147_current_position > sensors[j].max_pos &&
									abs(as5147_current_position - sensors[j].max_pos) < abs(as5147_current_position - sensors[j].min_pos))
									limit_reached = true;
								else
									; // no limit reached ... so do nothing
							}
						}
				}
					/*
				if (as5147_position < sensors[j].min_pos) {
					if (distance_to_go * as5147_direction < 0) {
						limit_reached = true;
					}
				}
				if (as5147_position > sensors[j].max_pos) {
					if (distance_to_go * as5147_direction > 0) {
						limit_reached = true;
					}
				}
				*/
				if (going_to_position) {
					// must stop to home
					//int as5147_position = as5147s_controller->get_position(sensor_index);
					int as5147_stop_position = sensor_stop_position[j]; // sensors[j].home_pos;// potentiometers_control->get_home_position(sensor_index);
					//int8_t pot_direction = sensors[j]._direction;// potentiometers_control->get_direction(sensor_index);

					if (as5147_direction == 1) {
						if (distance_to_go > 0) {
							if (as5147_current_position >= as5147_stop_position)
								limit_reached = true;
						}
						else { // distance to go is negative 
							if (as5147_current_position <= as5147_stop_position)
								limit_reached = true;
						}
					}
					else {// sensor direction == -1
						if (distance_to_go > 0) {
							if (as5147_current_position <= as5147_stop_position)
								limit_reached = true;
						}
						else { // distance to go is negative 
							if (as5147_current_position >= as5147_stop_position)
								limit_reached = true;
						}
					}
				}
			}
			break;

			case BUTTON:
			{
				int button_direction = sensors[j]._direction;
				int button_state = buttons_controller->get_state(sensor_index);

				if (button_state == 1) // limit reached
					if (distance_to_go * button_direction > 0)
						limit_reached = true;
			}
			break;
			}// end switch
		} // end for (byte j = 0; j < sensors_count; j++)

		if (!limit_reached) {
			stepper->processMovement();
			return MOTOR_STILL_RUNNING; // still running
		}
		else {
			if (is_motor_running()) {
				long to_go = stepper->get_distanceToGo();
				stepper->stop_motor();
				dist_left_to_go = to_go;
				going_to_position = false;
				//Serial.println("left to go  > 0");
				set_motor_running(0);
				return MOTOR_JUST_STOPPED;
			}
			else
				return MOTOR_DOES_NOTHING;
		}
	} // end if (distance_to_go)
	else {
		// the motor has just finished the move, so we output that event
		going_to_position = false;
		if (is_motor_running()) {
			set_motor_running(0);
			dist_left_to_go = 0;
			//Serial.println("left to go == 0");
			return MOTOR_JUST_STOPPED; // distance to go
		}
		else
			return MOTOR_DOES_NOTHING;
	}
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::go_home(t_as5147s_controller *as5147s_controller, 
#ifdef USE_POTENTIOMETERS
	t_potentiometers_controller *potentiometers_control, 
#endif
	t_buttons_controller* buttons_controller)
{
	if (sensors_count > 0) {
		byte sensor_index = sensors[0].index;
		byte sensor_type = sensors[0].type;

		switch (sensor_type) {
#ifdef USE_POTENTIOMETERS
		case POTENTIOMETER: {
			//calculate the remaining distance from the current position to home position, relative to the direction and position of the potentiometer
			int8_t pot_dir = sensors[0]._direction; // potentiometers_control->get_direction(sensor_index);
			int pot_home = sensors[0].home_pos; // potentiometers_control->get_home_position(sensor_index);
			int pot_pos = potentiometers_control->get_position(sensor_index);
			int max_steps_to_home = pot_dir * sign(pot_home - pot_pos) * 32000;
			going_to_position = true;
			sensor_stop_position[0] = pot_home;
			move_motor(max_steps_to_home);
		}
							break;
#endif
		case AS5147: {
			//calculate the remaining distance from the current position to home position, relative to the direction and position of the potentiometer
			int8_t as5147_dir = sensors[0]._direction;
			int as5147_home = sensors[0].home_pos;
			int as5147_current_pos = as5147s_controller->get_position(sensor_index);
			int max_steps_to_home = as5147_dir * sign(as5147_home - as5147_current_pos) * 32000;
			going_to_position = true;
			sensor_stop_position[0] = as5147_home;
			move_motor(max_steps_to_home);
		}
					 break;
		case BUTTON: {
			int8_t b_direction = sensors[0]._direction;//buttons_controller->get_direction(sensor_index);
			int max_steps_to_home;
			if (b_direction > 0)
				max_steps_to_home = 32000; // this depends on microstepping, gear redunction etc
			else
				max_steps_to_home = -32000;
			going_to_position = true;
			move_motor(max_steps_to_home);
		}
					 break;
		}// end switch
	}
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::go_to_sensor_position(
	t_as5147s_controller *as5147s_controller,
#ifdef USE_POTENTIOMETERS
	t_potentiometers_controller *potentiometers_control, 
#endif
	int _stop_position)
{
	if (sensors_count > 0) {
		byte sensor_type = sensors[0].type;
		byte sensor_index = sensors[0].index;

		switch (sensor_type) {
#ifdef USE_POTENTIOMETERS
		case POTENTIOMETER: {
			//calculate the remaining distance from the current position to home position, relative to the direction and position of the potentiometer
			int8_t pot_dir = sensors[0]._direction; // potentiometers_control->get_direction(sensor_index);
			//int pot_home = sensors[0].home_pos; // potentiometers_control->get_home_position(sensor_index);
			int pot_pos = potentiometers_control->get_position(sensor_index);

			int max_steps_to_home = pot_dir * sign(-pot_pos + pot_stop_position) * 32000;
			//Serial.println(pot_dir);
			//Serial.println(pot_home);
			//Serial.println(pot_stop_position);
	  //	  Serial.println(distance_to_home);
			going_to_position = true;
			sensor_stop_position[0] = pot_stop_position;
			//Serial.println(max_steps_to_home);
			move_motor(max_steps_to_home);
		}
							break;
#endif
		case AS5147: {
			//calculate the remaining distance from the current position to home position, relative to the direction and position of the potentiometer
			int8_t as5147_dir = sensors[0]._direction; 
			//int pot_home = sensors[0].home_pos; // potentiometers_control->get_home_position(sensor_index);
			int as5147_current_pos = as5147s_controller->get_position(sensor_index);

			int max_steps_to_home = as5147_dir * sign(_stop_position - as5147_current_pos) * 32000;
			//Serial.println(pot_dir);
			//Serial.println(pot_home);
			//Serial.println(pot_stop_position);
	  //	  Serial.println(distance_to_home);
			going_to_position = true;
			sensor_stop_position[0] = _stop_position;
			//Serial.println(max_steps_to_home);
			move_motor(max_steps_to_home);
		}
			break;
		} // end switch
	}
}
//-------------------------------------------------------------------------------
void t_stepper_motor_controller::stop(void)
{
	stepper->stop_motor();
}
//-------------------------------------------------------------------------------