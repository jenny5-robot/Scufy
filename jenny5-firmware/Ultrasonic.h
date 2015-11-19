#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

class Ultrasonic
{
  private:
    int Trig_pin;
    int Echo_pin;
    long  duration, distacne_cm;

  public:
    Ultrasonic (int _TP, int _EP);
    long Timing();
    long Ranging();
};

#endif //ULTRASONIC_H