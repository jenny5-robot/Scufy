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
  sensors = new t_sensor_array[num_motors];

	for (byte m = 0; m < num_motors; m++)
	{
		steppers[m] = new AccelStepper(AccelStepper::DRIVER, step_pins[m], dir_pins[m]);
		steppers[m]->setMaxSpeed(default_motor_speed);
		steppers[m]->setSpeed(default_motor_speed);
		steppers[m]->setAcceleration(default_motor_acceleration);
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
void t_motors_control::set_motor_speed_and_acceleration(byte motor_index, int motor_speed, int motor_acceleration)
{
  steppers[motor_index]->setMaxSpeed(motor_speed);
  steppers[motor_index]->setSpeed(motor_speed);
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
//-------------------------------------------------------------------------------
void t_motors_control::add_sensor(byte motor_index, byte sensor_type, byte sensor_index)
{
	sensors[motor_index].sensors_array[sensors[motor_index].count].sensor_type = sensor_type;
	sensors[motor_index].sensors_array[sensors[motor_index].count].index = sensor_index;
  sensors[motor_index].count++;
}
//-------------------------------------------------------------------------------
/*
void t_motors_control::remove_all_sensors(byte motor_index)
{
	memset(sensor_data[motor_index], 0, MAX_SENSORS_PER_MOTOR * sizeof(t_sensor_data));
}
*/
//-------------------------------------------------------------------------------
void t_motors_control::set_num_attached_sensors(byte motor_index, byte num_sensors)
{
  if (sensors[motor_index].count != num_sensors){
    // clear memory if the array has a different size
    if (sensors[motor_index].sensors_array){
      delete[] sensors[motor_index].sensors_array;
      sensors[motor_index].sensors_array = NULL;
    }

    if (num_sensors > 0){
      sensors[motor_index].sensors_array = new t_sensor_info[num_sensors]; // allocate memory for them
    }
  }
  sensors[motor_index].count = 0; // actual number of sensors
}
//-------------------------------------------------------------------------------
/*
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
*/
//-------------------------------------------------------------------------------

