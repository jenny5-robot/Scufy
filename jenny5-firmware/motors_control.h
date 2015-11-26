#include "Arduino.h"
#include "AccelStepper.h"

#define MAX_SENSORS_PER_MOTOR 6

enum t_sensor_type {
	senTypeNone = 0,
	senTypePotentiometer = 1,
	senTypeUltrasound = 2,
};

struct t_sensor_data {
	t_sensor_type 	sensor_type;
	byte			index;
};

class t_motors_control
{
public:
	//Declare pin functions on Arduino
  
	byte *dir_pins;
	byte *step_pins;
	byte *enable_pins;

	int default_motor_speed; //maximum steps per second 
	int default_motor_acceleration; //steps/second/second to accelerate

  	byte 			num_motors;
  
  	byte			*motor_sensor_count;
  	t_sensor_data	**sensor_data;
	AccelStepper 	**steppers;
	
public:
	t_motors_control(byte _num_motors);
	
	void move_motor(byte motor_index, int num_steps);
	
	void set_motor_speed(byte motor_index, int motor_speed);
	
	void set_motor_acceleration(byte motor_index, int motor_acceleration);
	
	void disable_motor(byte motor_index);
	
	void lock_motor(byte motor_index);

	void add_sensor(byte motor_index, t_sensor_type sensor_type, byte sensor_index);
	void remove_sensor(byte motor_index, t_sensor_type, byte sensor_index);
	void remove_all_sensors(byte motor_index);
	
	//Reset pins to default states
	void reset_pins();
};
