#ifndef dc_motors_controller_TB6612FNG_H
#define dc_motors_controller_TB6612FNG_H

#include "Arduino.h"
#include "dc_motor_controller_TB6612FNG.h"
#include "potentiometers_controller.h"

//---------------------------------------------------
class t_dc_motors_controller_TB6612FNG
{
public:
	//Declare pin functions on Arduino

  t_dc_motor_controller_TB6612FNG *motors;
  byte num_motors;

	byte default_speed; //maximum steps per second 
	
public:
	t_dc_motors_controller_TB6612FNG(void);
  ~t_dc_motors_controller_TB6612FNG();

  void set_motor_pins(byte motor_index, byte pwm_pin, byte _dir1, byte _dir2, byte _enable);

  byte get_num_motors(void);
  void set_num_motors(int value);
  
	void move_motor(byte motor_index, long num_millis);
	
	void set_speed(byte motor_index, byte motor_speed);
	
  void get_speed(byte motor_index, byte *motor_speed);
	
	void disable(byte motor_index);

  void set_num_attached_sensors(byte motor_index, byte num_sensors);
  void get_num_attached_sensors(byte motor_index, byte *num_sensors);

	void add_sensor(byte motor_index, byte sensor_type, byte sensor_index);

  void get_sensor(byte motor_index, byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index);

  void set_running(byte motor_index, byte is_running);
  void go_home(byte motor_index, t_buttons_controller *buttons_control);
  byte is_running(byte motor_index);
  byte is_running(void);

  void update_motors(t_buttons_controller *buttons_control, char* serial_out);
};
//---------------------------------------------------
#endif
