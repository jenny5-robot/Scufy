#include "tera_ranger_one_lidar.h"

//-----------------------------------------------------------------------------
t_tera_ranger_one_lidar::t_tera_ranger_one_lidar(byte motor_dir_pin, byte motor_step_pin, byte motor_enable_pin, byte infrared_pin)
{
	started = false;
	infrared_read_first_time = false;

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
	infrared_read_first_time = false;
	request_for_distance_sent = false;

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
    
		if (request_for_distance_sent) {
    //  Serial.write("3");
			// no more steps until lidar sends a distance back
			if (tera_ranger_one.check_echo()) 
			{
				request_for_distance_sent = false;

				// here I have to output a string
				sprintf(serial_out, "L%d %d#", motor_position, tera_ranger_one.get_last_read_distance()); //tera_ranger_one.get_last_read_distance());
			}
		}
		else {
			// no request for distance yet
			if (!infrared_sensor.get_value()) {
        // infrared has detected an obstacle
				//Serial.write("4 ");
				infrared_read_first_time = true;
				motor_position = 0;
			}
			if (!infrared_read_first_time) {
			//	Serial.write("1 ");
				stepper->run();
			}
			else {
				//Serial.write("2 ");
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
		}
	}
}
//-----------------------------------------------------------------------------
