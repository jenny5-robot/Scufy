#ifndef INFRARED_H
#define INFRARED_H

#define NUM_INDEXES 20

#include "Arduino.h"

class infrared 
{
	private:
				  int min_valid;
				  int max_valid;
				  byte analogPinNumber;
				  int latest_max_value;
				  int latest_min_value;
				  int latest_time_updated;
				  void CalcAverage();

	public: 
				int lastReadValues[NUM_INDEXES];
				int sensorValue;
				infrared(byte _analogPinNumber, int _minValid, int _maxValid);
				void UpdateSensor();
				int getMinValid();
				int getMaxValid();
};

#endif //INFRARED_H
