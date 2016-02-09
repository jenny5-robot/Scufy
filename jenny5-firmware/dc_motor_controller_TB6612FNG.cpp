#include "dc_motor_controller_TB6612FNG.h"
#include "jenny5_types.h"

//-------------------------------------------------------------------------------
t_dc_motor_controller_TB6612FNG::t_dc_motor_controller_TB6612FNG(void)
{
  pwm_pin = 3;
  dir_pin1 = 4;
  dir_pin1 = 5;
  enable_pin = 6;
  
  sensors_count = 0;
  sensors = NULL;

  motor_running = 0;

  motor_speed = 100;
}
//-------------------------------------------------------------------------------
t_dc_motor_controller_TB6612FNG::~t_dc_motor_controller_TB6612FNG(void)
{
  if (sensors)
    delete[] sensors;

  sensors_count = 0;

  motor_running = 0;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::create_init(byte _pwm_pin, byte _dir1, byte _dir2, byte _enable_pin, byte _motor_speed)
{
  pwm_pin = _pwm_pin;
  dir_pin1 = _dir1;
  dir_pin2 = _dir2;
  
  enable_pin = _enable_pin;
  
  motor_speed = _motor_speed;
  
  if (sensors)
    delete[] sensors;

  sensors_count = 0;

  motor_running = 0;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::move_motor(int num_steps)
{

  motor_running = 1;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::set_speed(byte __speed)
{
//  motor_speed = _motor_speed;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::disable(void)
{
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::add_sensor(byte sensor_type, byte sensor_index)
{
  sensors[sensors_count].type = sensor_type;
  sensors[sensors_count].index = sensor_index;
  sensors_count++;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::set_num_attached_sensors(byte num_sensors)
{
  if (sensors_count != num_sensors){
    // clear memory if the array has a different size
    if (sensors){
      delete[] sensors;
      sensors = NULL;
    }

    if (num_sensors > 0)
      sensors = new t_sensor_info[num_sensors]; // allocate memory for them
  }
  sensors_count = num_sensors; // actual number of sensors
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::set_running(byte is_running)
{
  motor_running = is_running;
}
//-------------------------------------------------------------------------------
byte t_dc_motor_controller_TB6612FNG::is_running(void)
{
  return motor_running;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::get_sensor(byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index)
{
  *sensor_type = sensors[sensor_index_in_motor_list].type;
  *sensor_index = sensors[sensor_index_in_motor_list].index;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::get_speed(byte *_motor_speed)
{
  *_motor_speed = motor_speed;
}
//-------------------------------------------------------------------------------
int t_dc_motor_controller_TB6612FNG::update_motor(t_potentiometers_controller *potentiometers_control)
{
// return distance_to_go or 0 if it has just been stopped
// return -1 if is still running or does nothing
  
    bool limit_reached = false;
    int time_to_go = 0; // must compute time to go
    int potentiometer_direction = 1;

    if (time_to_go)
    {
      for (byte j = 0 ; j < sensors_count ; ++j)
      {
        byte sensor_index = sensors[j].index;
        byte type = sensors[j].type;

        if (POTENTIOMETER == type)
        {
            potentiometer_direction = potentiometers_control->get_direction(sensor_index);
            if (potentiometers_control->is_lower_bound_reached(sensor_index)){
              if (time_to_go * potentiometer_direction < 0){
                limit_reached = true;
              }
            }
            if (potentiometers_control->is_upper_bound_reached(sensor_index)){
              if (time_to_go * potentiometer_direction > 0){
                limit_reached = true;
              }
            }
        }
        else if (ULTRASOUND == type)
        {
          // deal with ultrasound sensor
        }
      }

      if (!limit_reached)
      {
//        stepper->run();
        return -1;
      } else {
        int to_go = 0;//stepper->distanceToGo();
        //stepper->setCurrentPosition(0);
        //stepper->move(0);
        return to_go;
      }
    } else {
// the motor has just finished the move, so we output that event
      if (is_running()){
        set_running(0);
        return 0; // distance to go
      } else
        return -1;
    }
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::go_home(t_potentiometers_controller *potentiometers_control)
{
  byte sensor_index = 0;
  for (byte j = 0 ; j < sensors_count ; ++j) {
    byte type = sensors[j].type;
    if (POTENTIOMETER == type) {
      sensor_index = sensors[j].index;
      break;
    }
  }
  //calculate the remaining distance from the current position to home position, relative to the direction and position of the potentiometer
  int pot_dir = potentiometers_control->get_direction(sensor_index);
  int pot_home = potentiometers_control->get_home(sensor_index);
  int pot_pos = potentiometers_control->get_position(sensor_index);
  int distance_to_home = pot_dir*(pot_home - pot_pos);
  move_motor(distance_to_home);
}
//-------------------------------------------------------------------------------

