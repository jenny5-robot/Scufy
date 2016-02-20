#include "dc_motors_controller_TB6612FNG.h"
#include "jenny5_types.h"
//-------------------------------------------------------------------------------
t_dc_motors_controller_TB6612FNG::t_dc_motors_controller_TB6612FNG(void)
{
  default_speed = 100;

  num_motors = 0;
  motors = NULL;
}
//-------------------------------------------------------------------------------
t_dc_motors_controller_TB6612FNG::~t_dc_motors_controller_TB6612FNG()
{
  if (motors){
    delete[] motors;
    motors = NULL;
  }
}
//-------------------------------------------------------------------------------
byte t_dc_motors_controller_TB6612FNG::get_num_motors(void)
{
  return num_motors;
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::set_num_motors(int new_num_motors)
{
  if (new_num_motors != num_motors){
    num_motors = new_num_motors;
    if (motors)
      delete[] motors;

    if (num_motors > 0){
    
      motors = new t_dc_motor_controller_TB6612FNG[num_motors];
    }
    else
      motors = NULL;
  }
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::set_motor_pins(byte motor_index, byte pwm_pin, byte _dir1, byte _dir2, byte _enable)
{  
  if (!motors[motor_index].is_running()){
    motors[motor_index].create_init(pwm_pin, _dir1, _dir2, _enable, default_speed);
  }
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::move_motor(byte motor_index, long num_millis)
{
  motors[motor_index].move_motor(num_millis); //move num_millis
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::set_speed(byte motor_index, byte _motor_speed)
{
  motors[motor_index].set_speed(_motor_speed); //set speed
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::disable(byte motor_index)
{
  motors[motor_index].disable();
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::add_sensor(byte motor_index, byte sensor_type, byte sensor_index)
{
	motors[motor_index].add_sensor(sensor_type, sensor_index);
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::set_num_attached_sensors(byte motor_index, byte num_sensors)
{
  motors[motor_index].set_num_attached_sensors(num_sensors);
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::set_running(byte motor_index, byte is_running)
{
  motors[motor_index].set_running(is_running);
}
//-------------------------------------------------------------------------------
byte t_dc_motors_controller_TB6612FNG::is_running(byte motor_index)
{
  return motors[motor_index].is_running();
}
//-------------------------------------------------------------------------------
byte t_dc_motors_controller_TB6612FNG::is_running(void)
{
  for (int m = 0; m < num_motors; m++)
    if (motors[m].is_running())
      return 1;
      
  return 0;
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::get_sensor(byte motor_index, byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index)
{
  motors[motor_index].get_sensor(sensor_index_in_motor_list, sensor_type, sensor_index);
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::get_speed(byte motor_index, byte *_motor_speed)
{
  motors[motor_index].get_speed(_motor_speed);
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::update_motors(t_buttons_controller *buttons_controller, char* serial_out)
{
  serial_out[0] = 0;
  // run motors
  
  //bool is_one_motor_running = false;
  for (int m = 0; m < num_motors; m++)
  {
    long time_to_go;
    int result = motors[m].update_motor(buttons_controller, time_to_go);
    if (result == MOTOR_JUST_STOPPED){
      char tmp_str[20];
      sprintf(tmp_str, "MD%d %d#", m, time_to_go);
      strcat(serial_out, tmp_str);
    }
  }
}
//-------------------------------------------------------------------------------
void t_dc_motors_controller_TB6612FNG::go_home(byte motor_index, t_buttons_controller *buttons_controller)
{
  motors[motor_index].go_home(buttons_controller);
}
//-------------------------------------------------------------------------------
