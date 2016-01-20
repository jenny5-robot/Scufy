#include "ultrasonic_sensors_controller.h"

//---------------------------------------------------------------------
t_ultrasonic_sensors_controller::t_ultrasonic_sensors_controller (byte _num_u_sensors, byte *trig_pins, byte *echo_pins)
{
	num_sensors = _num_u_sensors;
	sensors = new t_ultrasonic*[num_sensors];

	for (byte i = 0; i < _num_u_sensors; i++){
		sensors [i] = new t_ultrasonic(trig_pins [i], echo_pins [i]);
	}
}
//---------------------------------------------------------------------
long t_ultrasonic_sensors_controller::get_distance (byte sensor_index)
{
  int distance = sensors[sensor_index]->get_distance();
  if (distance > 250)
    return 0;
  else
    return distance;
}
//---------------------------------------------------------------------
