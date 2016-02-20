#include <avr/pgmspace.h>

#include "ultrasonic_sensors_controller.h"
#include "potentiometers_controller.h"
#include "stepper_motors_control.h"
#include "buttons_controller.h"
#include "jenny5_types.h"
#include "infrared_sensors_controller.h"
#include "dc_motors_controller_TB6612FNG.h"

//#include "MemoryFree.h"

t_stepper_motors_controller stepper_motors_controller;
t_dc_motors_controller_TB6612FNG dc_motors_controller_TB6612FNG;
t_potentiometers_controller potentiometers_controller;
t_ultrasonic_sensors_controller ultrasonic_sensors_controller;
t_infrared_sensors_controller infrared_sensors_controller;
t_buttons_controller buttons_controller;

char is_command_running;

char firmware_version[20];// year.month.day.build number

char current_buffer[65];

//#define DEBUG

//unsigned long time;

bool first_start;

//--------------------------------------------------------------------------------------------
void setup() 
{
  first_start = 0;
  strcpy(firmware_version, "2016.02.20.4");
  
  current_buffer[0] = 0;

  Serial.begin(115200); //Open Serial connection

  #ifdef DEBUG
  Serial.write("Commands are:");
  Serial.println(F("T# // test connection. Returns T#."));
  Serial.println(F("MSx y# // Moves stepper motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement. Outputs MSx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go."));
  Serial.println(F("MDx y# // Moves dc motor x for y miliseconds. Outputs MDx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the time to go."));
  Serial.println(F("MVx y# // Moves servo motor x for y steps.");
  
  Serial.println(F("HSx# // Moves stepper motor x to home position. The first sensor in the list of sensors must be the potentiometer which establish the home position. The motor does nothing if no potentiometer is attached. Returns HSx#."));
  Serial.println(F("HDx# // Moves DC motor x to home position. The first sensor in the list of sensors must be the button which establish the home position. The motor does nothing if no potentiometer is attached. Returns HDx#."));
  Serial.println(F("HVx# // Moves servo motor x to home position.  Returns HVx#.");
  
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
  Serial.println(F("Bx# // Gets the status of the button x. Outputs Bx s#"));
  Serial.println(F("Px# // Gets the position of the potentiometer x. Outputs Px p#"));
  Serial.println(F("Ix# // Gets the value of infrared sensor x. Outputs Ix v#"));
  
  Serial.println(F("GSx# // Gets the parameters for stepper motor x: speed acceleration num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GSx s a 1 0 0#"));
  Serial.println(F("GDx# // Gets the parameters for dc motor x: speed num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GDx s a 1 0 0#"));
  Serial.println(F("GVx# // Gets the parameters for dc motor x: speed num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GVx s a 1 0 0#"));
  Serial.println(F("GPx# // Gets the parameters for potentiometer x: min max home. Outputs PPx l u h#"));
  Serial.println(F("GUx# // Gets the parameters for ultrasound x: trig_pin echo_pin. Outputs UPx t e#"));
  
  Serial.println(F("CS n d1 s1 e1 d2 s2 e2# // Creates the stepper motors controller and set some of its parameters. n is the number of motors, d, s, e are dir, step and enable pins. Outputs CS# when done."));
  Serial.println(F("CD n p1 d11 d12 e1 p2 d21 d22 e2# // Creates the dc motors controller and set some of its parameters. n is the number of motors, p is the pwm_pin, d1 and d2 are the direction pins and e is the enable pins. Outputs CD# when done."));
  Serial.println(F("CV n# // Creates the servo motors controller and set some of its parameters. n is the number of motors, Outputs CV# when done."));

  Serial.println(F("CP n p1 l1 h1 _h1 _d1 p2 l2 h2 _h2 _d2# // Creates the potentiometers controller and set some of its parameters. n is the number of potentiometers, p is the output pin, l, h and _h are bottom, upper and home position, _d is the directon of the sensor relative to the direction in which the motor is moving. Outputs CP# when done."));
  Serial.println(F("CU n t1 e1 t2 e2# // Creates the ultrasonic controller and set some of its parameters. n is the number of sonars, t and e are trigger and echo pins. Outputs CU# when done."));
  Serial.println(F("CI n p1 l1 p2 l2# // Creates the infrared controller and set some of its parameters. n is the number of infrared sensors, p is the analog pin and l is the low and e are trigger and echo pins. Outputs CI# when done."));
  Serial.println(F("CB n p1 p2# // Creates the buttons controller and set some of its parameters. n is the number of button sensors, p is the digital pin. Outputs CB# when done."));
  
  
  Serial.println(F("V# // Outputs version string. eg: 2016.01.17.0#"));

  
  Serial.println(F("Motor index is between 0 and num_motors - 1"));
  
  Serial.println();
#endif
  Serial.write("T#");// initialization is over; must check for T# string (which is the alive test)
}
//--------------------------------------------------------------------------------------------
void parse_and_execute_commands(char* tmp_str, byte str_length, char *serial_out)
{
  byte i = 0;
  serial_out[0] = 0;
  while (i < str_length){
    // can be more than 1 command in a string, so I have to check again for a letter
    if ((tmp_str[i] >= 'A' && tmp_str[i] <= 'Z') || (tmp_str[i] >= 'a' && tmp_str[i] <= 'z')){
      
      if (tmp_str[i] == 'M' || tmp_str[i] == 'm'){// moves motor
        if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's'){// stepper motor
          int motor_index, num_steps;
          int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_steps);
          if (num_read == 2){
            stepper_motors_controller.move_motor(motor_index, num_steps);
            is_command_running = 1;
            i += 5;
          }
          else
            i++;// error on incomplete string (does nothing)
        }
        else
          if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd'){// DC motor
            int motor_index, num_miliseconds;
            int num_read = sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_miliseconds);
            if (num_read == 2){
              dc_motors_controller_TB6612FNG.move_motor(motor_index, num_miliseconds);
              is_command_running = 1;
              i += 5;
            }
            else
              i++;// error on incomplete string (does nothing)
        }
        else
          i++; // incomplete string  
      }
      else
          if (tmp_str[i] == 'P' || tmp_str[i] == 'p'){// potentiometer            
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            int sensor_value = potentiometers_controller.get_position(sensor_index);
            sprintf(serial_out, "P%d %d#", sensor_index, sensor_value);
            i += 2;
          }
        else
          if (tmp_str[i] == 'U' || tmp_str[i] == 'u'){// ultrasonic
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            ultrasonic_sensors_controller.trigger(sensor_index);
            //sprintf(serial_out, "U%d %d#", sensor_index, sensor_value);
       
            i += 2;
          }
        else
          if (tmp_str[i] == 'I' || tmp_str[i] == 'i'){// infrared
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            int sensor_value = infrared_sensors_controller.get_distance(sensor_index);
            sprintf(serial_out, "I%d %d#", sensor_index, sensor_value);
            i += 2;
          }
        else
        if (tmp_str[i] == 'D' || tmp_str[i] == 'd'){// disables motor
          int motor_index;
          if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's'){// disables stepper motor
            sscanf(tmp_str + i + 2, "%d", &motor_index);
            stepper_motors_controller.disable(motor_index);
            sprintf(serial_out, "DS%d#", motor_index);
          }
          else
            if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd'){// disables DC motor
              sscanf(tmp_str + i + 2, "%d", &motor_index);
              dc_motors_controller_TB6612FNG.disable(motor_index);
              sprintf(serial_out, "DD%d#", motor_index);            
            }
          i += 4;
        }
        else
          if (tmp_str[i] == 'L' || tmp_str[i] == 'l'){// locks motor
            int motor_index;
            sscanf(tmp_str + i + 1, "%d", &motor_index);
            stepper_motors_controller.lock(motor_index);
            sprintf(serial_out, "L%d#", motor_index);
            i += 2;
          }
        else
          if (tmp_str[i] == 'H' || tmp_str[i] == 'h'){// go home
            int motor_index;
            if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's'){// go stepper home
              sscanf(tmp_str + i + 2, "%d", &motor_index);
              stepper_motors_controller.go_home(motor_index, &potentiometers_controller);
              sprintf(serial_out, "HS%d#", motor_index);
            }
            else
              if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd'){// go stepper home
                sscanf(tmp_str + i + 2, "%d", &motor_index);
                dc_motors_controller_TB6612FNG.go_home(motor_index, &buttons_controller);
                sprintf(serial_out, "HD%d#", motor_index);
              }
            is_command_running = 1;
            i += 4;
          }
          else
          if (tmp_str[i] == 'S' || tmp_str[i] == 's'){ // sets something
            if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's'){ // stepper motor speed and acceleration
              int motor_index; 
              int motor_speed, motor_acceleration;
              sscanf(tmp_str + i + 2, "%d%d%d", &motor_index, &motor_speed, &motor_acceleration);
              stepper_motors_controller.set_speed_and_acceleration(motor_index, motor_speed, motor_acceleration);
              i += 5;
            }
            else
              if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){ // potentiometer min max home
                int pot_index, pot_min, pot_max, pot_home, pot_pin, pot_dir;
                sscanf(tmp_str + i + 2, "%d%d%d%d%d%d", &pot_index, &pot_pin, &pot_min, &pot_max, &pot_home, &pot_dir);
                potentiometers_controller.set_params(pot_index, pot_pin, pot_min, pot_max, pot_home, pot_dir);
                i += 7;
            }
            else
              if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd'){ // dc motor speed
                int motor_index; 
                int motor_speed;
                sscanf(tmp_str + i + 2, "%d%d", &motor_index, &motor_speed);
                dc_motors_controller_TB6612FNG.set_speed(motor_index, motor_speed);
                i += 5;
              }

          }
        else
          if (tmp_str[i] == 'A' || tmp_str[i] == 'a'){// attach sensors to motors
            int motor_index, num_sensors;
            char motor_type = tmp_str[i];
            sscanf(tmp_str + i + 2, "%d%d", &motor_index, &num_sensors);
            if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
              stepper_motors_controller.set_num_attached_sensors(motor_index, num_sensors);
            else
              if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
                dc_motors_controller_TB6612FNG.set_num_attached_sensors(motor_index, num_sensors);
              
            // now I have to add sensors one by one
            int j = i + 4;
            i += 5;
            while (j < str_length){
              if (tmp_str[j] == 'P' || tmp_str[j] == 'p'){
                int sensor_index;
                sscanf(tmp_str + j + 1, "%d", &sensor_index);
                if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
                  stepper_motors_controller.add_sensor(motor_index, POTENTIOMETER, sensor_index);
                else
                  if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
                    dc_motors_controller_TB6612FNG.add_sensor(motor_index, POTENTIOMETER, sensor_index);
                i += 2;
              }
              else
                if (tmp_str[j] == 'B' || tmp_str[j] == 'b'){
                  int sensor_index;
                  sscanf(tmp_str + j + 1, "%d", &sensor_index);
                  if (motor_type == 'S' || motor_type == 's') // attach to stepper motors controller
                    stepper_motors_controller.add_sensor(motor_index, BUTTON, sensor_index);
                  else
                    if (motor_type == 'D' || motor_type == 'd') // attach to DC motors controller
                      dc_motors_controller_TB6612FNG.add_sensor(motor_index, BUTTON, sensor_index);
                  i += 2;
                }
                else
                  i++;
              j++;
            }
            i++;
          }
          else
          if (tmp_str[i] == 'G' || tmp_str[i] == 'g'){// for debugging purpose
            if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's'){ // gets stepper motor speed and acceleration
              int motor_index; 
              float motor_speed, motor_acceleration;
              sscanf(tmp_str + i + 2, "%d", &motor_index);
              stepper_motors_controller.get_speed_and_acceleration(motor_index, &motor_speed, &motor_acceleration);
              sprintf(serial_out, "GS%d %d %d#", motor_index, (int)motor_speed, (int)motor_acceleration);
              i += 4;
            }
            else
              if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd'){ // gets stepper motor speed and acceleration
                int motor_index; 
                byte motor_speed;
                sscanf(tmp_str + i + 2, "%d", &motor_index);
                dc_motors_controller_TB6612FNG.get_speed(motor_index, &motor_speed);
                sprintf(serial_out, "GD%d %d#", motor_index, motor_speed);
                i += 4;
              }
              else
                if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){ // get potentiometer min max home
                  int pot_index, pot_min, pot_max, pot_home, pot_dir;
                  byte pot_pin;
                  sscanf(tmp_str + i + 2, "%d", &pot_index);
                  potentiometers_controller.get_params(pot_index, &pot_pin, &pot_min, &pot_max, &pot_home, &pot_dir);
                  sprintf(serial_out, "GP%d %d %d %d %d %d#", pot_index, (int)pot_pin, pot_min, pot_max, pot_home, pot_dir);
                  i += 5;
                }
                else
                  if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u'){ // get potentiometer min max home
                    int ultrasound_index;
                    byte trig_pin, echo_pin;
                    sscanf(tmp_str + i + 2, "%d", &ultrasound_index);
                    ultrasonic_sensors_controller.get_sensor_pins(ultrasound_index, &trig_pin, &echo_pin);
                    sprintf(serial_out, "GU%d %d %d#", ultrasound_index, trig_pin, echo_pin);
                    i += 4;
                  }
                  else
                    i++;// incomplete string
        }
        else
         if (tmp_str[i] == 'T' || tmp_str[i] == 't'){// test connection
           sprintf(serial_out, "T#");
           i += 2;
         }
         else
         if (tmp_str[i] == 'V' || tmp_str[i] == 'v'){// version number
           sprintf(serial_out, "%s#", firmware_version);
           i += 2;
         }
         else
           if (tmp_str[i] == 'C' || tmp_str[i] == 'c'){// create something
             if (tmp_str[i + 1] == 'S' || tmp_str[i + 1] == 's'){// create a list of stepper motors
               if (!stepper_motors_controller.is_running()){           
                 int num_motors = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 stepper_motors_controller.set_num_motors(num_motors);
                 for (int k = 0; k < num_motors; k++){
                   int _step_pin, _dir_pin, _enable_pin;
                   sscanf(tmp_str + i + num_consumed_total, "%d%d%d%n", &_dir_pin, &_step_pin, &_enable_pin, &num_consumed);
                   stepper_motors_controller.set_pins(k, _dir_pin, _step_pin, _enable_pin);
                   num_consumed_total += num_consumed + 1;
                 }
     
                 sprintf(serial_out, "CS#");
                 i += num_consumed_total;
               }
             }
             else
             if (tmp_str[i + 1] == 'D' || tmp_str[i + 1] == 'd'){// create a list of DC motors
               if (!dc_motors_controller_TB6612FNG.is_running()){           
                 int num_motors = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 dc_motors_controller_TB6612FNG.set_num_motors(num_motors);
                 for (int k = 0; k < num_motors; k++){
                   int pwm_pin, dir_pin1, dir_pin2, enable_pin;
                   sscanf(tmp_str + i + num_consumed_total, "%d%d%d%d%n", &pwm_pin, &dir_pin1, &dir_pin2, &enable_pin, &num_consumed);
                   dc_motors_controller_TB6612FNG.set_motor_pins(k, pwm_pin, dir_pin1, dir_pin2, enable_pin);
                   num_consumed_total += num_consumed + 1;
                 }
     
                 sprintf(serial_out, "CD#");
                 i += num_consumed_total;
               }
             }
             else
               if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u'){// create a list of ultrasonic sensors
               
                 int num_sonars = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_sonars, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 ultrasonic_sensors_controller.set_num_sensors(num_sonars);
                 for (int k = 0; k < num_sonars; k++){
                   int _trig_pin, _echo_pin;
                   sscanf(tmp_str + i + num_consumed_total, "%d%d%n", &_trig_pin, &_echo_pin, &num_consumed);
                   ultrasonic_sensors_controller.set_sensor_pins(k, _trig_pin, _echo_pin);
                   num_consumed_total += num_consumed + 1;
                 }
     
                 sprintf(serial_out, "CU#");
                 i += num_consumed_total;
             }
             else
               if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){// create a list of potentiometers
               
                 int num_potentiometers = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_potentiometers, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 potentiometers_controller.set_num_sensors(num_potentiometers);
                 for (int k = 0; k < num_potentiometers; k++){
                   int _out_pin;
                   int _low, _high, _home, _direction;
                   sscanf(tmp_str + i + num_consumed_total, "%d%d%d%d%d%n", &_out_pin, &_low, &_high, &_home, &_direction, &num_consumed);
                   potentiometers_controller.set_params(k, _out_pin, _low, _high, _home, _direction);
                   num_consumed_total += num_consumed + 1;
                 }
     
                 sprintf(serial_out, "CP#");
                 i += num_consumed_total;
               }
             else
               if (tmp_str[i + 1] == 'I' || tmp_str[i + 1] == 'i'){// create a list of infrared sensors
               
                 int num_infrareds = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_infrareds, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 infrared_sensors_controller.set_num_sensors(num_infrareds);
                 for (int k = 0; k < num_infrareds; k++){
                   int _out_pin;
                   int _low;
                   sscanf(tmp_str + i + num_consumed_total, "%d%d%n", &_out_pin, &_low, &num_consumed);
                   infrared_sensors_controller.set_params(k, _out_pin, _low);
                   num_consumed_total += num_consumed + 1;
                 }
                 sprintf(serial_out, "CI#");
                 i += num_consumed_total;
               }
             else
               if (tmp_str[i + 1] == 'B' || tmp_str[i + 1] == 'b'){// create a list of button sensors
               
                 int num_buttons = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_buttons, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 buttons_controller.set_num_sensors(num_buttons);
                 for (int k = 0; k < num_buttons; k++){
                   int _pin;
                   sscanf(tmp_str + i + num_consumed_total, "%d%n", &_pin, &num_consumed);
                   buttons_controller.set_params(k, _pin);
                   num_consumed_total += num_consumed + 1;
                 }
                 sprintf(serial_out, "CB#");
                 i += num_consumed_total;
               }
               else
                 i++; // incomplete string
         }
         else
           i++;// incomplete string
    }
    else
      i++;
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
  
  if (Serial.available() || current_buffer[0]) {
    int num_read = 0;
    char serial_buffer[65];
  
    //num_read = Serial.readBytes(serial_buffer, 64); //Read up to 64 bytes
    
    int incomingByte = Serial.read();
      if (incomingByte != -1){
      serial_buffer[0] = incomingByte;
      num_read = 1;
    }
    serial_buffer[num_read] = 0;// terminate the string
    if (serial_buffer[0] || current_buffer[0]){
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
        if ((current_buffer[i] >= 'A' && current_buffer[i] <= 'Z') || (current_buffer[i] >= 'a' && current_buffer[i] <= 'z')){// a command
          // find the terminal character #
          int j = current_buffer_length - 1;
          for (; j > i && current_buffer[j] != '#'; j--);// parse (from the end) until I find the termination char
          if (j > i){

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
          else{// the string is not completed ... so I must wait for more...
            break; // for i
          }
        }
    }
  }
  stepper_motors_controller.run_motors(&potentiometers_controller, serial_out);
  if (serial_out[0])
    Serial.write(serial_out);

  dc_motors_controller_TB6612FNG.update_motors(&buttons_controller, serial_out);
  if (serial_out[0])
    Serial.write(serial_out);

 //   Serial.print("freeMemory()=");
 // Serial.println(freeMemory());

  //delay(1000);
}
//------------------------------------------------------------------------------
