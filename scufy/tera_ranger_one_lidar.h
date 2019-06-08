// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef TERA_RANGER_ONE_LIDAR_H
#define TERA_RANGER_ONE_LIDAR_H

#include "tera_ranger_one.h"
#include "SpeedyStepper.h"
#include "infrared_digital.h"

class t_tera_ranger_one_lidar{

  private:

  byte enable_pin;
  bool request_for_distance_sent;

  SpeedyStepper stepper;
  t_infrared_digital_sensor infrared_sensor;
  t_tera_ranger_one_controller tera_ranger_one;

  bool started;
  bool reference_touched, prev_reference_touched, first_reference_touched;
  int motor_position;

public:
  t_tera_ranger_one_lidar(byte motor_dir_pin, byte motor_step_pin, byte motor_enable_pin, byte infrared_pin);
  
  void start(void);
  void stop(void);
  void run_motor(char *serial_out);
  void set_motor_speed_and_acceleration(float motor_speed, float motor_acceleration);
};

#endif
