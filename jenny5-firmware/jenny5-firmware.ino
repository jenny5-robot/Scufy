#include <avr/pgmspace.h>

#include "ultrasonic_sensors_controller.h"
#include "potentiometers_controller.h"
#include "motors_control.h"
#include "button_controller.h"
#include "jenny5_types.h"
#include "infrared_sensors_controller.h"

byte potentiometers_pins[4] = {0, 1, 2, 3};
t_limit_pair potentiometer_limits[4] = {{500, 1000}, {500, 1023}, {200, 600}, {300, 600}};

byte ultrasonic_trig_pins[2] = {48, 50};
byte ultrasonic_echo_pins[2] = {49, 51};

byte infrared_pins[2] = {52, 53};

t_motors_control motors_control(4);
t_potentiometers_controller potentiometers_control (4, potentiometers_pins, potentiometer_limits);
t_ultrasonic_sensors_controller ultrasonic_sensors_controller (2, ultrasonic_trig_pins, ultrasonic_echo_pins);
t_infrared_sensors_controller infrared_sensors_control(2, infrared_pins);

char is_command_running;

char firmware_version[] = "2015.12.25.1";// year.month.day.build number

char current_buffer[65];

#define DEBUG

//--------------------------------------------------------------------------------------------
void setup() 
{
  motors_control.reset_pins();

  Serial.begin(9600); //Open Serial connection
  
  Serial.print(F("Jenny 5 firmware version: "));
  Serial.print(firmware_version);
  Serial.println();
  //Print function list for user selection5
  Serial.println(F("Commands are:"));
  Serial.println(F("Mx y# // Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement."));
  Serial.println(F("Dx#  // Disables motor x."));
  Serial.println(F("Lx#  // Lock motor x."));
  Serial.println(F("Sx s a# // Sets speed of motor x to s and the acceleration to a."));
  Serial.println(F("Ax n Py Bz ... # // Attach to motor x a list of n sensors (like Potentiometer y, Button z etc)."));
  Serial.println(F("Ux# // Gets the distance as measured by the ultrasonic sensor x."));
  Serial.println(F("Bx# // Gets the status of the button x."));
  Serial.println(F("Px# // Gets the position of the potentiometer x."));
  Serial.println(F("Ix# // Gets the status of infrared sensor x."));
  Serial.println(F("G# // Prints for each motor: [sensor index, sensor type, sensor value, is whithin limits]"));
//  Serial.println(F("PL# // sets the min (lower) position for potentiometer x."));
//  Serial.println(F("PU# // sets the max (upper) position for potentiometer x."));
  
  Serial.println(F("Motor index is between 0 and num_motors - 1"));
  
  Serial.println();

  current_buffer[0] = 0;
}

//--------------------------------------------------------------------------------------------
void parse_and_execute_commands(char* tmp_str, byte str_length)
{
  byte i = 0;
  while (i < str_length){
    if (tmp_str[i] >= 'A' && tmp_str[i] <= 'Z' || tmp_str[i] >= 'a' && tmp_str[i] <= 'z'){
      if (tmp_str[i] == 'M' || tmp_str[i] == 'm'){// moves motor
        int motor_index, num_steps;
        sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_steps);
        motors_control.move_motor(motor_index, num_steps);
        motors_control.set_motor_running(motor_index, 1);
        is_command_running = 1;
        i += 3;
      }
      else
        if (tmp_str[i] == 'D' || tmp_str[i] == 'd'){// disables motor
          int motor_index;
          sscanf(tmp_str + i + 1, "%d", &motor_index);
          motors_control.disable_motor(motor_index);
          i++;
        }
        else
          if (tmp_str[i] == 'L' || tmp_str[i] == 'l'){// locks motor
            int motor_index;
            sscanf(tmp_str + i + 1, "%d", &motor_index);
            motors_control.lock_motor(motor_index);
            i++;
          }
        else
          if (tmp_str[i] == 'S' || tmp_str[i] == 's'){// motor speed and acceleration
            int motor_index, motor_speed, motor_acceleration;
            sscanf(tmp_str + i + 1, "%d%d%d", &motor_index, &motor_speed, &motor_acceleration);
            motors_control.set_motor_speed_and_acceleration(motor_index, motor_speed, motor_acceleration);
            i += 3;
          }
        else
          if (tmp_str[i] == 'A' || tmp_str[i] == 'a'){// attach sensors to motors
            int motor_index, num_sensors;
            sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_sensors);
            motors_control.set_num_attached_sensors(motor_index, num_sensors);
            // now I have to add sensors one by one
            int j = i + 3;
            while (j < str_length){
              if (tmp_str[j] == 'P' || tmp_str[j] == 'p'){
                int sensor_index;
                sscanf(tmp_str + j + 1, "%d", &sensor_index);
                motors_control.add_sensor(motor_index, POTENTIOMETER, sensor_index);
              }
              j++;
            }
            i += 3;
          }
        else
          if (tmp_str[i] == 'U' || tmp_str[i] == 'u'){// ultrasonic
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            Serial.write('U');
            Serial.print(ultrasonic_sensors_controller.getDistanceForSensor(sensor_index));
            Serial.write('#');
            i++;
          }
        else
          if (tmp_str[i] == 'P' || tmp_str[i] == 'p'){// potentiometer
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            Serial.write('P');
            Serial.print(potentiometers_control.getPotentiometerValue(sensor_index));
            Serial.write('#');
            i++;
          }
        else
          if (tmp_str[i] == 'I' || tmp_str[i] == 'i'){// infrared
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            Serial.write('I');
            Serial.print(infrared_sensors_control.get_distance(sensor_index));
            Serial.write('#');
            i++;
          }
        else
          if (tmp_str[i] == 'G' || tmp_str[i] == 'g'){// for debugging purpose
            int index;
            for(index = 0; index < motors_control.num_motors; index++) {
              Serial.print("Motor: ");
              Serial.println(index);
              for (byte j = 0 ; j < motors_control.sensors[index].count ; ++j) {
                byte sensor_index = motors_control.sensors[index].sensors_array[j].index;
                byte type = motors_control.sensors[index].sensors_array[j].sensor_type;
                Serial.print("Sensor index: ");
                Serial.print(sensor_index);
                Serial.print(", Type: ");
                
                if (POTENTIOMETER == type)
                  Serial.print("potentiometer, Value: ");
                  Serial.print(potentiometers_control.getPotentiometerValue(sensor_index));
                  Serial.print(", Is whitin limits: ");
                  Serial.println(potentiometers_control.isWithinLimits(sensor_index));
                }
            }
            i++;
        }
    }
    else
      i++;
  }
}
//--------------------------------------------------------------------------------------------
//Main loop
void loop() {

  if (Serial.available() || current_buffer[0]) {
    int num_read = 0;
    char serial_buffer[65];

    num_read = Serial.readBytes(serial_buffer, 64); //Read up to 64 bytes
    serial_buffer[num_read] = 0;// terminate the string
    if (serial_buffer[0] || current_buffer[0]){
      strcat(current_buffer, serial_buffer);
      
      #ifdef DEBUG
        Serial.write("initial buffer is=");
        Serial.write(current_buffer);
        Serial.println();
      #endif
      
      // parse from the beginning until I find a M, D, L, S, A, P, B, U
      int buffer_length = strlen(current_buffer);
      for (int i = 0; i < buffer_length; i++)
        if (current_buffer[i] >= 'A' && current_buffer[i] <= 'Z' || current_buffer[i] >= 'a' && current_buffer[i] <= 'z'){// a command
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

             //parse_and_execute_commands(tmp_str, j - i);
             parse_and_execute_commands(current_buffer + i, j - i);
             
                    
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
  
// run motors
  bool is_one_motor_running = false;
  for (int m = 0; m < motors_control.num_motors; m++)
  {
    bool limit_reached = false;

    if (motors_control.steppers[m]->distanceToGo())
    {
      for (byte j = 0 ; j < motors_control.sensors[m].count ; ++j)
      {
        byte sensor_index = motors_control.sensors[m].sensors_array[j].index;
        byte type = motors_control.sensors[m].sensors_array[j].sensor_type;

        if (POTENTIOMETER == type)
        {
            if (0 == potentiometers_control.isWithinLimits(sensor_index))
              limit_reached = true;
        }
        else if (ULTRASOUND == type)
        {
          // deal with ultrasound sensor
        }
      }

      if (!limit_reached)
      {
        motors_control.steppers[m]->run();
        is_one_motor_running = true;
      }
      else{
        motors_control.steppers[m]->setCurrentPosition(0);
        motors_control.set_motor_running(m, 0);
        Serial.write("M");
        Serial.print(m);
        Serial.write('#');
      }
    }
    else{
      motors_control.steppers[m]->setCurrentPosition(0);
// the motor has just finished the move, so we output that event
      if (motors_control.is_motor_running(m)){
        motors_control.set_motor_running(m, 0);
        Serial.write("M");
        Serial.print(m);
        Serial.write('#');
      }
    }
  }
}

