#include "ultrasonic_sensors_controller.h"
#include "potentiometers_controller.h"
#include "motors_control.h"
#include "button_controller.h"

byte potentiometers_pins[4] = {0, 1, 2, 3};
t_limit_pair potentiometer_limits[4] = {{500, 1000}, {0, 1023}, {200, 600}, {300, 600}};

byte ultrasonic_trig_pins[2] = {48, 50};
byte ultrasonic_echo_pins[2] = {49, 51};

t_motors_control motors_control(4);
t_potentiometers_controller potentiometers_control (4, potentiometers_pins, potentiometer_limits);
t_ultrasonic_sensors_controller ultrasonic_sensors_controller (2, ultrasonic_trig_pins, ultrasonic_echo_pins);

char is_command_running;

char firmware_version[] = "2015.11.22.0";// year.month.day.version

char current_buffer[65];

#define DEBUG

//--------------------------------------------------------------------------------------------
void setup() 
{
  motors_control.reset_pins();

  Serial.begin(9600); //Open Serial connection
  
  Serial.print("Jenny 5 firmware version: ");
  Serial.print(firmware_version);
  Serial.println();
  //Print function list for user selection5
  Serial.println("Commands are:");
  Serial.println("Mx y# // Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement.");
  Serial.println("Dx#  // Disables motor x.");
  Serial.println("Lx#  // Lock motor x.");
  Serial.println("Sx y# // Sets speed of motor x to y.");
  Serial.println("Ax y# // Sets acceleration of motor x to y.");
  Serial.println("Ux# // Gets the distance as measured by the ultrasonic sensor x.");
  Serial.println("Bx# // Gets the status of the button x.");
  Serial.println("Px# // Gets the position of the potentiometer x.");
  Serial.println("Ix# // Gets the status of infrared sensor x.");
  Serial.println("PL# // sets the min (lower) position for potentiometer x.");
  Serial.println("PU# // sets the max (upper) position for potentiometer x.");
  
  Serial.println("Motor index is between 0 and num_motors - 1");
  
  Serial.println();

  current_buffer[0] = 0;
  is_command_running = 0;

  //motors_control.add_sensor(0, senTypePotentiometer, 0);
}

//--------------------------------------------------------------------------------------------
void parse_and_execute_commands(char* tmp_str, byte str_length)
{
  byte i = 0;
  while (i < str_length){
    if (current_buffer[i] >= 'A' && current_buffer[i] <= 'Z' || current_buffer[i] >= 'a' && current_buffer[i] <= 'z'){
      if (tmp_str[i] == 'M' || tmp_str[i] == 'm'){// moves motor
        int motor_index, num_steps;
        sscanf(tmp_str + i + 1, "%d%d", &motor_index, &num_steps);
        motors_control.move_motor(motor_index, num_steps);
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
          if (tmp_str[i] == 'S' || tmp_str[i] == 's'){// motor speed
            int motor_index, motor_speed;
            sscanf(tmp_str + i + 1, "%d%d", &motor_index, &motor_speed);
            motors_control.set_motor_speed(motor_index, motor_speed);
            i += 3;
          }
        else
          if (tmp_str[i] == 'A' || tmp_str[i] == 'a'){// motor acceleration
            int motor_index, motor_acceleration;
            sscanf(tmp_str + i + 1, "%d%d", &motor_index, &motor_acceleration);
            motors_control.set_motor_acceleration(motor_index, motor_acceleration);
            i += 3;
          }
        else
          if (tmp_str[i] == 'U' || tmp_str[i] == 'u'){// ultrasonic
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            Serial.print(ultrasonic_sensors_controller.getDistanceForSensor(sensor_index));
            i++;
          }
        else
          if (tmp_str[i] == 'P' || tmp_str[i] == 'p'){// potentiometer
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            Serial.print(potentiometers_control.getPotentiometerValue(sensor_index));
            i++;
          }
        else
          if (tmp_str[i] == 'I' || tmp_str[i] == 'i'){// infrared
            int sensor_index;
            sscanf(tmp_str + i + 1, "%d", &sensor_index);
            //Serial.print(infrareds_control.get_infrared_value(sensor_index));
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
  
  if (!is_command_running && (Serial.available() || current_buffer[0])) {
    int num_read = 0;
    char arduino_buffer[65];

    num_read = Serial.readBytes(arduino_buffer, 64); //Read up to 64 bytes
    arduino_buffer[num_read] = 0;// terminate the string
    if (arduino_buffer[0] || current_buffer[0]){
      strcat(current_buffer, arduino_buffer);
      
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
              char tmp_str[64];
              strncpy(tmp_str, current_buffer + i, j - i);
              tmp_str[j - i] = 0;

              #ifdef DEBUG
                Serial.write("current command is=");
                Serial.write(tmp_str);
                Serial.println();
              #endif

              parse_and_execute_commands(tmp_str, j - i);
                    
              // remove the current executed command
              strcpy(current_buffer, current_buffer + j + 1);// not sure if this is good
              
              #ifdef DEBUG
                Serial.write("buffer left=");
                Serial.write(current_buffer);
                Serial.write("\n----------------\n");
                //Serial.println(strlen(current_buffer)); // buffer length
              #endif
              
              break; //for i
          }
          else{// the string is not completed ... so I must wait more...
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
      for (byte j = 0 ; j < motors_control.motor_sensor_count[m] ; ++j)
      {
        byte sensor_index = motors_control.sensor_data[m][j].index;
        t_sensor_type type = motors_control.sensor_data[m][j].sensor_type;

        if (senTypePotentiometer == type)
        {
            if (0 == potentiometers_control.isWithinLimits(sensor_index))
            {
              limit_reached = true;
            }
        }
        else if (senTypeUltrasound == type)
        {
          // deal with ultrasound sensor
        }
      }

      //Serial.write("limit_reached:");
      //Serial.write(limit_reached);
      //Serial.println();

      if (false == limit_reached)
      {
        motors_control.steppers[m]->run();
        is_one_motor_running = true;
      }
      else
      {
        motors_control.steppers[m]->setCurrentPosition(0);
      }

    }
    else{
      motors_control.steppers[m]->setCurrentPosition(0);
    }
  }

    
  if (!is_one_motor_running)
    is_command_running = 0;
}

