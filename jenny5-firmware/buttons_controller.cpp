// Push-button must be connected as follows:
//                    __,__
//   Pin[index] ------o   o------ GND

#include "buttons_controller.h"
//-----------------------------------------------------------------------
t_buttons_controller::t_buttons_controller(void)
{
	num_sensors = 0;
	buttons_pins = NULL;
  _direction = NULL;
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
      
    if (_direction)
      delete[] _direction;

    if (num_sensors > 0){    
      buttons_pins = new byte[num_sensors];
      _direction = new byte[num_sensors];
      
      for (byte m = 0; m < num_sensors; m++){
        buttons_pins[m] = 2;
        _direction[m] = 1;
      }
    }
    else
      buttons_pins = NULL;
      _direction = NULL;
  } 
}
//--------------------------------------------------------------------
byte t_buttons_controller::get_num_sensors(void)
{
  return num_sensors;
}
//--------------------------------------------------------------------
void t_buttons_controller::set_params(int index, byte _pin, byte _dir)
{
    buttons_pins[index] = _pin;
    _direction[index] = _dir;
    pinMode(buttons_pins[index], INPUT);
    digitalWrite(buttons_pins[index], HIGH); // pull-up
}
//--------------------------------------------------------------------
void t_buttons_controller::get_params(int index, byte *_pin, byte *_dir)
{
    *_pin = buttons_pins[index];
    *_dir  = _direction[index];
}
//--------------------------------------------------------------------
byte t_buttons_controller::get_direction(int sensor_index)
{
  return _direction[sensor_index];
}
//--------------------------------------------------------------------

