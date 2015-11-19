// Push-button must be connected as follows:
//                    __,__
//   Pin[index] ------o   o------ GND

#include "button_controller.h"

t_button_controller::t_button_controller(byte _num_buttons, byte* button_pin)
{
	button_pins = new byte[_num_buttons];
	
	for (int i = 0; i < _num_buttons; i++)
	{
		button_pins[i] = button_pin[i];
		pinMode(button_pin[i], INPUT);
		digitalWrite(button_pin[i], HIGH); // pull-up
	}
}

bool t_button_controller::button_status(byte button_index)
{
	bool button_pressed = !digitalRead(button_pins[button_index]); // pin low -> pressed
	return button_pressed;
}

