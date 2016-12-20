#include "buttons.h"
//-----------------------------------------------------------------------
t_buttons_controller::t_buttons_controller(void)
{
	num_sensors = 0;
	buttons_pins = NULL;
}
//-----------------------------------------------------------------------
bool t_buttons_controller::get_state(byte button_index)
{
	bool button_pressed = !digitalRead(buttons_pins[button_index]); // pin low -> pressed
	return button_pressed;
}
//-----------------------------------------------------------------------
void t_buttons_controller::set_num_sensors(byte new_num_sensors)
{
  if (new_num_sensors != num_sensors){
    num_sensors = new_num_sensors;
    if (buttons_pins)
      delete[] buttons_pins;

    if (num_sensors > 0){    
      buttons_pins = new byte[num_sensors];
    }
    else{
      buttons_pins = NULL;
    }
  } 
}
//--------------------------------------------------------------------
byte t_buttons_controller::get_num_sensors(void)
{
  return num_sensors;
}
//--------------------------------------------------------------------
void t_buttons_controller::set_params(int button_index, byte _pin)
{
    buttons_pins[button_index] = _pin;
    pinMode(_pin, INPUT);
    digitalWrite(_pin, HIGH); // pull-up
}
//--------------------------------------------------------------------
void t_buttons_controller::get_params(int button_index, byte *_pin)
{
    *_pin = buttons_pins[button_index];
}
//--------------------------------------------------------------------

