// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#include "tera_ranger_one_lidar.h"

//-----------------------------------------------------------------------------
t_tera_ranger_one_lidar::t_tera_ranger_one_lidar(byte motor_dir_pin, byte motor_step_pin, byte motor_enable_pin, byte infrared_pin)
{
	started = false;
	reference_touched = false;
	first_reference_touched = false;

	enable_pin = motor_enable_pin;

	stepper = new AccelStepper(AccelStepper::DRIVER, motor_step_pin, motor_dir_pin);
	stepper->setMaxSpeed(200);
	stepper->setSpeed(200);
	stepper->setAcceleration(100);

	// reset pins
	digitalWrite(motor_step_pin, LOW);
	digitalWrite(motor_dir_pin, LOW);
	digitalWrite(enable_pin, HIGH); // motor is disabled now

	infrared_sensor.set_params(infrared_pin);

	tera_ranger_one.create_init();
}
//-------------------------------------------------------------------------------
void t_tera_ranger_one_lidar::set_motor_speed_and_acceleration(float _motor_speed, float _motor_acceleration)
{
	stepper->setMaxSpeed(_motor_speed);
	stepper->setSpeed(_motor_speed);
	stepper->setAcceleration(_motor_acceleration);
}
//-----------------------------------------------------------------------------
void t_tera_ranger_one_lidar::start(void)
{
	digitalWrite(enable_pin, LOW); // turn motor on
	stepper->move(400); //move num_steps
	reference_touched = false;
	request_for_distance_sent = false;
	first_reference_touched = false;
	prev_reference_touched = false;

	motor_position = 0;
	started = true;
}
//-----------------------------------------------------------------------------
void t_tera_ranger_one_lidar::stop(void)
{
	digitalWrite(enable_pin, HIGH); // turn motor off

	started = false;
}
//-----------------------------------------------------------------------------
void t_tera_ranger_one_lidar::run_motor(char *serial_out)
{
	serial_out[0] = 0;

	if (started) {

		if (!infrared_sensor.get_value()) {
			// infrared has detected an obstacle (the reference point)
			//	Serial.write("4");
			prev_reference_touched = reference_touched;
			reference_touched = true;
			
			//motor_position = 0;
		}
		else {
			reference_touched = false;
			if (prev_reference_touched) {
				prev_reference_touched = false;
				motor_position = 0;
				first_reference_touched = true;
			}
		}

		if (first_reference_touched) {
			if (request_for_distance_sent) {
				// Serial.write("3");
				// no more steps until lidar sends a distance back
				if (tera_ranger_one.check_echo()) {
					request_for_distance_sent = false;

					// here I have to output a string
					sprintf(serial_out, "L%d %d#", motor_position, tera_ranger_one.get_last_read_distance()); //tera_ranger_one.get_last_read_distance());
				}
			}

			long prev_dist_to_go = stepper->distanceToGo();
			stepper->run();
			long curr_dist_to_go = stepper->distanceToGo();

			long traveled_distance = prev_dist_to_go - curr_dist_to_go;
			//Serial.print(traveled_distance);
			if (traveled_distance) {
				// now I have to read the distance
				tera_ranger_one.trigger();

				stepper->move(400); //move num_steps
				request_for_distance_sent = true;
				motor_position += traveled_distance;
			}
		}
		else
			stepper->run();
	}
}
//-----------------------------------------------------------------------------
