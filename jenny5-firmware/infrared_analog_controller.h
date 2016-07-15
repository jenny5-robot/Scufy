#ifndef INFRARED_ANALOG_H
#define INFRARED_ANALOG_H

#include "Arduino.h"

class t_infrared_analog_sensor
{
  private:
    byte pin;

    int min_pos;
    int max_pos;
    int home_pos;

    unsigned char _direction;// 1 - same direction with motor, -1 - different direction

  public:
    t_infrared_analog_sensor();
    void set_params(byte _pin, int _min_pos, int _max_pos, int _home_pos, unsigned char _direction);
    void get_params(byte *_pin, int *_min_pos, int *_max_pos, int *_home_pos, unsigned char *_direction);
    int get_home_position(void);
    unsigned char get_direction(void);

    int get_signal_strength(void);
    byte is_within_limits(void);
    bool is_lower_bound_reached(void);
    bool is_upper_bound_reached(void);
};

#endif //INFRARED_H
