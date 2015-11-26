#include "infrared_sensors_controller.h"

t_infrared_sensors_controller::t_infrared_sensors_controller(byte _numberOfSensors, byte *_analogPinNumber, int *_minValid, int *_maxValid)
{
	numberOfSensors = _numberOfSensors;
	sensors = new infrared*[numberOfSensors];
  for (byte i = 0; i < numberOfSensors; i++)
    sensors[i] = new infrared(_analogPinNumber[i], _minValid[i], _maxValid[i]);  
}

byte t_infrared_sensors_controller::getDistanceFromSensor(byte _sensorIndex, int startValue, int endValue)
{
	if (sensors[_sensorIndex]->sensorValue >= startValue && sensors[_sensorIndex]->sensorValue <= endValue)
		return 1;
	else
		return 0;
}

int t_infrared_sensors_controller::getSensorValue(byte _sensorIndex) {
  return sensors[_sensorIndex]->sensorValue;
}

void t_infrared_sensors_controller::updateSensor(byte _sensorIndex)
{
	sensors[_sensorIndex]->UpdateSensor();
}
