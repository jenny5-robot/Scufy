// Push-button must be connected as follows:
//                    __,__
//   Pin[index] ------o   o------ GND

#include "buttons_controller.h"
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
      
      for (byte m = 0; m < num_sensors; m++)
        buttons_pins[m] = 2;
    }
    else
      buttons_pins = NULL;
  } 
}
//--------------------------------------------------------------------
byte t_buttons_controller::get_num_sensors(void)
{
  return num_sensors;
}
//--------------------------------------------------------------------
void t_buttons_controller::set_params(byte *_pins)
{
  for (int i = 0; i < num_sensors; i++)
  {
    buttons_pins[i] = _pins[i];
    pinMode(buttons_pins[i], INPUT);
    digitalWrite(buttons_pins[i], HIGH); // pull-up
  }
}
//--------------------------------------------------------------------
void t_buttons_controller::get_params(byte *_pin)
{
    
}
//--------------------------------------------------------------------

