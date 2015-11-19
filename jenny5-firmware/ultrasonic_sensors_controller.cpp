#include "ultrasonic_sensors_controller.h"

t_ultrasonic_sensors_controller::t_ultrasonic_sensors_controller (byte _num_u_sensors, byte *trig_pins, byte *echo_pins)
{
	num_sensors = _num_u_sensors;
	sensors = new Ultrasonic*[num_sensors];

	for (byte i = 0; i < _num_u_sensors; i++){
		sensors [i] = new Ultrasonic(trig_pins [i], echo_pins [i]);
	}
}

long t_ultrasonic_sensors_controller::getDistanceForSensor (byte sensor_index)
{
	int NUMAR_RULARI = 10;
	long suma;

	for (int i = 0; i < NUMAR_RULARI; i++)
		suma += sensors [i]->Ranging();

	return suma / 10;
}
