// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#include "as5147.h"
#include "Arduino.h"
//--------------------------------------------------------------------
t_as5147_controller::t_as5147_controller(void)
{
	pin = 24;
}
//--------------------------------------------------------------------
void t_as5147_controller::set_params(byte _pin)
{
	pin = _pin;
	pinMode(pin, OUTPUT);
	//Set Slave Select High to Start i.e disable chip
	digitalWrite(pin, HIGH);
	//Initialize SPI 
	SPI.begin();
}
//--------------------------------------------------------------------
int	t_as5147_controller::get_position(void)
{
	SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE1));

	//Send the Command Frame
	digitalWrite(pin, LOW);
	delayMicroseconds(1);
	SPI.transfer16(0xFFFF);
	digitalWrite(pin, HIGH);

	//unsigned long time_start = micros();
	//Read data frame
	digitalWrite(pin, LOW);
	delayMicroseconds(1);
	unsigned int angle = SPI.transfer16(0xC000);
	digitalWrite(pin, HIGH);
	SPI.endTransaction();

//	unsigned long time_end = micros();

	angle = (angle & (0x3FFF));

	int pos = ((unsigned long)angle) * 360UL / 16384UL;
	return pos;
}
//--------------------------------------------------------------------
void t_as5147_controller::get_params(byte *_pin)
{
	*_pin = pin;
}
//--------------------------------------------------------------------
/*
byte t_potentiometer_controller::is_within_limits(void)
{
int val = get_position();

return low <= val && val <= high;
}
//--------------------------------------------------------------------
bool t_potentiometer_controller::is_lower_bound_reached(void)
{
int val = get_position();
return low > val;
}
//--------------------------------------------------------------------
bool t_potentiometer_controller::is_upper_bound_reached(void)
{
int val = get_position();
return high < val;
}
//--------------------------------------------------------------------
int8_t t_potentiometer_controller::get_direction(void)
{
return _direction;
}
//--------------------------------------------------------------------
int t_potentiometer_controller::get_home_position(void)
{
return _home;
}
//--------------------------------------------------------------------
*/
