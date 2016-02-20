#ifndef motor_controller_H
#define motor_controller_H

#include "Arduino.h"
#include "buttons_controller.h"
#include "jenny5_types.h"

//---------------------------------------------------
class t_dc_motor_controller_TB6612FNG
{
public:
  //Declare pin functions on Arduino

  byte pwm_pin;
  byte dir_pin1, dir_pin2;
  byte speed_pin;// must be pwm
  byte enable_pin;

  unsigned long time_to_go;
  unsigned long start_time;
  byte motor_speed;
  
  t_sensor_info  *sensors;
  byte sensors_count;
  
  byte motor_running;  
public:
  t_dc_motor_controller_TB6612FNG(void);
  ~t_dc_motor_controller_TB6612FNG(void);
  
  void create_init(byte pwm_pin, byte _dir1, byte _dir2, byte _enable, byte _speed);
  
  void move_motor(unsigned long num_millis);
  
  void set_speed(byte motor_speed);
  void get_speed(byte *motor_speed);
  
  void disable(void);

  void set_num_attached_sensors(byte num_sensors);
  void get_num_attached_sensors(byte *num_sensors);

  void add_sensor(byte sensor_type, byte sensor_index);

  void get_sensor(byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index);

  void set_running(byte is_running);
  void go_home(t_buttons_controller *buttons_controller);
  byte is_running(void);
  
  long update_motor(t_buttons_controller *buttons_controller);
};
//---------------------------------------------------

#endif
