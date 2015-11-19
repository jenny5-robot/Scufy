// Push-button must be connected as follows:
//                    __,__
//   Pin[index] ------o   o------ GND

#include <Arduino.h>

class t_button_controller
{
public:
	//Declare pin functions on Arduino
	byte *button_pins;
	
	public:
	t_button_controller(byte _num_buttons,  byte* button_pin);
	bool button_status(byte button_index);
};
