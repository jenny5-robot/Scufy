#ifndef stepper_motors_controller_H
#define stepper_motors_controller_H

#include "Arduino.h"
#include "stepper_motor_controller.h"
#include "potentiometers_controller.h"

//#define MAX_SENSORS_PER_MOTOR 6

//---------------------------------------------------
class t_stepper_motors_controller
{
public:
	//Declare pin functions on Arduino

  t_stepper_motor_controller *motors;
  byte num_motors;

	float default_motor_speed; //maximum steps per second 
	float default_motor_acceleration; //steps/second/second to accelerate
	
public:
	t_stepper_motors_controller(void);
  ~t_stepper_motors_controller();

  void set_motor_pins(byte motor_index, byte _dir, byte _step, byte _enable);

  byte get_num_motors(void);
  void set_num_motors(int value);
  
	void move_motor(byte motor_index, int num_steps);
  void move_motor_to(byte motor_index, int _position);
	
	void set_motor_speed(byte motor_index, float motor_speed);
	
	void set_motor_acceleration(byte motor_index, float motor_acceleration);

  void set_motor_speed_and_acceleration(byte motor_index, float motor_speed, float motor_acceleration);
  void get_motor_speed_and_acceleration(byte motor_index, float *motor_speed, float *motor_acceleration);
	
	void disable_motor(byte motor_index);
	
	void lock_motor(byte motor_index);

  void set_num_attached_sensors(byte motor_index, byte num_sensors);
  void get_num_attached_sensors(byte motor_index, byte *num_sensors);

	void add_sensor(byte motor_index, byte sensor_type, byte sensor_index);

  void get_sensor(byte motor_index, byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index);

  void set_motor_running(byte motor_index, byte is_running);
  void go_home(byte motor_index, t_potentiometers_controller *potentiometers_control);
  byte is_motor_running(byte motor_index);
  byte is_motor_running(void);

  void run_motors(t_potentiometers_controller *potentiometers_control, char* serial_out);
};
//---------------------------------------------------
#endif
