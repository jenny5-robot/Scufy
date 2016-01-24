#include "motor_controller.h"
#include "jenny5_types.h"

//-------------------------------------------------------------------------------
t_motor_controller::t_motor_controller(void)
{
  dir_pin = 2;
  step_pin = 3;
  enable_pin = 4;
  
  sensors_count = 0;
  sensors = NULL;

  motor_running = 0;
  stepper = NULL;

  //motor_speed = 300;
 // motor_acceleration = 100;
}
//-------------------------------------------------------------------------------
t_motor_controller::~t_motor_controller(void)
{
  if (stepper)
    delete stepper;

  if (sensors)
    delete[] sensors;

  sensors_count = 0;

  motor_running = 0;
}
//-------------------------------------------------------------------------------
void t_motor_controller::create_init(byte _dir, byte _step, byte _enable, float default_motor_speed, float default_motor_acceleration)
{
  dir_pin = _dir;
  step_pin = _step;
  enable_pin = _enable;
  
  //motor_speed = default_motor_speed;
  //motor_acceleration = default_motor_acceleration;

  if (stepper)
    delete stepper;

  if (sensors)
    delete[] sensors;

  sensors_count = 0;

  motor_running = 0;
    
  stepper = new AccelStepper(AccelStepper::DRIVER, step_pin, dir_pin);
  stepper->setMaxSpeed(default_motor_speed);
  stepper->setSpeed(default_motor_speed);
  stepper->setAcceleration(default_motor_acceleration);

  reset_pins();
}
//-------------------------------------------------------------------------------
void t_motor_controller::move_motor(int num_steps)
{
  digitalWrite(enable_pin, LOW); // turn motor on
  stepper->move(num_steps); //move num_steps
  motor_running = 1;
}
//-------------------------------------------------------------------------------
void t_motor_controller::move_motor_to(int _position)
{
  digitalWrite(enable_pin, LOW); // turn motor on
  stepper->moveTo(_position); //move num_steps  
  motor_running = 1;
}
//-------------------------------------------------------------------------------
void t_motor_controller::set_motor_speed(float _motor_speed)
{
  stepper->setMaxSpeed(_motor_speed);
  stepper->setSpeed(_motor_speed);
//  motor_speed = _motor_speed;
}
//-------------------------------------------------------------------------------
void t_motor_controller::set_motor_acceleration(float _motor_acceleration)
{
  stepper->setAcceleration(_motor_acceleration);
 // motor_acceleration = _motor_acceleration;
}
//-------------------------------------------------------------------------------
void t_motor_controller::set_motor_speed_and_acceleration(float _motor_speed, float _motor_acceleration)
{
  stepper->setMaxSpeed(_motor_speed);
  stepper->setSpeed(_motor_speed);
  stepper->setAcceleration(_motor_acceleration);
 // motor_acceleration = _motor_acceleration;
  //motor_speed = _motor_speed;
}
//-------------------------------------------------------------------------------
void t_motor_controller::disable_motor(void)
{
  digitalWrite(enable_pin, HIGH); // disable motor
}
//-------------------------------------------------------------------------------
void t_motor_controller::lock_motor(void)
{
  digitalWrite(enable_pin, LOW); // enable motor
}
//-------------------------------------------------------------------------------
//Reset pins to default states
void t_motor_controller::reset_pins(void)
{
    digitalWrite(step_pin, LOW);
    digitalWrite(dir_pin, LOW);
    digitalWrite(enable_pin, HIGH); // all motors are disabled now
}
//-------------------------------------------------------------------------------
void t_motor_controller::add_sensor(byte sensor_type, byte sensor_index)
{
  sensors[sensors_count].type = sensor_type;
  sensors[sensors_count].index = sensor_index;
  sensors_count++;
}
//-------------------------------------------------------------------------------
void t_motor_controller::set_num_attached_sensors(byte num_sensors)
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
void t_motor_controller::set_motor_running(byte is_running)
{
  motor_running = is_running;
}
//-------------------------------------------------------------------------------
byte t_motor_controller::is_motor_running(void)
{
  return motor_running;
}
//-------------------------------------------------------------------------------
void t_motor_controller::get_sensor(byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index)
{
  *sensor_type = sensors[sensor_index_in_motor_list].type;
  *sensor_index = sensors[sensor_index_in_motor_list].index;
}
//-------------------------------------------------------------------------------
void t_motor_controller::get_motor_speed_and_acceleration(float *_motor_speed, float *_motor_acceleration)
{
  if (stepper){
   *_motor_acceleration = stepper->getAcceleration();
    *_motor_speed = stepper->maxSpeed();
}
    else{
    *_motor_speed = 0;
    *_motor_acceleration = 0;
    }
}
//-------------------------------------------------------------------------------
int t_motor_controller::run_motor(t_potentiometers_controller *potentiometers_control)
{
// return distance_to_go or 0 if it has just been stopped
// return -1 if is still running or does nothing
  
    bool limit_reached = false;

    if (stepper->distanceToGo())
    {
      for (byte j = 0 ; j < sensors_count ; ++j)
      {
        byte sensor_index = sensors[j].index;
        byte type = sensors[j].type;

        if (POTENTIOMETER == type)
        {
            if (0 == potentiometers_control->is_within_limits(sensor_index))
              limit_reached = true;
        }
        else if (ULTRASOUND == type)
        {
          // deal with ultrasound sensor
        }
      }

      if (!limit_reached)
      {
        stepper->run();
        return -1;
      }
      else{
        return stepper->distanceToGo();
        //stepper->setCurrentPosition(0);
        set_motor_running(0);
      }
    }
    else{
// the motor has just finished the move, so we output that event
      if (is_motor_running()){
        set_motor_running(0);
        return 0; // distance to go
      }
      else
        return -1;
    }
}
//-------------------------------------------------------------------------------
void t_motor_controller::go_home(void)
{
  // to do...
}
//-------------------------------------------------------------------------------

