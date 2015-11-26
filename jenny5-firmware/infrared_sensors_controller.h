#ifndef T_INFRARED_SENSORS_CONTROLLER_H
#define T_INFRARED_SENSORS_CONTROLLER_H

#include "Arduino.h"
#include "Infrared.h"

class t_infrared_sensors_controller
{
	public:
		infrared **sensors;
		byte numberOfSensors;
	
	public:
		t_infrared_sensors_controller(byte _numberOfSensors, byte *_analogPinNumber, int *_minValid, int *_maxValid);
		byte getDistanceFromSensor(byte _sensorIndex, int startValue, int endValue);
		void updateSensor(byte _sensorIndex);
    int getSensorValue(byte _sensorIndex);
};

#endif //T_INFRARED_SENSORS_CONTROLLER_H
