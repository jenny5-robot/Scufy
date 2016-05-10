#ifndef T_INFRARED_SENSORS_CONTROLLER_H
#define T_INFRARED_SENSORS_CONTROLLER_H

#include "Arduino.h"
#include "infrared_controller.h"

class t_infrared_sensors_controller
{
	public:
		t_infrared_sensor *sensors;
		byte num_sensors;
	
	public:
		t_infrared_sensors_controller(void);
   
	  byte is_within_limits(byte sensor_index);
	
    int get_signal_strength(byte sensor_index);
    int get_home_position(byte sensor_index);
    int get_direction(byte sensor_index);

    void set_params(byte infrared_index, byte pin, int min_pos, int max_pos, int home_pos, int _direction);
    void get_params(byte infrared_index, byte *pin, int *min_pos, int *max_pos, int *home_pos, int *_direction);
  
    void set_num_sensors(byte new_num_sensors);
    byte get_num_sensors(void);

      byte is_lower_bound_reached(byte sensor_index);
  byte is_upper_bound_reached(byte sensor_index);
};

#endif //T_INFRARED_SENSORS_CONTROLLER_H
