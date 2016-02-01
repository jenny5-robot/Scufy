#include "infrared_sensors_controller.h"


t_infrared_sensors_controller::t_infrared_sensors_controller(void)
{
  num_sensors = 0;
  sensors = NULL;
}
//--------------------------------------------------------------------
int  t_infrared_sensors_controller::get_distance(byte infrared_index)
{
  return sensors[infrared_index].get_distance();
}
//--------------------------------------------------------------------
void t_infrared_sensors_controller::set_params(byte infrared_index, byte pin, int low)
{
  sensors[infrared_index].set_params(pin, low);
}
//--------------------------------------------------------------------
void t_infrared_sensors_controller::get_params(byte infrared_index, byte *pin, int *low)
{
  sensors[infrared_index].get_params(pin, low);
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
        sensors[m].set_params(2, 100);
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
