#ifndef TERA_RANGER_ONE_LIDAR_H
#define TERA_RANGER_ONE_LIDAR_H

#include "tera_ranger_one.h"
#include "stepper_motor.h"
#include "infrared_digital.h"

class t_tera_ranger_one_lidar{

  private:
  t_stepper_motor_controller *motor;
  t_infrared_digital_sensor infrared;
  t_tera_ranger_one_controller tera_ranger_one;

  bool started;
  bool infrared_read_first_time;

public:
  t_tera_ranger_one_lidar(byte motor_dir_pin, byte motor_step_pin, byte motor_enable_pin, byte infrared_pin);
  
  void get_params(byte *motor_dir_pin, byte *motor_step_pin, byte *motor_enable_pin, byte *infrared_pin);
  void start(void);
  void run_motor(void);
  int get_distance(void);
};

#endif
