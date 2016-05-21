#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class t_ultrasonic
{
  public:
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
