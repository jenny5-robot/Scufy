#include "Ultrasonic.h"
#include "motor_control.h"

Ultrasonic ultrasonic(53,52);
motor_control mc;

char is_command_running;

char firmware_version[] = "2015.11.12.0";

char current_buffer[65];

#define DEBUG

//--------------------------------------------------------------------------------------------
void setup() 
{
  mc.reset_pins();

  Serial.begin(9600); //Open Serial connection
  
  Serial.print("Jenny 5 firmware version: ");
  Serial.print(firmware_version);
  Serial.println();
  //Print function list for user selection5
  Serial.println("Commands are:");
  Serial.println("Mx y# // Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement.");
  Serial.println("Dx#  // Disables motor x.");
  Serial.println("Lx#  // Lock motor x.");
  Serial.println("Sx y# // Sets speed of motor x to y");
  Serial.println("Ax y# // Sets acceleration of motor x to y");
  Serial.println("Ux# // Meassures distance");
  Serial.println("Motor index is between 0 and num_motors - 1");
  
  Serial.println();

  current_buffer[0] = 0;//'\0';
  is_command_running = 0;
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
              strncpy(tmp_str, current_buffer + i + 1, j - i - 1);
              tmp_str[j - i - 1] = 0;

              #ifdef DEBUG
                Serial.write("current command is=");
                Serial.write(tmp_str);
                Serial.println();
              #endif

              if (current_buffer[i] == 'M' || current_buffer[i] == 'm'){// moves motor
                int motor_index, num_steps;
                sscanf(tmp_str, "%d%d", &motor_index, &num_steps);
                mc.move_motor(motor_index, num_steps);
                is_command_running = 1;
              }
              else
                if (current_buffer[i] == 'D' || current_buffer[i] == 'd'){// disables motor
                  int motor_index;
                  sscanf(tmp_str, "%d", &motor_index);
                  mc.disable_motor(motor_index);
                }
                else
                  if (current_buffer[i] == 'L' || current_buffer[i] == 'l'){// locks motor
                    int motor_index;
                    sscanf(tmp_str, "%d", &motor_index);
                    mc.lock_motor(motor_index);
                  }
                else
                  if (current_buffer[i] == 'U' || current_buffer[i] == 'u'){// ultrasonic
                    int i = 0;
                    while (i != 3)
                    {
                      Serial.print(ultrasonic.Ranging());
                      Serial.print(" cm.\n");
                      delay(1000);
                      ++i;
                    }
                  }
                else
                  if (current_buffer[i] == 'S' || current_buffer[i] == 's'){// motor speed
                    int motor_index, motor_speed;
                    sscanf(tmp_str, "%d%d", &motor_index, &motor_speed);
                    mc.set_motor_speed(motor_index, motor_speed);
                    is_command_running = 1;
                  }
                else
                  if (current_buffer[i] == 'A' || current_buffer[i] == 'a'){// motor acceleration
                    int motor_index, motor_acceleration;
                    sscanf(tmp_str, "%d%d", &motor_index, &motor_acceleration);
                    mc.set_motor_acceleration(motor_index, motor_acceleration);
                    is_command_running = 1;
                  }
                    
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
  for (int m = 0; m < num_motors; m++)
    if (mc.steppers[m]->distanceToGo()){
      mc.steppers[m]->run();
      is_one_motor_running = true;
    }
    else{
      mc.steppers[m]->setCurrentPosition(0);
    }
  if (!is_one_motor_running)
    is_command_running = 0;
}

