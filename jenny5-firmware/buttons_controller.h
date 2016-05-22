#ifndef buttons_controller_H
#define buttons_controller_H

#include <Arduino.h>

class t_buttons_controller
{
  private:
    byte num_sensors;

	  byte *buttons_pins;
    byte *_direction;
	
	public:

  t_buttons_controller();
  void set_params(int sensor_index, byte _pin, byte _dir);
  void get_params(int sensor_index, byte *_pin, byte *_dir);

  byte get_direction(int sensor_index);

  void set_num_sensors(byte new_num_sensors);
  byte get_num_sensors(void);
  
	bool get_state(byte button_index);
};

#endif
