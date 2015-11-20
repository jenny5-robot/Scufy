#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

class ultrasonic
{
  private:
    int Trig_pin;
    int Echo_pin;
    long  duration, distacne_cm;

  public:
    ultrasonic (int _TP, int _EP);
    long Timing();
    long Ranging();
};

#endif //ULTRASONIC_H