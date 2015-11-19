#include "Arduino.h"
#include "AccelStepper.h"

class t_motors_control
{
public:
	//Declare pin functions on Arduino
	byte *m_dir_pins;
	byte *m_step_pins;
	byte *m_enable_pins;

	int default_motor_speed; //maximum steps per second 
	int default_motor_acceleration; //steps/second/second to accelerate

  byte num_motors;
  
	AccelStepper **steppers;
	
public:
	t_motors_control(byte _num_motors, byte *dir_pins, byte *step_pins, byte *enable_pins);
	
	void move_motor(byte motor_index, int num_steps);
	
	void set_motor_speed(byte motor_index, int motor_speed);
	
	void set_motor_acceleration(byte motor_index, int motor_acceleration);
	
	void disable_motor(byte motor_index);
	
	void lock_motor(byte motor_index);
	
	//Reset pins to default states
	void reset_pins();
};
