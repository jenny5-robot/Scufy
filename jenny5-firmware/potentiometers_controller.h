#ifndef POTENTIOMETERS_CONTROLLER_H_
#define POTENTIOMETERS_CONTROLLER_H_

#include "Arduino.h"
#include "potentiometer_controller.h"


class t_potentiometers_controller
{
private:
	byte					num_sensors;
  t_potentiometer_controller *sensors;

public:
	t_potentiometers_controller();
 
	int get_position(byte potentiometer_index);
	void set_params(byte pot_index, byte pin, int low, int high, int _home);
	void get_params(byte pot_index, byte *pin, int *low, int *high, int *_home);
	int is_within_limits(byte potentiometer_index);

  void set_num_sensors(byte new_num_sensors);
  byte get_num_sensors(void);
};

#endif
