#include <Wire.h>
#include <avr/pgmspace.h>

#include "ultrasonic_sensors.h"
#include "potentiometers.h"
#include "stepper_motors.h"
#include "buttons.h"
#include "jenny5_types.h"
#include "infrared_analog_sensors.h"
#include "tera_ranger_one.h"
#include "dc_motors_TB6612FNG.h"
#include "tera_ranger_one_lidar.h"

//#include "MemoryFree.h"

t_stepper_motors_controller stepper_motors_controller;
t_dc_motors_controller_TB6612FNG dc_motors_controller_TB6612FNG;
t_potentiometers_controller potentiometers_controller;
t_ultrasonic_sensors_controller ultrasonic_sensors_controller;
t_infrared_analog_sensors_controller infrared_analog_sensors_controller;
t_buttons_controller buttons_controller;
t_tera_ranger_one_controller tera_ranger_one_controller;
t_tera_ranger_one_lidar *tera_ranger_one_lidar;

char is_command_running;

char firmware_version[20];// year.month.day.build number

char current_buffer[65];

//#define DEBUG
//#define PRINT_HELP

//unsigned long time;

bool first_start;

//--------------------------------------------------------------------------------------------
void setup()
{
  first_start = 0;
  strcpy(firmware_version, "2017.01.11.0");

  current_buffer[0] = 0;

  tera_ranger_one_lidar = NULL;

  Serial.begin(115200); //Open Serial connection

/*
  Serial.write("Commands are:");
  Serial.println(F("T# // test connection. Returns T#."));
  Serial.println(F("MSx y# // Moves stepper motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement. Outputs MSx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go."));
  Serial.println(F("MDx y# // Moves DC motor x for y miliseconds. Outputs MDx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the time to go."));
  Serial.println(F("MVx y# // Moves servo motor x for y steps."));
  Serial.println(F("HSx# // Moves stepper motor x to home position. The first sensor in the list of sensors will establish the home position. The motor does nothing if no sensor is attached. Returns HSx#."));
  Serial.println(F("HDx# // Moves DC motor x to home position. The first sensor in the list of sensors must be the button which establish the home position. The motor does nothing if no sensor is attached. Returns HDx#."));
  Serial.println(F("HVx# // Moves servo motor x to home position.  Returns HVx#."));

  Serial.println(F("DSx#  // Disables stepper motor x. Outputs DSx# when done."));
  Serial.println(F("DDx#  // Disables dc motor x. Outputs DDx# when done."));

  Serial.println(F("Lx#  // Locks stepper motor x. Outputs Lx# when done."));

  Serial.println(F("SSx s a# // Sets speed of stepper motor x to s and the acceleration to a."));
  Serial.println(F("SDx s# // Sets speed of motor x to s."));
  Serial.println(F("SPx min max home# // Sets the parameters of a potentiometer. Min and max are the limits where it can move and home is from where we bring the robot when we start."));

  Serial.println(F("ASx n Py Bz ... # // Attach to stepper motor x a list of n sensors (like Potentiometer y, Button z etc)."));
  Serial.println(F("ADx n Py Bz ... # // Attach to dc motor x a list of n sensors (like Potentiometer y, Button z etc)."));
  Serial.println(F("AVx n Py Bz ... # // Attach to servo motor x a list of n sensors (like Potentiometer y, Button z etc)."));

  Serial.println(F("Ux# // Gets the distance as measured by the ultrasonic sensor x. Outputs Ux d# when the reading has been completed. Note that this works in \"background\" and can output after some time (and in the meantime the program can do something else."));
  Serial.println(F("Bx# // Gets the status of the button x. Outputs Bx s# where s is either 0 or 1."));
  Serial.println(F("Px# // Gets the position of the potentiometer x. Outputs Px p#"));
  Serial.println(F("Ix# // Gets the value of infrared sensor x. Outputs Ix v#"));
  Serial.println(F("TRx# // Gets the value of Tera Ranger One sensor. Outputs TRx v#"));

  Serial.println(F("GSx# // Gets the parameters for stepper motor x: speed acceleration num_sensors_attached. Outputs GSx s a 1#"));
  Serial.println(F("GDx# // Gets the parameters for dc motor x: speed num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GDx s a 1 0 0#"));
  Serial.println(F("GPx# // Gets the parameters for potentiometer x: min max home direction. Outputs GPx n u h d#"));
  Serial.println(F("GUx# // Gets the parameters for ultrasound x: trig_pin echo_pin. Outputs GUx t e#"));
  Serial.println(F("GBx# // Gets the parameters for button x: out_pin direction. Outputs GBx p d#"));

  Serial.println(F("CS n d1 s1 e1 d2 s2 e2# // Creates the stepper motors controller and set some of its parameters. n is the number of motors, d, s, e are dir, step and enable pins. Outputs CS# when done."));
  Serial.println(F("CD n p1 d11 d12 e1 p2 d21 d22 e2# // Creates the dc motors controller and set some of its parameters. n is the number of motors, p is the pwm_pin, d1 and d2 are the direction pins and e is the enable pins. Outputs CD# when done."));
  Serial.println(F("CV n# // Creates the servo motors controller and set some of its parameters. n is the number of motors, Outputs CV# when done."));

  Serial.println(F("CP n p1 l1 h1 _h1 _d1 p2 l2 h2 _h2 _d2# // Creates the potentiometers controller and set some of its parameters. n is the number of potentiometers, p is the output pin, l, h and _h are bottom, upper and home position, _d is the directon of the sensor relative to the direction in which the motor is moving. Outputs CP# when done."));
  Serial.println(F("CU n t1 e1 t2 e2# // Creates the ultrasonic controller and set some of its parameters. n is the number of sonars, t and e are trigger and echo pins. Outputs CU# when done."));
  Serial.println(F("CIA n pin1 min1 max1 home1 dir1 pin2 min2 max2 home2 dir2# // Creates the infrared controller and set some of its parameters. n is the number of infrared sensors, pin is the analog pin index and min, max and home are the lower, upper bounds and home position of this sensor. Outputs CIA# when done."));
  Serial.println(F("CID n pin1 pin2# // Creates the infrared digital controller and set some of its parameters. n is the number of infrared digital sensors, pin1 is the analog pin index. Outputs CID# when done."));
  Serial.println(F("CB n p1 p2# // Creates the buttons controller and set some of its parameters. n is the number of button sensors, p is the digital pin. Outputs CB# when done."));
  Serial.println(F("CTR# // Creates the Tera Ranger One controller. Outputs CTR# when done."));
  - CL creates LIDAR


  Serial.println(F("V# // Outputs version string. eg: 2016.06.12.0#"));


  Serial.println(F("Motor index is between 0 and num_motors - 1"));

  Serial.println();
*/
  Serial.write("T#");// initialization is over; must check for T# string (which is the alive test)
}
//--------------------------------------------------------------------------------------------
void parse_and_execute_commands(char* tmp_str, byte str_length, char *serial_out)
{
  byte i = 0;
  serial_out[0] = 0;
  char tmp_serial_out[100];
  tmp_serial_out[0] = 0;

  while (i < str_length) {
    // can be more than 1 command in a string, so I have to check again for a letter
    if ((tmp_str[i] >= 'A' && tmp_str[i] <= 'Z') || (tmp_str[i] >= 'a' && tmp_str[i] <= 'z')) {

		if (tmp_str[i] == 'S' || tmp_str[i] == 's') { // stepper motor
			if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') { // move motor
				int motor_index, num_steps;
       int num_consumed;
				int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_index, &num_steps, &num_consumed);
				if (num_read == 2) {
					stepper_motors_controller.move_motor(motor_index, num_steps);
					is_command_running = 1;
					i += 2 + num_consumed;
				}
				else
					i++;// error on incomplete string (does nothing)
			}
			else
			if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // disable motor
				int motor_index;
					sscanf(tmp_str + i + 2, "%d", &motor_index);
					stepper_motors_controller.disable(motor_index);
					sprintf(tmp_serial_out, "SD%d#", motor_index);
					strcat(serial_out, tmp_serial_out);
					i += 4;
			}
			// locks motor
			else
			if (tmp_str[i + 1] == 'L' || tmp_str[i + 1] == 'l') {
				int motor_index;
				sscanf(tmp_str + i + 2, "%d", &motor_index);
				stepper_motors_controller.lock(motor_index);
				sprintf(tmp_serial_out, "SL%d#", motor_index);
				strcat(serial_out, tmp_serial_out);
				i += 4;
			}
     else
        if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') { // stepper motor speed and acceleration
          int motor_index;
          int motor_speed, motor_acceleration;
          int num_consumed;
          sscanf(tmp_str + i + 2, "%d%d%d%n", &motor_index, &motor_speed, &motor_acceleration, &num_consumed);
          stepper_motors_controller.set_speed_and_acceleration(motor_index, motor_speed, motor_acceleration);
		  sprintf(tmp_serial_out, "SS%d#", motor_index);
		  strcat(serial_out, tmp_serial_out);
          i += 2 + num_consumed;
        }
		else
			if (tmp_str[i + 1] == 'H' || tmp_str[i + 1] == 'h') {// home
				int motor_index;
				int num_consumed;
				sscanf(tmp_str + i + 2, "%d%n", &motor_index, &num_consumed);
				stepper_motors_controller.go_home(motor_index, &potentiometers_controller, &buttons_controller);
				i += 4;
			}
			else
				if (tmp_str[i + 1] == 'G' || tmp_str[i + 1] == 'g') { // move motor to sensor position
					int motor_index, sensor_position;
					int num_consumed;
					int num_read = sscanf(tmp_str + i + 2, "%d%d%n", &motor_index, &sensor_position, &num_consumed);
					if (num_read == 2) {
						stepper_motors_controller.go_to_sensor_position(motor_index, sensor_position);
						is_command_running = 1;
						i += 2 + num_consumed;
					}
					else
						i++;// error on incomplete string (does nothing)
				}
			else
				i++;
			continue;
		}
		
        // dc motor
		if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // DC motor
			if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm') { // stepper motor
				int motor_index, num_miliseconds;
          int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_miliseconds);
          if (num_read == 2) {
            dc_motors_controller_TB6612FNG.move_motor(motor_index, num_miliseconds);
            is_command_running = 1;
            i += 5;
          }
          else
            i++;// error on incomplete string (does nothing)
        }
			// disables motor
			else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // disables DC motor
				int motor_index;
				sscanf(tmp_str + i + 2, "%d", &motor_index);
				dc_motors_controller_TB6612FNG.disable(motor_index);
				sprintf(tmp_serial_out, "DD%d#", motor_index);
				strcat(serial_out, tmp_serial_out);
				i += 4;
			}
		
        else
          i++; // incomplete string
        continue;
      }

      // potentiometer
      if (tmp_str[i] == 'P' || tmp_str[i] == 'p') {
        int sensor_index;
        sscanf(tmp_str + i + 1, "%d", &sensor_index);
        int sensor_value = potentiometers_controller.get_position(sensor_index);
        sprintf(tmp_serial_out, "P%d %d#", sensor_index, sensor_value);
        strcat(serial_out, tmp_serial_out);
        i += 2;
        continue;
      }

      // button
      if (tmp_str[i] == 'B' || tmp_str[i] == 'b') {
        int sensor_index;
        sscanf(tmp_str + i + 1, "%d", &sensor_index);
       // Serial.println(sensor_index);
        int sensor_value = buttons_controller.get_state(sensor_index);
        sprintf(tmp_serial_out, "B%d %d#", sensor_index, sensor_value);
        strcat(serial_out, tmp_serial_out);
        i += 2;
        continue;
      }
      // ultrasonic
      if (tmp_str[i] == 'U' || tmp_str[i] == 'u') {
        int sensor_index;
        sscanf(tmp_str + i + 1, "%d", &sensor_index);
        ultrasonic_sensors_controller.trigger(sensor_index);
        i += 2;
        continue;
      }

      // infrared
      if (tmp_str[i] == 'I' || tmp_str[i] == 'i') {
        int sensor_index;
        sscanf(tmp_str + i + 1, "%d", &sensor_index);
        int sensor_value = infrared_analog_sensors_controller.get_signal_strength(sensor_index);
        sprintf(tmp_serial_out, "I%d %d#", sensor_index, sensor_value);
        strcat(serial_out, tmp_serial_out);
        i += 2;
        continue;
      }

	  // LIDAR
	  if (tmp_str[i] == 'L' || tmp_str[i] == 'l') {// lidar operations
		  if (tmp_str[i + 1] == 'G' || tmp_str[i + 1] == 'g') {// lidar go
			  tera_ranger_one_lidar->start();
			  i += 3;
		  }
		  else
			  if (tmp_str[i + 1] == 'H' || tmp_str[i + 1] == 'h') {// lidar halt
				  tera_ranger_one_lidar->stop();
				  i += 3;
			  }
			  else
				  if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') {// lidar halt
					  int motor_speed, motor_acceleration;
					  int num_consumed;
					  sscanf(tmp_str + i + 2, "%d%d%n", &motor_speed, &motor_acceleration, &num_consumed);
					  tera_ranger_one_lidar->set_motor_speed_and_acceleration(motor_speed, motor_acceleration);
					  i += 2 + num_consumed;
				  }
			  else// unknown command
				  i += 2;
		  continue;
	  }

	  /*
      // go home
        else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // go DC home
          sscanf(tmp_str + i + 2, "%d", &motor_index);
          dc_motors_controller_TB6612FNG.go_home(motor_index, &buttons_controller);
        }
        is_command_running = 1;
        i += 4;
        continue;
      }
	  */
      // sets something
/*
      if (tmp_str[i] == 'S' || tmp_str[i] == 's') {
        else if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') { // potentiometer min max home
          int pot_index, pot_min, pot_max, pot_home, pot_pin, pot_dir;
          sscanf(tmp_str + i + 2, "%d%d%d%d%d%d", &pot_index, &pot_pin, &pot_min, &pot_max, &pot_home, &pot_dir);
          potentiometers_controller.set_params(pot_index, pot_pin, pot_min, pot_max, pot_home, pot_dir);
          i += 7;
        }
        else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // dc motor speed
          int motor_index;
          int motor_speed;
          sscanf(tmp_str + i + 2, "%d%d", &motor_index, &motor_speed);
          dc_motors_controller_TB6612FNG.set_speed(motor_index, motor_speed);
          i += 5;
        }
        continue;
      }
*/
      if (tmp_str[i] == 'A' || tmp_str[i] == 'a') { // attach sensors to motors
        int motor_index, num_sensors;
        char motor_type = tmp_str[i + 1];
        sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_sensors);
       // Serial.println(num_sensors);
        if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
          stepper_motors_controller.set_num_attached_sensors(motor_index, num_sensors);
        else if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
          dc_motors_controller_TB6612FNG.set_num_attached_sensors(motor_index, num_sensors);

        // now I have to add sensors one by one
        int j = i + 4;
        i += 5;
        int k = 0;
        while (k < num_sensors){
          if (tmp_str[j] == 'P' || tmp_str[j] == 'p') {
            int sensor_index;
			int _low, _high, _home;
			int _direction;
			int num_consumed;

            sscanf(tmp_str + j + 1, "%d%d%d%d%d%n", &sensor_index, &_low, &_high, &_home, &_direction, &num_consumed);
            if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
              stepper_motors_controller.add_sensor(motor_index, POTENTIOMETER, sensor_index, _low, _high, _home, _direction);
            else if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
              dc_motors_controller_TB6612FNG.add_sensor(motor_index, POTENTIOMETER, sensor_index);
            j += 1 + num_consumed;
            k++;
          }
          else if (tmp_str[j] == 'B' || tmp_str[j] == 'b') {
            int sensor_index;
            int _direction;
            int num_consumed;
            sscanf(tmp_str + j + 1, "%d%d", &sensor_index, &_direction, &num_consumed);
            if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
              stepper_motors_controller.add_sensor(motor_index, BUTTON, sensor_index, 0, 0, 0, _direction);
            else if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
              dc_motors_controller_TB6612FNG.add_sensor(motor_index, BUTTON, sensor_index);
            j += 1 + num_consumed;
            k++;
          }
          else 
          if (tmp_str[j] == 'I' || tmp_str[j] == 'i') {
            int sensor_index;
            sscanf(tmp_str + j + 1, "%d", &sensor_index);
            if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
              stepper_motors_controller.add_sensor(motor_index, INFRARED_ANALOG, sensor_index, 0, 0, 0, 0);
            else if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
              dc_motors_controller_TB6612FNG.add_sensor(motor_index, INFRARED_ANALOG, sensor_index);
            j += 2;
            k++;
          }
          else
            j++;
        }
        i = j + 1;
       // Serial.println(i);
        continue;
      }

      if (tmp_str[i] == 'G' || tmp_str[i] == 'g') { // for debugging purpose
        if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') { // gets stepper motor speed and acceleration num sensors attached
          int motor_index;
          byte num_sensors_attached;
          float motor_speed, motor_acceleration;
          sscanf(tmp_str + i + 2, "%d", &motor_index);
          stepper_motors_controller.get_speed_and_acceleration(motor_index, &motor_speed, &motor_acceleration);
          num_sensors_attached = stepper_motors_controller.get_num_attached_sensors(motor_index);
          sprintf(tmp_serial_out, "GS%d %d %d %d#", motor_index, (int)motor_speed, (int)motor_acceleration, num_sensors_attached);
          strcat(serial_out, tmp_serial_out);
          i += 4;
        }
        else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // gets stepper motor speed and acceleration
          int motor_index;
          byte motor_speed;
          sscanf(tmp_str + i + 2, "%d", &motor_index);
          dc_motors_controller_TB6612FNG.get_speed(motor_index, &motor_speed);
          sprintf(tmp_serial_out, "GD%d %d#", motor_index, motor_speed);
          strcat(serial_out, tmp_serial_out);
          i += 4;
        }
        else if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') { // get potentiometer min max home dir
          int pot_index;
//          int pot_min, pot_max, pot_home;
//		  int8_t pot_dir;
          byte pot_pin;
          sscanf(tmp_str + i + 2, "%d", &pot_index);
          potentiometers_controller.get_params(pot_index, &pot_pin);
          sprintf(tmp_serial_out, "GP%d %d#", pot_index, (int)pot_pin);
          strcat(serial_out, tmp_serial_out);
          i += 5;
        }
        else if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') { // get ultrasound trig_pin echo_pin
          int ultrasound_index;
          byte trig_pin, echo_pin;
          sscanf(tmp_str + i + 2, "%d", &ultrasound_index);
          ultrasonic_sensors_controller.get_sensor_pins(ultrasound_index, &trig_pin, &echo_pin);
          sprintf(tmp_serial_out, "GU%d %d %d#", ultrasound_index, trig_pin, echo_pin);
          strcat(serial_out, tmp_serial_out);
          i += 4;
        }
        else if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') { // get button pin direction
          int button_index;
          byte _pin;
 
          sscanf(tmp_str + i + 2, "%d", &button_index);
          buttons_controller.get_params(button_index, &_pin);
          sprintf(tmp_serial_out, "GB%d %d#", button_index, _pin);
          strcat(serial_out, tmp_serial_out);
          i += 2;
        }
        else
          i++;// incomplete string
        continue;
      }
      // test connection or read Tera Ranger One
      if (tmp_str[i] == 'T' || tmp_str[i] == 't') {
        if (tmp_str[i + 1] == 'R' || tmp_str[i + 1] == 'r') {
          tera_ranger_one_controller.trigger();
          i += 3;
          continue;
        }
        else{ // test connection
          sprintf(tmp_serial_out, "T#");
          strcat(serial_out, tmp_serial_out);
          i += 2;
          continue;
        }
      }
      // version number
      if (tmp_str[i] == 'V' || tmp_str[i] == 'v') {
        sprintf(tmp_serial_out, "V%s#", firmware_version);
        strcat(serial_out, tmp_serial_out);
        i += 2;
        continue;
      }

      // create something
      if (tmp_str[i] == 'C' || tmp_str[i] == 'c') {
        if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's') { // create a list of stepper motors
          if (!stepper_motors_controller.is_running()) {
            int num_motors = 0;

            int num_consumed = 0;
            sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);

            int total_num_consumed = 3 + num_consumed;
            stepper_motors_controller.set_num_motors(num_motors);
            for (int k = 0; k < num_motors; k++) {
              int _step_pin, _dir_pin, _enable_pin;
              sscanf(tmp_str + i + total_num_consumed, "%d%d%d%n", &_dir_pin, &_step_pin, &_enable_pin, &num_consumed);
              stepper_motors_controller.set_pins(k, _dir_pin, _step_pin, _enable_pin);
			  total_num_consumed += num_consumed + 1;
            }
            stepper_motors_controller.disable_all();
            sprintf(tmp_serial_out, "CS#");
            strcat(serial_out, tmp_serial_out);
            i += total_num_consumed;
          }
        }
        else if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd') { // create a list of DC motors
          if (!dc_motors_controller_TB6612FNG.is_running()) {
            int num_motors = 0;

            int num_consumed = 0;
            sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);

            int total_num_consumed = 3 + num_consumed;
            dc_motors_controller_TB6612FNG.set_num_motors(num_motors);
            for (int k = 0; k < num_motors; k++) {
              int pwm_pin, dir_pin1, dir_pin2, enable_pin;
              sscanf(tmp_str + i + total_num_consumed, "%d%d%d%d%n", &pwm_pin, &dir_pin1, &dir_pin2, &enable_pin, &num_consumed);
              dc_motors_controller_TB6612FNG.set_motor_pins(k, pwm_pin, dir_pin1, dir_pin2, enable_pin);
			  total_num_consumed += num_consumed + 1;
            }

            sprintf(tmp_serial_out, "CD#");
            strcat(serial_out, tmp_serial_out);
            i += total_num_consumed;
          }
        }
        else if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u') { // create a list of ultrasonic sensors

          int num_sonars = 0;

          int num_consumed = 0;
          sscanf(tmp_str + i + 3, "%d%n", &num_sonars, &num_consumed);

          int total_num_consumed = 3 + num_consumed;
          ultrasonic_sensors_controller.set_num_sensors(num_sonars);
          for (int k = 0; k < num_sonars; k++) {
            int _trig_pin, _echo_pin;
            sscanf(tmp_str + i + total_num_consumed, "%d%d%n", &_trig_pin, &_echo_pin, &num_consumed);
            ultrasonic_sensors_controller.set_sensor_pins(k, _trig_pin, _echo_pin);
			total_num_consumed += num_consumed + 1;
          }

          sprintf(tmp_serial_out, "CU#");
          strcat(serial_out, tmp_serial_out);
          i += total_num_consumed;
        }
        else if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p') { // create a list of potentiometers

          int num_potentiometers = 0;

          int num_consumed = 0;
          sscanf(tmp_str + i + 3, "%d%n", &num_potentiometers, &num_consumed);

          int total_num_consumed = 3 + num_consumed;
          potentiometers_controller.set_num_sensors(num_potentiometers);
          for (int k = 0; k < num_potentiometers; k++) {
            int _out_pin;
            sscanf(tmp_str + i + total_num_consumed, "%d%n", &_out_pin, &num_consumed);
            potentiometers_controller.set_params(k, _out_pin);
			total_num_consumed += num_consumed + 1;
          }

          sprintf(tmp_serial_out, "CP#");
          strcat(serial_out, tmp_serial_out);
          i += total_num_consumed;
        }
        else if (tmp_str[i + 1] == 'I' || tmp_str[i + 1] == 'i') { // create a list of infrared sensors

          int num_infrareds = 0;

          int num_consumed = 0;
          sscanf(tmp_str + i + 3, "%d%n", &num_infrareds, &num_consumed);

          int total_num_consumed = 3 + num_consumed;
          infrared_analog_sensors_controller.set_num_sensors(num_infrareds);
          for (int k = 0; k < num_infrareds; k++) {
            int out_pin;
            int num_read = sscanf(tmp_str + i + total_num_consumed, "%d%n", &out_pin, &num_consumed);
            if (num_read == 5)
              infrared_analog_sensors_controller.set_params(k, out_pin);
            else {
              sprintf(tmp_serial_out, "E#");
              strcat(serial_out, tmp_serial_out);
            }
			total_num_consumed += num_consumed + 1;
          }
          sprintf(tmp_serial_out, "CI#");
          strcat(serial_out, tmp_serial_out);
          i += total_num_consumed;
        }
        else if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b') { // create a list of button sensors
          int num_buttons = 0;
          int num_consumed = 0;
          sscanf(tmp_str + i + 3, "%d%n", &num_buttons, &num_consumed);

          int total_num_consumed = 3 + num_consumed;
          buttons_controller.set_num_sensors(num_buttons);
          for (int k = 0; k < num_buttons; k++) {
            int _pin;
            sscanf(tmp_str + i + total_num_consumed, "%d%n", &_pin, &num_consumed);
            buttons_controller.set_params(k, _pin);
			total_num_consumed += num_consumed + 1;
          }
          sprintf(tmp_serial_out, "CB#");
          strcat(serial_out, tmp_serial_out);
          i += total_num_consumed;
        }
        else if (tmp_str[i + 1] == 'T' || tmp_str[i + 1] == 't') { // create Tera Ranger One controller
          tera_ranger_one_controller.create_init();
          sprintf(tmp_serial_out, "CT#");
		  strcat(serial_out, tmp_serial_out);

          i += 3;
        }
        else
            if (tmp_str[i + 1] == 'L' || tmp_str[i + 1] == 'l') { // creates a LIDAR
            
              int num_consumed = 0;
              int _dir_pin, _step_pin, _enable_pin, _infrared_pin;
              sscanf(tmp_str + i + 2, "%d%d%d%d%n", &_dir_pin, &_step_pin, &_enable_pin, &_infrared_pin, &num_consumed);
			        tera_ranger_one_lidar = new t_tera_ranger_one_lidar(_dir_pin, _step_pin, _enable_pin, _infrared_pin);              
         
              sprintf(tmp_serial_out, "CL#");
              strcat(serial_out, tmp_serial_out);
              i += num_consumed + 2;
            }
            else
              i++; // incomplete string
        continue;
      }
      else
        i++;// incomplete string
    }
    else
      i++; // not a letter, move to the next one

  }
}
//--------------------------------------------------------------------------------------------
//Main loop
void loop()
{
  char serial_out[100];

  // check to see if there are new results from ultrasonic sensor
  ultrasonic_sensors_controller.update_results(serial_out);
  if (serial_out[0])
    Serial.write(serial_out);

  tera_ranger_one_controller.update_results(serial_out);
  if (serial_out[0])
    Serial.write(serial_out);

  if (Serial.available() || current_buffer[0]) {
    int num_read = 0;
    char serial_buffer[65];

    //num_read = Serial.readBytes(serial_buffer, 64); //Read up to 64 bytes

    int incomingByte = Serial.read();
    if (incomingByte != -1) {
      serial_buffer[0] = incomingByte;
      num_read = 1;
    }
    serial_buffer[num_read] = 0;// terminate the string
    if (serial_buffer[0] || current_buffer[0]) {
      if (serial_buffer[0])
        strcat(current_buffer, serial_buffer);

#ifdef DEBUG
      Serial.write("initial buffer is=");
      Serial.write(current_buffer);
      Serial.println();
#endif

      // parse from the beginning until I find a M, D, L, S, A, P, B, U, G, T
      int current_buffer_length = strlen(current_buffer);
      for (int i = 0; i < current_buffer_length; i++)
        if ((current_buffer[i] >= 'A' && current_buffer[i] <= 'Z') || (current_buffer[i] >= 'a' && current_buffer[i] <= 'z')) { // a command
          // find the terminal character #
          int j = current_buffer_length - 1;
          for (; j > i && current_buffer[j] != '#'; j--);// parse (from the end) until I find the termination char
          if (j > i) {

#ifdef DEBUG
            char tmp_str[64];
            strncpy(tmp_str, current_buffer + i, j - i);
            tmp_str[j - i] = 0;
            Serial.write("current command is=");
            Serial.write(tmp_str);
            Serial.println();
#endif

            parse_and_execute_commands(current_buffer + i, j - i, serial_out);
            if (serial_out[0])
              Serial.write(serial_out);


            // remove the current executed command
            strcpy(current_buffer, current_buffer + j + 1);// not sure if this is good due to overlaps

#ifdef DEBUG
            Serial.write("buffer left=");
            Serial.write(current_buffer);
            Serial.write("\n----------------\n");
            //Serial.println(strlen(current_buffer)); // buffer length
#endif

            break; //for i
          }
          else { // the string is not completed ... so I must wait for more...
            break; // for i
          }
        }
    }
  }
  stepper_motors_controller.run_motors(&potentiometers_controller, &buttons_controller, serial_out);
  if (serial_out[0]){
    Serial.write(serial_out);
    serial_out[0] = 0;
  }

  dc_motors_controller_TB6612FNG.update_motors(&buttons_controller, serial_out);
  if (serial_out[0]){
    Serial.write(serial_out);
     serial_out[0] = 0;
  }

  if (tera_ranger_one_lidar) {
	  tera_ranger_one_lidar->run_motor(serial_out);
	  if (serial_out[0]) {
		  Serial.write(serial_out);
		  serial_out[0] = 0;
	  }
  }
}
//------------------------------------------------------------------------------
