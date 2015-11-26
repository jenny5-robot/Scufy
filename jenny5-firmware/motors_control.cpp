#include "motors_control.h"
//-------------------------------------------------------------------------------
t_motors_control::t_motors_control(byte _num_motors)
{
  num_motors = _num_motors;
  
  dir_pins = new byte[num_motors];
  dir_pins[0] = 2;
	dir_pins[1] = 5;
	dir_pins[2] = 8;
	dir_pins[3] = 11;

  step_pins = new byte[num_motors];
	step_pins[0] = 3;
	step_pins[1] = 6;
	step_pins[2] = 9;
	step_pins[3] = 12;

  enable_pins = new byte[num_motors];
	enable_pins[0] = 4;
	enable_pins[1] = 7;
	enable_pins[2] = 10;
	enable_pins[3] = 13;
	
	default_motor_speed = 200;
	default_motor_acceleration = 100;

  steppers = new AccelStepper*[num_motors];
  sensor_data = new t_sensor_data*[num_motors];

  motor_sensor_count = new byte[num_motors];
  memset(motor_sensor_count, 0, sizeof(byte) * num_motors);

	for (byte m = 0; m < num_motors; m++)
	{
		steppers[m] = new AccelStepper(AccelStepper::DRIVER, step_pins[m], dir_pins[m]);
		steppers[m]->setMaxSpeed(default_motor_speed);
		steppers[m]->setSpeed(default_motor_speed);
		steppers[m]->setAcceleration(default_motor_acceleration);



		sensor_data[m] = new t_sensor_data[MAX_SENSORS_PER_MOTOR];
		memset(sensor_data[0], 0, sizeof(t_sensor_data) * MAX_SENSORS_PER_MOTOR);
	}
}
//-------------------------------------------------------------------------------
void t_motors_control::move_motor(byte motor_index, int num_steps)
{
  digitalWrite(enable_pins[motor_index], LOW); // turn motor on
  steppers[motor_index]->moveTo(num_steps); //move num_steps
}
//-------------------------------------------------------------------------------
void t_motors_control::set_motor_speed(byte motor_index, int motor_speed)
{
	steppers[motor_index]->setMaxSpeed(motor_speed);
	steppers[motor_index]->setSpeed(motor_speed);
}
//-------------------------------------------------------------------------------
void t_motors_control::set_motor_acceleration(byte motor_index, int motor_acceleration)
{
	steppers[motor_index]->setAcceleration(motor_acceleration);
}
//-------------------------------------------------------------------------------
void t_motors_control::disable_motor(byte motor_index)
{
  digitalWrite(enable_pins[motor_index], HIGH); // disable motor
}
//-------------------------------------------------------------------------------
void t_motors_control::lock_motor(byte motor_index)
{
  digitalWrite(enable_pins[motor_index], LOW); // enable motor
}
//-------------------------------------------------------------------------------
//Reset pins to default states
void t_motors_control::reset_pins()
{
  for (byte m = 0; m < num_motors; m++){
	  digitalWrite(step_pins[m], LOW);
	  digitalWrite(dir_pins[m], LOW);
	  digitalWrite(enable_pins[m], HIGH); // all motors are disabled now
  }
}


void t_motors_control::add_sensor(byte motor_index, t_sensor_type sensor_type, byte sensor_index)
{
	if (MAX_SENSORS_PER_MOTOR - 1 == this->motor_sensor_count[motor_index])
	{
		// if we reached the sensor count per motor limit, exit
		// maybe return an error code?
		return;
	}

	this->sensor_data[motor_index][++motor_sensor_count[motor_index]].sensor_type = sensor_type;
	this->sensor_data[motor_index][motor_sensor_count[motor_index]].index = sensor_index;
}

void t_motors_control::remove_sensor(byte motor_index, t_sensor_type sensor_type, byte sensor_index)
{
	for (byte i = 0 ; i < motor_sensor_count[motor_index] ; ++i)
	{
		if (sensor_data[motor_index][i].sensor_type == sensor_type &&
			sensor_data[motor_index][i].index == sensor_index)
		{
			--motor_sensor_count[motor_index];
			if (i < MAX_SENSORS_PER_MOTOR - 1)
			{
				memcpy(&sensor_data[motor_index][i], &sensor_data[motor_index][i + 1], MAX_SENSORS_PER_MOTOR - i - 1);	
				sensor_data[motor_index][MAX_SENSORS_PER_MOTOR - 1].sensor_type = senTypeNone;
			}
		}
	}
}

void t_motors_control::remove_all_sensors(byte motor_index)
{
	memset(sensor_data[motor_index], 0, MAX_SENSORS_PER_MOTOR * sizeof(t_sensor_data));
}
