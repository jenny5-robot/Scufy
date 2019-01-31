// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#include "potentiometers.h"
#include "Arduino.h"
//--------------------------------------------------------------------
t_potentiometers_controller::t_potentiometers_controller(void)
{
  num_sensors = 0;
  sensors = NULL;
}
//--------------------------------------------------------------------
int	t_potentiometers_controller::get_position(byte potentiometer_index)
{
	return sensors[potentiometer_index].get_position();
}
//--------------------------------------------------------------------
/*
int8_t t_potentiometers_controller::get_direction(byte potentiometer_index)
{
  return sensors[potentiometer_index].get_direction();
}
//--------------------------------------------------------------------
int t_potentiometers_controller::get_home_position(byte potentiometer_index)
{
  return sensors[potentiometer_index].get_home_position();
}
*/
//--------------------------------------------------------------------
void t_potentiometers_controller::set_params(byte potentiometer_index, byte pin)
{
  sensors[potentiometer_index].set_params(pin);
}
//--------------------------------------------------------------------
void t_potentiometers_controller::get_params(byte potentiometer_index, byte *pin)
{
  sensors[potentiometer_index].get_params(pin);
}
//--------------------------------------------------------------------
/*
byte t_potentiometers_controller::is_within_limits(byte potentiometer_index)
{
	return sensors[potentiometer_index].is_within_limits();
}
*/
//--------------------------------------------------------------------
void t_potentiometers_controller::set_num_sensors(byte new_num_sensors)
{
  if (new_num_sensors != num_sensors){
    num_sensors = new_num_sensors;
    if (sensors)
      delete[] sensors;

    if (num_sensors > 0){    
      sensors = new t_potentiometer_controller[num_sensors];
      
      for (byte m = 0; m < num_sensors; m++)
        sensors[m].set_params(2);
    }
    else
      sensors = NULL;
  } 
}
//--------------------------------------------------------------------
byte t_potentiometers_controller::get_num_sensors(void)
{
  return num_sensors;
}
//--------------------------------------------------------------------
/*
byte t_potentiometers_controller::is_lower_bound_reached(byte potentiometer_index)
{
  return sensors[potentiometer_index].is_lower_bound_reached();
}
//--------------------------------------------------------------------
byte t_potentiometers_controller::is_upper_bound_reached(byte potentiometer_index)
{
  return sensors[potentiometer_index].is_upper_bound_reached();  
}
//--------------------------------------------------------------------
*/