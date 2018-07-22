#include "Arduino.h"
#include "as5147s.h"

//--------------------------------------------------------------------
t_as5147_s_controller::t_as5147_s_controller(void)
{
	num_sensors = 0;
	sensors = NULL;
}
//--------------------------------------------------------------------
int	t_as5147_s_controller::get_position(byte sensor_index)
{
	return sensors[sensor_index].get_position();
}
//--------------------------------------------------------------------
/*
int8_t t_as5147_s_controller::get_direction(byte potentiometer_index)
{
	return sensors[sensor_index].get_direction();
}
//--------------------------------------------------------------------
int t_as5147_s_controller::get_home_position(byte potentiometer_index)
{
	return sensors[sensor_index].get_home_position();
}
*/
//--------------------------------------------------------------------
void t_as5147_s_controller::set_params(byte sensor_index, byte pin)
{
	sensors[sensor_index].set_params(pin);
}
//--------------------------------------------------------------------
void t_as5147_s_controller::get_params(byte sensor_index, byte *pin)
{
	sensors[sensor_index].get_params(pin);
}
//--------------------------------------------------------------------
/*
byte t_as5147_s_controller::is_within_limits(byte sensor_index)
{
	return sensors[sensor_index].is_within_limits();
}
*/
//--------------------------------------------------------------------
void t_as5147_s_controller::set_num_sensors(byte new_num_sensors)
{
	if (new_num_sensors != num_sensors) {
		num_sensors = new_num_sensors;
		if (sensors)
			delete[] sensors;

		if (num_sensors > 0) {
			sensors = new t_as5147_controller[num_sensors];

			for (byte m = 0; m < num_sensors; m++)
				sensors[m].set_params(24);
		}
		else
			sensors = NULL;
	}
}
//--------------------------------------------------------------------
byte t_as5147_s_controller::get_num_sensors(void)
{
	return num_sensors;
}
//--------------------------------------------------------------------
/*
byte t_as5147_s_controller::is_lower_bound_reached(byte sensor_index)
{
	return sensors[sensor_index].is_lower_bound_reached();
}
//--------------------------------------------------------------------
byte t_as5147_s_controller::is_upper_bound_reached(byte sensor_index)
{
	return sensors[sensor_index].is_upper_bound_reached();
}
//--------------------------------------------------------------------
*/