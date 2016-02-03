#include <avr/pgmspace.h>

#include "ultrasonic_sensors_controller.h"
#include "potentiometers_controller.h"
#include "motors_control.h"
#include "button_controller.h"
#include "jenny5_types.h"
#include "infrared_sensors_controller.h"
//#include "MemoryFree.h"

t_motors_controller motors_controller;
t_potentiometers_controller potentiometers_controller;
t_ultrasonic_sensors_controller ultrasonic_sensors_controller;
t_infrared_sensors_controller infrared_sensors_controller;

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
  strcpy(firmware_version, "2016.02.01.1");
  
  //motors_controller.set_num_motors(2);
  
 // ultrasonic_sensors_controller.set_num_sensors(1);
  
  //potentiometers_control = new t_potentiometers_controller(4, potentiometers_pins, potentiometer_limits);
  //infrared_sensors_control = new t_infrared_sensors_controller (2, infrared_pins);
  
  current_buffer[0] = 0;

  Serial.begin(115200); //Open Serial connection

  #ifdef DEBUG
  Serial.write("Commands are:");
  Serial.println(F("T# // test connection. Returns T#."));
  Serial.println(F("Mx y# // Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement. Outputs Mx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go."));
  Serial.println(F("Hx# // Moves motor x to home position. The first sensor in the list of sensors must be the potentiometer which establish the home position. The motor does nothing if no potentiometer is attached."));
  Serial.println(F("Dx#  // Disables motor x. Outputs Dx# when done."));
  Serial.println(F("Lx#  // Lock motor x. Outputs Lx# when done."));
  Serial.println(F("SMx s a# // Sets speed of motor x to s and the acceleration to a."));
  Serial.println(F("SPx min max home# // Sets the parameters of a potentiometer. Min and max are the limits where it can move and home is from where we bring the robot when we start."));
  Serial.println(F("Ax n Py Bz ... # // Attach to motor x a list of n sensors (like Potentiometer y, Button z etc)."));
  Serial.println(F("Ux# // Gets the distance as measured by the ultrasonic sensor x. Outputs Ux d# when the reading has been completed. Note that this works in \"background\" and can output after some time (and in the meantime the program can do something else."));
  Serial.println(F("Bx# // Gets the status of the button x. Outputs Bx s#"));
  Serial.println(F("Px# // Gets the position of the potentiometer x. Outputs Px p#"));
  Serial.println(F("Ix# // Gets the value of infrared sensor x. Outputs Ix v#"));
  Serial.println(F("GMx# // Gets the parameters for motor x: speed acceleration num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs MPx s a 1 0 0#"));
  Serial.println(F("GPx# // Gets the parameters for potentiometer x: min max home. Outputs PPx l u h#"));
  Serial.println(F("GUx# // Gets the parameters for ultrasound x: trig_pin echo_pin. Outputs UPx t e#"));
  Serial.println(F("CM n d1 s1 e1 d2 s2 e2# // Creates the motors controller and set some of its parameters. n is the number of motors, d, s, e are dir, step and enable pins. Outputs CM# when done."));
  Serial.println(F("CP n p1 l1 h1 _h1 _d1 p2 l2 h2 _h2 _d2# // Creates the potentiometers controller and set some of its parameters. n is the number of potentiometers, p is the output pin, l, h and _h are bottom, upper and home position, _d is the directon of the sensor relative to the direction in which the motor is moving. Outputs CP# when done."));
  Serial.println(F("CU n t1 e1 t2 e2# // Creates the ultrasonic controller and set some of its parameters. n is the number of sonars, t and e are trigger and echo pins. Outputs CU# when done."));
  Serial.println(F("CI n p1 l1 p2 l2# // Creates the infrared controller and set some of its parameters. n is the number of infrared sensors, p is the analog pin and l is the low and e are trigger and echo pins. Outputs CU# when done."));
  
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
        int motor_index, num_steps;
        int num_read = sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_steps);
        if (num_read == 2){
          motors_controller.move_motor(motor_index, num_steps);
          is_command_running = 1;
          i += 4;
        }
        else
          i++;// error on incomplete string (does nothing)
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
          sscanf(tmp_str + i + 1, "%d", &motor_index);
          motors_controller.disable_motor(motor_index);
          sprintf(serial_out, "D%d#", motor_index);
          i += 2;
        }
        else
          if (tmp_str[i] == 'L' || tmp_str[i] == 'l'){// locks motor
            int motor_index;
            sscanf(tmp_str + i + 1, "%d", &motor_index);
            motors_controller.lock_motor(motor_index);
            sprintf(serial_out, "L%d#", motor_index);
            i += 2;
          }
        else
          if (tmp_str[i] == 'H' || tmp_str[i] == 'h'){// go home
            int motor_index;
            sscanf(tmp_str + i + 1, "%d", &motor_index);
            motors_controller.go_home(motor_index, &potentiometers_controller);
            sprintf(serial_out, "H%d#", motor_index);
            is_command_running = 1;
            i += 2;
          }
          else
          if (tmp_str[i] == 'S' || tmp_str[i] == 's'){ // sets something
            if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm'){ // motor speed and acceleration
              int motor_index; 
              int motor_speed, motor_acceleration;
              sscanf(tmp_str + i + 2, "%d%d%d", &motor_index, &motor_speed, &motor_acceleration);
              motors_controller.set_motor_speed_and_acceleration(motor_index, motor_speed, motor_acceleration);
              i += 5;
            }
            else
              if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){ // potentiometer min max home
                int pot_index, pot_min, pot_max, pot_home, pot_pin, pot_dir;
                sscanf(tmp_str + i + 2, "%d%d%d%d%d%d", &pot_index, &pot_pin, &pot_min, &pot_max, &pot_home, &pot_dir);
                potentiometers_controller.set_params(pot_index, pot_pin, pot_min, pot_max, pot_home, pot_dir);
                i += 7;
            }
          }
        else
          if (tmp_str[i] == 'A' || tmp_str[i] == 'a'){// attach sensors to motors
            int motor_index, num_sensors;
            sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_sensors);
            motors_controller.set_num_attached_sensors(motor_index, num_sensors);
            // now I have to add sensors one by one
            int j = i + 4;
            i += 4;
            while (j < str_length){
              if (tmp_str[j] == 'P' || tmp_str[j] == 'p'){
                int sensor_index;
                sscanf(tmp_str + j + 1, "%d", &sensor_index);
                motors_controller.add_sensor(motor_index, POTENTIOMETER, sensor_index);
                i += 2;
              }
              j++;
            }
            i++;
          }
        else
          if (tmp_str[i] == 'G' || tmp_str[i] == 'g'){// for debugging purpose
            if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm'){ // get motor speed and acceleration
              int motor_index; 
              float motor_speed, motor_acceleration;
              sscanf(tmp_str + i + 2, "%d", &motor_index);
              motors_controller.get_motor_speed_and_acceleration(motor_index, &motor_speed, &motor_acceleration);
              sprintf(serial_out, "MP%d %d %d#", motor_index, (int)motor_speed, (int)motor_acceleration);
              i += 4;
            }
            else
              if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){ // get potentiometer min max home
                int pot_index, pot_min, pot_max, pot_home, pot_dir;
                byte pot_pin;
                sscanf(tmp_str + i + 2, "%d", &pot_index);
                potentiometers_controller.get_params(pot_index, &pot_pin, &pot_min, &pot_max, &pot_home, &pot_dir);
                sprintf(serial_out, "PP%d %d %d %d %d %d#", pot_index, (int)pot_pin, pot_min, pot_max, pot_home, pot_dir);
                i += 5;
              }
            else
              if (tmp_str[i + 1] == 'U' || tmp_str[i + 1] == 'u'){ // get potentiometer min max home
                int ultrasound_index;
                byte trig_pin, echo_pin;
                sscanf(tmp_str + i + 2, "%d", &ultrasound_index);
                ultrasonic_sensors_controller.get_sensor_pins(ultrasound_index, &trig_pin, &echo_pin);
                sprintf(serial_out, "UP%d %d %d#", ultrasound_index, trig_pin, echo_pin);
                i += 4;
              }
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
             if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm'){// create a list of motors
               if (!motors_controller.is_motor_running()){           
                 int num_motors = 0;
                 
                 int num_consumed = 0;
                 sscanf(tmp_str + i + 3, "%d%n", &num_motors, &num_consumed);
                 
                 int num_consumed_total = 3 + num_consumed;
                 motors_controller.set_num_motors(num_motors);
                 for (int k = 0; k < num_motors; k++){
                   int _step_pin, _dir_pin, _enable_pin;
                   sscanf(tmp_str + i + num_consumed_total, "%d%d%d%n", &_dir_pin, &_step_pin, &_enable_pin, &num_consumed);
                   motors_controller.set_motor_pins(k, _dir_pin, _step_pin, _enable_pin);
                   num_consumed_total += num_consumed + 1;
                 }
     
                 sprintf(serial_out, "CM#");
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
         }
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
  motors_controller.run_motors(&potentiometers_controller, serial_out);
  if (serial_out[0])
    Serial.write(serial_out);

 //   Serial.print("freeMemory()=");
 // Serial.println(freeMemory());

  //delay(1000);
}
//------------------------------------------------------------------------------
