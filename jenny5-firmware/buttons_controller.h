#ifndef buttons_controller_H
#define buttons_controller_H

// Push-button must be connected as follows:
//                    __,__
//   Pin[index] ------o   o------ GND


#include <Arduino.h>

class t_buttons_controller
{
  private:
	  //Declare pin functions on Arduino
	  byte *buttons_pins;
    byte num_sensors;
	
	public:

  t_buttons_controller();
  void set_params(int k, byte _pins);
  void get_params(int k, byte *_pin);

  void set_num_sensors(byte new_num_sensors);
  byte get_num_sensors(void);
  

	bool get_state(byte button_index);
};

#endif
