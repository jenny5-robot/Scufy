#ifndef ULTRASONIC_SENSORS_CONTROLLER_H
#define ULTRASONIC_SENSORS_CONTROLLER_H

#include "arduino.h"
#include "Ultrasonic.h"

class UltrasonicSensorsController
{
public:
	Ultrasonic **sensors;

	byte num_sensors;

public:
	UltrasonicSensorsController (byte _num_u_sensors, byte *trig_pins, byte *echo_pins);

	long getDistanceForSensor (byte sensor_index);
};

#endif //ULTRASONIC_SENSORS_CONTROLLER_H