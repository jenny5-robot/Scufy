#ifndef INFRARED_ANALOG_H
#define INFRARED_ANALOG_H

#include "Arduino.h"

class t_infrared_analog_sensor
{
  private:
    byte pin;
  public:
    t_infrared_analog_sensor();
    void set_params(byte _pin);
    void get_params(byte *_pin);

    int get_signal_strength(void);
};

#endif //INFRARED_H
