#include "motor_control.h"

motor_control::motor_control()
{
	dir_pins[0] = 2;
	dir_pins[1] = 5;
	dir_pins[2] = 8;
	dir_pins[3] = 11;
	
	step_pins[0] = 3;
	step_pins[1] = 6;
	step_pins[2] = 9;
	step_pins[3] = 12;
	
	enable_pins[0] = 4;
	enable_pins[1] = 7;
	enable_pins[2] = 10;
	enable_pins[3] = 13;
	
	default_motor_speed = 200;
	default_motor_acceleration = 100;
	
	for (int m = 0; m < num_motors; m++)
	{
		steppers[m] = new AccelStepper(AccelStepper::DRIVER, step_pins[m], dir_pins[m]);
		steppers[m]->setMaxSpeed(default_motor_speed);
		steppers[m]->setSpeed(default_motor_speed);
		steppers[m]->setAcceleration(default_motor_acceleration);
	}
}

void motor_control::move_motor(int motor_index, int num_steps)
{
  digitalWrite(enable_pins[motor_index], LOW); // turn motor on
  steppers[motor_index]->moveTo(num_steps); //move num_steps
}

void motor_control::set_motor_speed(int motor_index, int motor_speed)
{
	steppers[motor_index]->setMaxSpeed(motor_speed);
	steppers[motor_index]->setSpeed(motor_speed);
}

void motor_control::set_motor_acceleration(int motor_index, int motor_acceleration)
{
	steppers[motor_index]->setAcceleration(motor_acceleration);
}

void motor_control::disable_motor(int motor_index)
{
  digitalWrite(enable_pins[motor_index], HIGH); // disable motor
}

void motor_control::lock_motor(int motor_index)
{
  digitalWrite(enable_pins[motor_index], LOW); // enable motor
}

//Reset pins to default states
void motor_control::reset_pins()
{
  for (int m = 0; m < num_motors; m++){
	digitalWrite(step_pins[m], LOW);
	digitalWrite(dir_pins[m], LOW);
	digitalWrite(enable_pins[m], HIGH); // all motors are disabled now
  }
}