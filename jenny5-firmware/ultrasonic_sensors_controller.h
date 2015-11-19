#ifndef T_ULTRASONIC_SENSORS_CONTROLLER_H
#define T_ULTRASONIC_SENSORS_CONTROLLER_H

#include "Arduino.h"
#include "Ultrasonic.h"

class t_ultrasonic_sensors_controller
{
public:
	Ultrasonic **sensors;

	byte num_sensors;

public:
	t_ultrasonic_sensors_controller (byte _num_u_sensors, byte *trig_pins, byte *echo_pins);

	long getDistanceForSensor (byte sensor_index);
};

#endif //T_ULTRASONIC_SENSORS_CONTROLLER_H