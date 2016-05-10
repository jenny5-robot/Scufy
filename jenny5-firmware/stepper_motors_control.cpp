#include "stepper_motors_control.h"
#include "jenny5_types.h"
//-------------------------------------------------------------------------------
t_stepper_motors_controller::t_stepper_motors_controller(void)
{
  default_motor_speed = 200;
  default_motor_acceleration = 100;

  num_motors = 0;
  motors = NULL;
}
//-------------------------------------------------------------------------------
t_stepper_motors_controller::~t_stepper_motors_controller()
{
  if (motors){
    delete[] motors;
    motors = NULL;
  }
}
//-------------------------------------------------------------------------------
byte t_stepper_motors_controller::get_num_motors(void)
{
  return num_motors;
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_num_motors(int new_num_motors)
{
  if (new_num_motors != num_motors){
    num_motors = new_num_motors;
    if (motors)
      delete[] motors;

    if (num_motors > 0){
      motors = new t_stepper_motor_controller[num_motors];
    }
    else
      motors = NULL;
  }
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_pins(byte motor_index, byte _dir, byte _step, byte _enable)
{  
  if (!motors[motor_index].is_motor_running()){
    motors[motor_index].create_init(_dir, _step, _enable, default_motor_speed, default_motor_acceleration);
  }
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::move_motor(byte motor_index, int num_steps)
{
  motors[motor_index].move_motor(num_steps); //move num_steps
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::move_motor_to(byte motor_index, int _position)
{
  motors[motor_index].move_motor_to(_position); //move to 
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_speed(byte motor_index, float _motor_speed)
{
  motors[motor_index].set_motor_speed(_motor_speed); //set speed
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_acceleration(byte motor_index, float _motor_acceleration)
{
  motors[motor_index].set_motor_acceleration(_motor_acceleration); // set acceleration
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_speed_and_acceleration(byte motor_index, float _motor_speed, float _motor_acceleration)
{
  motors[motor_index].set_motor_speed(_motor_speed); //set speed
  motors[motor_index].set_motor_acceleration(_motor_acceleration); // set acceleration
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::disable(byte motor_index)
{
  motors[motor_index].disable_motor();
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::lock(byte motor_index)
{
  motors[motor_index].lock_motor();
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::add_sensor(byte motor_index, byte sensor_type, byte sensor_index)
{
	motors[motor_index].add_sensor(sensor_type, sensor_index);
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_num_attached_sensors(byte motor_index, byte num_sensors)
{
  motors[motor_index].set_num_attached_sensors(num_sensors);
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::set_is_running(byte motor_index, byte is_running)
{
  motors[motor_index].set_motor_running(is_running);
}
//-------------------------------------------------------------------------------
byte t_stepper_motors_controller::is_running(byte motor_index)
{
  return motors[motor_index].is_motor_running();
}
//-------------------------------------------------------------------------------
byte t_stepper_motors_controller::is_running(void)
{
  for (int m = 0; m < num_motors; m++)
    if (motors[m].is_motor_running())
      return 1;
      
  return 0;
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::get_sensor(byte motor_index, byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index)
{
  motors[motor_index].get_sensor(sensor_index_in_motor_list, sensor_type, sensor_index);
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::get_speed_and_acceleration(byte motor_index, float *_motor_speed, float *_motor_acceleration)
{
  motors[motor_index].get_motor_speed_and_acceleration(_motor_speed, _motor_acceleration);
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::run_motors(t_potentiometers_controller *potentiometers_control, t_infrared_sensors_controller *infrared_control, char* serial_out)
{
  serial_out[0] = 0;
  // run motors
  
  //bool is_one_motor_running = false;
  for (int m = 0; m < num_motors; m++)
  {
    int dist_to_go;
    byte result = motors[m].run_motor(potentiometers_control, infrared_control, dist_to_go);
    if (result == MOTOR_JUST_STOPPED){
      char tmp_str[20];
      sprintf(tmp_str, "MS%d %d#", m, dist_to_go);
      strcat(serial_out, tmp_str);
    }
  }
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::go_home(byte motor_index, t_potentiometers_controller *potentiometers_control)
{
  motors[motor_index].go_home(potentiometers_control);
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::go_home(byte motor_index, t_infrared_sensors_controller *infrared_control)
{
  motors[motor_index].go_home(infrared_control);
}
//-------------------------------------------------------------------------------
void t_stepper_motors_controller::disable_all(void)
{
  for (int m = 0; m < num_motors; m++)
    motors[m].disable_motor();
}

