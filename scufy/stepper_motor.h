// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef stepper_motor_controller_H
#define stepper_motor_controller_H

#include "config.h"

#include "Arduino.h"
#include "SpeedyStepper.h"
#ifdef USE_POTENTIOMETERS
#include "potentiometers.h"
#endif
#include "infrared_analog_sensors.h"
#include "buttons.h"
#include "jenny5_types.h"
#include "as5147s.h"

//---------------------------------------------------
class t_stepper_motor_controller
{
public:
  //Declare pin functions on Arduino
  
  byte enable_pin;
  
  t_sensor_info  *sensors;
  byte sensors_count;

  int *sensor_stop_position;
  
  SpeedyStepper stepper;
  byte motor_running;

  bool going_to_position;

public:
  t_stepper_motor_controller(void);
  ~t_stepper_motor_controller(void);
  
  void create_init(byte _dir, byte _step, byte _enable, float default_motor_speed, float default_motor_acceleration);
  
  void move_motor(int num_steps);
  void move_motor_to(int _position);
  
  void set_motor_speed(float motor_speed);
  
  void set_motor_acceleration(float motor_acceleration);
// speed and acceleration are float, but we read and write them as int in order to reduce serial traffic
  void set_motor_speed_and_acceleration(float motor_speed, float motor_acceleration);
  void get_motor_speed_and_acceleration(float *motor_speed, float *motor_acceleration);
  
  void disable_motor(void);
  
  void lock_motor(void);

  void set_num_attached_sensors(byte num_sensors);
  byte get_num_attached_sensors(void);

  void add_sensor(byte sensor_type, byte sensor_index, int _min_pos, int _max_pos, int _home_pos, int8_t same_direction_as_motor);

  void get_sensor(byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index, int *_min_pos, int *_max_pos, int *_home_pos, int8_t * same_direction_as_motor);

  void set_motor_running(byte is_running);
  void go_home(t_as5147s_controller *as5147s_controller, 
#ifdef USE_POTENTIOMETERS 
	  t_potentiometers_controller *potentiometers_control, 
#endif 
	 t_buttons_controller* buttons_controller);
  
  byte is_motor_running(void);

  void go_to_sensor_position(t_as5147s_controller *as5147s_controller,
#ifdef USE_POTENTIOMETERS
	  t_potentiometers_controller *potentiometers_control, 
#endif
	  int sensor_stop_position
  );
  void stop(void);
  
  byte run_motor(t_as5147s_controller *as5147s_controller, 
#ifdef USE_POTENTIOMETERS
	  t_potentiometers_controller *potentiometers_control, 
#endif
	  t_buttons_controller* buttons_controller, int &dist_to_go);
};
//---------------------------------------------------

#endif
