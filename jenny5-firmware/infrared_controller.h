#ifndef INFRARED_H
#define INFRARED_H

#include "Arduino.h"

class t_infrared_sensor 
{
	private:
				  int min_pos;
				  int max_pos;
          int home_pos;

				  byte pin;
	public: 
				t_infrared_sensor();
  void set_params(byte _pin, int _min_pos, int _max_pos, int _home_pos);
  void get_params(byte *_pin, int *_min_pos, int *_max_pos, int *_home_pos);
  int get_home(void);
 
  int get_signal_strength(void); 
  byte is_within_limits(void);
};

#endif //INFRARED_H
