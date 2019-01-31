// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef INFRARED_DIGITAL_H
#define INFRARED_DIGITAL_H

#include "Arduino.h"

class t_infrared_digital_sensor
{
  private:
    byte pin;
  public:
    t_infrared_digital_sensor();
    void set_params(byte _pin);
    void get_params(byte *_pin);
    
    int get_value(void);
};

#endif //INFRARED_H
