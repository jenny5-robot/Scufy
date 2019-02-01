// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#ifndef stepper_motors_controller_H
#define stepper_motors_controller_H

#include "Arduino.h"
#include "stepper_motor.h"
#ifdef USE_POTENTIOMETERS
#include "potentiometers.h"
#endif
#include "infrared_analog_sensors.h"
#include "buttons.h"


//---------------------------------------------------
class t_stepper_motors_controller
{
  public:
    //Declare pin functions on Arduino

    t_stepper_motor_controller *motors;
    byte num_motors;

    float default_motor_speed; //maximum steps per second
    float default_motor_acceleration; //steps/second/second to accelerate

  public:
    t_stepper_motors_controller(void);
    ~t_stepper_motors_controller();
	void delete_memory(void);


    void set_pins(byte motor_index, byte _dir, byte _step, byte _enable);

    byte get_num_motors(void);
    void set_num_motors(int value);

    void move_motor(byte motor_index, int num_steps);
    void move_motor_to(byte motor_index, int _position);

    void set_speed(byte motor_index, float motor_speed);

    void set_acceleration(byte motor_index, float motor_acceleration);

    void set_speed_and_acceleration(byte motor_index, float motor_speed, float motor_acceleration);
    void get_speed_and_acceleration(byte motor_index, float *motor_speed, float *motor_acceleration);

    void disable(byte motor_index);
    void disable_all(void);

    void lock(byte motor_index);

    void set_num_attached_sensors(byte motor_index, byte num_sensors);
    byte get_num_attached_sensors(byte motor_index);

    void add_sensor(byte motor_index, byte sensor_type, byte sensor_index, int _min_pos, int _max_pos, int _home_pos, int8_t _direction);

    void get_sensor(byte motor_index, byte sensor_index_in_motor_list, byte *sensor_type, byte *sensor_index, int *_min_pos, int *_max_pos, int *_home_pos, int8_t *_direction);

    void set_is_running(byte motor_index, byte is_running);
    void go_home(byte motor_index, t_as5147s_controller *as5147_controller, 
#ifdef USE_POTENTIOMETERS
		t_potentiometers_controller *potentiometers_control, 
#endif
		t_buttons_controller* buttons_controller);
    
    byte is_running(byte motor_index);
    byte is_running(void);
	void stop(byte motor_index);

	void go_to_sensor_position(byte motor_index, t_as5147s_controller *as5147_controller,
#ifdef USE_POTENTIOMETERS
		t_potentiometers_controller *potentiometers_control, 
#endif
		int sensor_stop_position
	);

    void run_motors(t_as5147s_controller *as5147s_controller, 
#ifdef USE_POTENTIOMETERS
		t_potentiometers_controller *potentiometers_control, 
#endif
		t_buttons_controller* buttons_controller, char* serial_out);
};
//---------------------------------------------------
#endif
