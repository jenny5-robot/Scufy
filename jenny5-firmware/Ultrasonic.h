#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "NewPing.h"

class t_ultrasonic
{
  public:
    byte Trig_pin;
    byte Echo_pin;

    NewPing *sonar;

  public:
    t_ultrasonic (void);
    ~t_ultrasonic (void);
    void create_init(byte trigger_pin, byte echo_pin);
    void get_sensor_pins(byte *trig_pins, byte *echo_pins);
    //void trigger(void);
    //unsigned long get_distance(void);
};

#endif //ULTRASONIC_H
