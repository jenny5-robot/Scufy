#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

class t_ultrasonic
{
  private:
    int Trig_pin;
    int Echo_pin;

  public:
    t_ultrasonic (int _TP, int _EP);
    unsigned long trigger(void);
    unsigned long get_distance(void);
};

#endif //ULTRASONIC_H
