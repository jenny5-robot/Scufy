#include "infrared_sensors_controller.h"


t_infrared_sensors_controller::t_infrared_sensors_controller(void)
{
  num_sensors = 0;
  sensors = NULL;
}
//--------------------------------------------------------------------
int  t_infrared_sensors_controller::get_signal_strength(byte infrared_index)
{
  return sensors[infrared_index].get_signal_strength();
}
//--------------------------------------------------------------------
int  t_infrared_sensors_controller::get_home(byte infrared_index)
{
  return sensors[infrared_index].get_home();
}
//--------------------------------------------------------------------
void t_infrared_sensors_controller::set_params(byte infrared_index, byte pin, int min_pos, int max_pos, int home_pos)
{
  sensors[infrared_index].set_params(pin, min_pos, max_pos, home_pos);
}
//--------------------------------------------------------------------
void t_infrared_sensors_controller::get_params(byte infrared_index, byte *pin, int *min_pos, int *max_pos, int *home_pos)
{
  sensors[infrared_index].get_params(pin, min_pos, max_pos, home_pos);
}
//--------------------------------------------------------------------
byte t_infrared_sensors_controller::is_within_limits(byte infrared_index)
{
  return sensors[infrared_index].is_within_limits();
}
//--------------------------------------------------------------------
void t_infrared_sensors_controller::set_num_sensors(byte new_num_sensors)
{
  if (new_num_sensors != num_sensors){
    num_sensors = new_num_sensors;
    if (sensors)
      delete[] sensors;

    if (num_sensors > 0){    
      sensors = new t_infrared_sensor[num_sensors];
      
      for (byte m = 0; m < num_sensors; m++)
        sensors[m].set_params(2, 100, 900, 500);
    }
    else
      sensors = NULL;
  } 
}
//--------------------------------------------------------------------
byte t_infrared_sensors_controller::get_num_sensors(void)
{
  return num_sensors;
}
//--------------------------------------------------------------------
