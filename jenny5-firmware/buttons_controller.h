#ifndef buttons_controller_H
#define buttons_controller_H

#include <Arduino.h>

class t_buttons_controller
{
  private:
    byte num_sensors;

	  byte *buttons_pins;
    unsigned char *_direction; // -1 motors moves to negative; +1 motors move to positive
	
	public:

  t_buttons_controller();
  void set_params(int sensor_index, byte _pin, unsigned char _dir);
  void get_params(int sensor_index, byte *_pin, unsigned char *_dir);

  unsigned char get_direction(int sensor_index);

  void set_num_sensors(byte new_num_sensors);
  byte get_num_sensors(void);
  
	bool get_state(byte button_index);
};

#endif
