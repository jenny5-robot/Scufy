#include <AccelStepper.h>
#include <Arduino.h>

#define num_motors 4

class motor_control
{
public:
	//Declare pin functions on Arduino
	int dir_pins[num_motors];
	int step_pins[num_motors];
	int enable_pins[4];

	int default_motor_speed; //maximum steps per second 
	int default_motor_acceleration; //steps/second/second to accelerate
	
	AccelStepper *steppers[num_motors];
	
public:
	motor_control();
	
	void move_motor(int motor_index, int num_steps);
	
	void set_motor_speed(int motor_index, int motor_speed);
	
	void set_motor_acceleration(int motor_index, int motor_acceleration);
	
	void disable_motor(int motor_index);
	
	void lock_motor(int motor_index);
	
	//Reset pins to default states
	void reset_pins();
};