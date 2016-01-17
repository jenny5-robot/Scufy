#include <avr/pgmspace.h>

#include "ultrasonic_sensors_controller.h"
#include "potentiometers_controller.h"
#include "motors_control.h"
#include "button_controller.h"
#include "jenny5_types.h"
#include "infrared_sensors_controller.h"

byte potentiometers_pins[4] = {0, 1, 2, 3};
t_limit_pair potentiometer_limits[4] = {{500, 1000, 500}, {500, 1023, 500}, {200, 600, 400}, {300, 600, 500}};

byte ultrasonic_trig_pins[2] = {48, 50};
byte ultrasonic_echo_pins[2] = {49, 51};

byte infrared_pins[2] = {52, 53};

t_motors_control motors_control(4);
t_potentiometers_controller potentiometers_control (4, potentiometers_pins, potentiometer_limits);
t_ultrasonic_sensors_controller ultrasonic_sensors_controller (2, ultrasonic_trig_pins, ultrasonic_echo_pins);
t_infrared_sensors_controller infrared_sensors_control(2, infrared_pins);

char is_command_running;

char firmware_version[] = "2016.01.17.4";// year.month.day.build number

char current_buffer[65];

//#define DEBUG

unsigned long time;

//--------------------------------------------------------------------------------------------
void setup() 
{
  motors_control.reset_pins();

current_buffer[0] = 0;

  Serial.begin(115200); //Open Serial connection
  
  Serial.write("Jenny 5 firmware version: ");
  Serial.write(firmware_version);
  Serial.write('\n');
  Serial.write("#");// initialization is over; must check for # string

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
  Serial.println(F("Ux# // Gets the distance as measured by the ultrasonic sensor x. Outputs Ux d#."));
  Serial.println(F("Bx# // Gets the status of the button x. Outputs Bx s#"));
  Serial.println(F("Px# // Gets the position of the potentiometer x. Outputs Px p#"));
  Serial.println(F("Ix# // Gets the value of infrared sensor x. Outputs Ix v#"));
  Serial.println(F("GMx# // Gets the parameters for motor x: speed acceleration num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs MPx s a 1 0 0#"));
  Serial.println(F("GPx# // Gets the parameters for potentiometer x: min max home. Outputs Px l u h#"));
  Serial.println(F("G# // Debug string - unformated"));

  
  Serial.println(F("Motor index is between 0 and num_motors - 1"));
  
  Serial.println();
#endif
  
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
        sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_steps);
        motors_control.move_motor(motor_index, num_steps);
        motors_control.set_motor_running(motor_index, 1);
        is_command_running = 1;
        i += 4;
      }
      else
          if (tmp_str[i] == 'P' || tmp_str[i] == 'p'){// potentiometer            
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            int sensor_value = potentiometers_control.get_position(sensor_index);
            sprintf(serial_out, "P%d %d#", sensor_index, sensor_value);
            i += 2;
          }
        else
          if (tmp_str[i] == 'U' || tmp_str[i] == 'u'){// ultrasonic
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            int sensor_value = ultrasonic_sensors_controller.getDistanceForSensor(sensor_index);
            sprintf(serial_out, "U%d %d#", sensor_index, sensor_value);
            i += 2;
          }
        else
          if (tmp_str[i] == 'I' || tmp_str[i] == 'i'){// infrared
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            int sensor_value = infrared_sensors_control.get_distance(sensor_index);
            sprintf(serial_out, "I%d %d#", sensor_index, sensor_value);
            i += 2;
          }
        else
        if (tmp_str[i] == 'D' || tmp_str[i] == 'd'){// disables motor
          int motor_index;
          sscanf(tmp_str + i + 1, "%d", &motor_index);
          motors_control.disable_motor(motor_index);
          sprintf(serial_out, "D%d#", motor_index);
          i += 2;
        }
        else
          if (tmp_str[i] == 'L' || tmp_str[i] == 'l'){// locks motor
            int motor_index;
            sscanf(tmp_str + i + 1, "%d", &motor_index);
            motors_control.lock_motor(motor_index);
            sprintf(serial_out, "L%d#", motor_index);
            i += 2;
          }
        else
          if (tmp_str[i] == 'H' || tmp_str[i] == 'h'){// go home
            int motor_index;
            sscanf(tmp_str + i + 1, "%d", &motor_index);
            motors_control.go_home(motor_index);
            sprintf(serial_out, "H%d#", motor_index);
          }
          else
          if (tmp_str[i] == 'S' || tmp_str[i] == 's'){ // sets something
            if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm'){ // motor speed and acceleration
              int motor_index, motor_speed, motor_acceleration;
              sscanf(tmp_str + i + 2, "%d%d%d", &motor_index, &motor_speed, &motor_acceleration);
              motors_control.set_motor_speed_and_acceleration(motor_index, motor_speed, motor_acceleration);
              i += 5;
            }
            else
              if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){ // potentiometer min max home
                int pot_index, pot_min, pot_max, pot_home;
                sscanf(tmp_str + i + 2, "%d%d%d%d", &pot_index, &pot_min, &pot_max, &pot_home);
                potentiometers_control.set_limits(pot_index, pot_min, pot_max, pot_home);
                i += 7;
            }
          }
        else
          if (tmp_str[i] == 'A' || tmp_str[i] == 'a'){// attach sensors to motors
            int motor_index, num_sensors;
            sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_sensors);
            motors_control.set_num_attached_sensors(motor_index, num_sensors);
            // now I have to add sensors one by one
            int j = i + 4;
            i += 4;
            while (j < str_length){
              if (tmp_str[j] == 'P' || tmp_str[j] == 'p'){
                int sensor_index;
                sscanf(tmp_str + j + 1, "%d", &sensor_index);
                motors_control.add_sensor(motor_index, POTENTIOMETER, sensor_index);
                i += 2;
              }
              j++;
            }
            i++;
          }
        else
          if (tmp_str[i] == 'G' || tmp_str[i] == 'g'){// for debugging purpose
            if (tmp_str[i + 1] == 'M' || tmp_str[i + 1] == 'm'){ // get motor speed and acceleration
              int motor_index, motor_speed, motor_acceleration;
              sscanf(tmp_str + i + 2, "%d", &motor_index);
              motors_control.get_motor_speed_and_acceleration(motor_index, &motor_speed, &motor_acceleration);
              sprintf(serial_out, "MP%d %d %d#", motor_index, motor_speed, motor_acceleration);
              i += 4;
            }
            else
              if (tmp_str[i + 1] == 'P' || tmp_str[i + 1] == 'p'){ // get potentiometer min max home
                int pot_index, pot_min, pot_max, pot_home;
                sscanf(tmp_str + i + 2, "%d", &pot_index);
                potentiometers_control.get_limits(pot_index, &pot_min, &pot_max, &pot_home);
                sprintf(serial_out, "PP%d %d %d %d#", pot_index, pot_min, pot_max, pot_home);
                i += 4;
              }
        }
        else
         if (tmp_str[i] == 'T' || tmp_str[i] == 't'){// test connection
           sprintf(serial_out, "T#");
           i += 2;
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
      strcat(current_buffer, serial_buffer);
      
      #ifdef DEBUG
        Serial.write("initial buffer is=");
        Serial.write(current_buffer);
        Serial.println();
      #endif
      
      // parse from the beginning until I find a M, D, L, S, A, P, B, U, G, T
      int buffer_length = strlen(current_buffer);
      for (int i = 0; i < buffer_length; i++)
        if ((current_buffer[i] >= 'A' && current_buffer[i] <= 'Z') || (current_buffer[i] >= 'a' && current_buffer[i] <= 'z')){// a command
          // find the terminal character #
          int j = i + 1;
          for (; j < buffer_length && current_buffer[j] != '#'; j++);// parse until I find the termination char
          if (j < buffer_length){

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
  motors_control.run_motors(potentiometers_control, serial_out);
  if (serial_out[0])
    Serial.write(serial_out);
  
}
//------------------------------------------------------------------------------
