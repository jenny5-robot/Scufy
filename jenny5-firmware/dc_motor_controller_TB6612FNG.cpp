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
  time_to_go = 0;
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

  pinMode(enable_pin, OUTPUT);

  pinMode(pwm_pin, OUTPUT);
  pinMode(dir_pin1, OUTPUT);
  pinMode(dir_pin2, OUTPUT);
  
  motor_speed = _motor_speed;
  
  if (sensors)
    delete[] sensors;

  sensors_count = 0;

  motor_running = 0;
  time_to_go = 0;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::move_motor(long num_millis)
{  
  byte value_pin1, value_pin2;

  if (num_millis > 0){
    value_pin1 = HIGH;
    value_pin2 = LOW;
  }
  else{
    value_pin1 = LOW;
    value_pin2 = HIGH;
  }

  digitalWrite(dir_pin1, value_pin1);
  digitalWrite(dir_pin2, value_pin2);
  analogWrite(pwm_pin, motor_speed);
    
  time_to_go = num_millis;
  start_time = millis();
  digitalWrite(enable_pin, HIGH); //disable standby
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
long t_dc_motor_controller_TB6612FNG::update_motor(t_buttons_controller *buttons_controller, long & t_to_go)
{
// return distance_to_go or 0 if it has just been stopped
// return -1 if is still running or does nothing
  if (motor_running){
    bool limit_reached = false;
    unsigned long time_now = millis();
    if (time_now - start_time > abs(time_to_go)){
      digitalWrite(enable_pin, LOW);
      motor_running = 0;
      t_to_go = 0;
      time_to_go = 0;
      return MOTOR_JUST_STOPPED;
    }
    else{ // still we have some time to go
      for (byte j = 0; j < sensors_count; j++){
        byte sensor_index = sensors[j].index;
        byte type = sensors[j].type;

        if (BUTTON == type)
        {
          if (buttons_controller->get_state((sensor_index)))
            if (time_to_go > 0)
              limit_reached = true;
        }
      }

      if (!limit_reached){
        return MOTOR_STILL_RUNNING;
      }
      else {
        long to_go = time_now - start_time;
        motor_running = 0;
        if (time_to_go > 0)
          t_to_go = to_go;
        else
          t_to_go = -to_go;
        return MOTOR_JUST_STOPPED;
      }
    }
  }
  else
    return MOTOR_DOES_NOTHING;
}
//-------------------------------------------------------------------------------
void t_dc_motor_controller_TB6612FNG::go_home(t_buttons_controller *buttons_controller)
{
  byte sensor_index = 0;
  for (byte j = 0 ; j < sensors_count ; ++j) {
    byte type = sensors[j].type;
    if (POTENTIOMETER == type) {
      sensor_index = sensors[j].index;
      break;
    }
  }
/*
  //calculate the remaining distance from the current position to home position, relative to the direction and position of the potentiometer
  int pot_dir = potentiometers_control->get_direction(sensor_index);
  int pot_home = potentiometers_control->get_home(sensor_index);
  int pot_pos = potentiometers_control->get_position(sensor_index);
  int distance_to_home = pot_dir*(pot_home - pot_pos);
  move_motor(distance_to_home);
  */
}
//-------------------------------------------------------------------------------

