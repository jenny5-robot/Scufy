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

    byte is_within_limits(byte sensor_index);

    int get_signal_strength(byte sensor_index);
    int get_home_position(byte sensor_index);
    unsigned char get_direction(byte sensor_index);

    void set_params(byte infrared_index, byte pin, int min_pos, int max_pos, int home_pos, unsigned char _direction);
    void get_params(byte infrared_index, byte *pin, int *min_pos, int *max_pos, int *home_pos, unsigned char *_direction);

    void set_num_sensors(byte new_num_sensors);
    byte get_num_sensors(void);

    byte is_lower_bound_reached(byte sensor_index);
    byte is_upper_bound_reached(byte sensor_index);

    bool is_going_home(byte sensor_index);
    void set_going_home(byte sensor_index, bool value);
};

#endif //T_INFRARED_SENSORS_CONTROLLER_H
