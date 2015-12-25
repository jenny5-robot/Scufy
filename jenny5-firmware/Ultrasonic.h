#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

class t_ultrasonic
{
  private:
    int Trig_pin;
    int Echo_pin;
    long  duration, distance_cm;

  public:
    t_ultrasonic (int _TP, int _EP);
    long Timing();
    long Ranging();
};

#endif //ULTRASONIC_H
