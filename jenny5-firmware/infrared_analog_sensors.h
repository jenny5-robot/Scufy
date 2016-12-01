#ifndef INFRARED_ANALOG_SENSORS_CONTROLLER_H
#define INFRARED_ANALOG_SENSORS_CONTROLLER_H

#include "Arduino.h"
#include "infrared_analog.h"

class t_infrared_analog_sensors_controller
{
  public:
    t_infrared_analog_sensor *sensors;
    byte num_sensors;

  public:
    t_infrared_analog_sensors_controller(void);


    int get_signal_strength(byte sensor_index);

    void set_params(byte infrared_index, byte pin);
    void get_params(byte infrared_index, byte *pin);

    void set_num_sensors(byte new_num_sensors);
    byte get_num_sensors(void);
};

#endif //T_INFRARED_SENSORS_CONTROLLER_H
