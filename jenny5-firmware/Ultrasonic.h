/*
  Ultrasonic.h - Library for HR-SC04 Ultrasonic Ranging Module.
  Created by ITead studio. Alex, Apr 20, 2010.
  iteadstudio.com
*/


#include "Arduino.h"

class Ultrasonic
{
  public:
    Ultrasonic(int TP, int EP);
    long Timing();
    long Ranging();

  private:
    int Trig_pin;
    int Echo_pin;
    long  duration,distacne_cm;
    
};
