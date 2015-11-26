#include "Infrared.h"

infrared::infrared(byte _analogPinNumber, int _minValid, int _maxValid) 
{
	min_valid = _minValid;
	max_valid = _maxValid;
	analogPinNumber = _analogPinNumber;
	for (int idx = 0; idx < NUM_INDEXES; idx++) 
		lastReadValues[idx] = 0;
}

void infrared::UpdateSensor()
{
  for(byte i = 0; i < NUM_INDEXES; i++) {
  	int latest_value = analogRead(analogPinNumber);
  	if (latest_value >= min_valid && latest_value <= max_valid) {
  		lastReadValues[i] = latest_value;
  	}
  }
  CalcAverage();
}

void infrared::CalcAverage() 
{
	sensorValue = 0;
	for (int idx = 0; idx < NUM_INDEXES; idx++) 
		sensorValue += lastReadValues[idx];
	if (sensorValue > 0) {
		sensorValue /= NUM_INDEXES;
	}
}

int infrared::getMinValid()
{
	return min_valid;
}

int infrared::getMaxValid()
{
	return max_valid;
}
