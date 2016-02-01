#ifndef INFRARED_H
#define INFRARED_H

#include "Arduino.h"

class t_infrared_sensor 
{
	private:
				  int low;

				  byte pin;
	public: 
				t_infrared_sensor();
  void set_params(byte _pin, int _low);
  void get_params(byte *_pin, int *_low);
 
  int get_distance(void); 
  byte is_within_limits(void);
};

#endif //INFRARED_H
