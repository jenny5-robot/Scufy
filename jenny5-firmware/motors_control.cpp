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
