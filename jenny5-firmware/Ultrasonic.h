// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class t_ultrasonic
{
  private:
    byte Trig_pin;
    byte Echo_pin;

    int last_read_distance;

    unsigned long micros_start;
    bool first_high_received;
    bool trigger_started;

  public:
    t_ultrasonic (void);
    void create_init(byte trigger_pin, byte echo_pin);
    void get_sensor_pins(byte *trig_pins, byte *echo_pins);
    void trigger(void);
    bool check_echo(void);
    int get_last_read_distance(void);// cannot be read twice !
};

#endif //ULTRASONIC_H
