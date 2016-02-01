#ifndef T_INFRARED_SENSORS_CONTROLLER_H
#define T_INFRARED_SENSORS_CONTROLLER_H

#include "Arduino.h"
#include "infrared.h"

class t_infrared_sensors_controller
{
	public:
		t_infrared_sensor *sensors;
		byte num_sensors;
	
	public:
		t_infrared_sensors_controller(void);
   
		byte is_within_limits(byte sensor_index);
		
    int get_distance(byte sensor_index);

  void set_params(byte infrared_index, byte pin, int low);
  void get_params(byte infrared_index, byte *pin, int *low);
  
  void set_num_sensors(byte new_num_sensors);
  byte get_num_sensors(void);
};

#endif //T_INFRARED_SENSORS_CONTROLLER_H
