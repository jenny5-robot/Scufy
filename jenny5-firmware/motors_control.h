#include "Arduino.h"
#include "AccelStepper.h"
#include "potentiometers_controller.h"

//#define MAX_SENSORS_PER_MOTOR 6


//---------------------------------------------------
struct t_sensor_info {
	byte 	type;
	byte			index;
};
//---------------------------------------------------
class t_sensor_array{
  public:
  t_sensor_info *sensors_array;
  byte count;

  t_sensor_array(void)
  {
    sensors_array = NULL;
    count = 0;
  }
  ~t_sensor_array(void)
  {
    if (sensors_array){
      delete[] sensors_array;
      sensors_array = NULL;
    }
    count = 0;
  }
};
//---------------------------------------------------
class t_motors_control
{
public:
	//Declare pin functions on Arduino
  
	byte *dir_pins;
	byte *step_pins;
	byte *enable_pins;

	int default_motor_speed; //maximum steps per second 
	int default_motor_acceleration; //steps/second/second to accelerate

  byte 			num_motors;
  
  t_sensor_array	*sensors;
	AccelStepper 	**steppers;
  byte *motors_running;

  int *motor_speed;
  int *motor_acceleration;
	
public:
	t_motors_control(byte _num_motors);
	
	void move_motor(byte motor_index, int num_steps);
	
	void set_motor_speed(byte motor_index, int motor_speed);
	
	void set_motor_acceleration(byte motor_index, int motor_acceleration);

  void set_motor_speed_and_acceleration(byte motor_index, int motor_speed, int motor_acceleration);
  void get_motor_speed_and_acceleration(byte motor_index, int *motor_speed, int *motor_acceleration);
	
	void disable_motor(byte motor_index);
	
	void lock_motor(byte motor_index);

  void set_num_attached_sensors(byte motor_index, byte num_sensors);
  void get_num_attached_sensors(byte motor_index, byte *num_sensors);

	void add_sensor(byte motor_index, byte sensor_type, byte sensor_index);

  void get_sensor(byte motor_index, byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index);

  void set_motor_running(byte motor_index, byte is_running);
  void go_home(byte motor_index);
  byte is_motor_running(byte motor_index);

	void reset_pins();
  bool run(void);
  void run_motors(t_potentiometers_controller &potentiometers_control, char* serial_out);
};
//---------------------------------------------------
