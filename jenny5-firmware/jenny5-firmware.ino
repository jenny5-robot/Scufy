#include <AccelStepper.h>

#define num_motors 4

//Declare pin functions on Arduino
int dir_pins[num_motors] = {2,5,8,11};
int step_pins[num_motors] = {3,6,9,12};
int enable_pins[4] = {4,7,10,13};

int default_motor_speed = 200; //maximum steps per second 
int default_motor_acceleration = 100; //steps/second/second to accelerate

char is_command_running;

char firmware_version[] = "2015.11.11.0";

AccelStepper *steppers[num_motors];

char current_buffer[65];

#define DEBUG

//--------------------------------------------------------------------------------------------
void setup() 
{
  for (int m = 0; m < num_motors; m++){
    steppers[m] = new AccelStepper(AccelStepper::DRIVER, step_pins[m], dir_pins[m]);
    steppers[m]->setMaxSpeed(default_motor_speed);
    steppers[m]->setSpeed(default_motor_speed);
    steppers[m]->setAcceleration(default_motor_acceleration);
  }

  reset_pins();

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
  Serial.println("Motor index can be between 0 and num_motors - 1");
  Serial.println("Each command is terminated with #.");
  
  Serial.println();

  current_buffer[0] = 0;
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
      #endif
      
      // parse from the beginning until I find a M, D, S or A
      int buffer_length = strlen(current_buffer);
      for (int i = 0; i < buffer_length; i++)
        if (current_buffer[i] == 'M'){// move motor
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
              #endif
              
              int motor_index, num_steps;
              sscanf(tmp_str, "%d%d", &motor_index, &num_steps);
              move_motor(motor_index, num_steps);
              is_command_running = 1;

              // remove the current executed command
              strcpy(current_buffer, current_buffer + j + 1);// not sure if this is good
              
              #ifdef DEBUG
                Serial.write("buffer left=");
                Serial.write(current_buffer);
                Serial.println(strlen(current_buffer));
              #endif
              
              break; //for i
          }
          else{// the string is not completed ... so I must wait more...
            break; // for i
          }
        }
        else
          if (current_buffer[i] == 'D'){// Disables motor
            // find the terminal character #
            int j = i + 1;
            for (; j < buffer_length && current_buffer[j] != '#'; j++);// parse until I find the termination char
            if (j < buffer_length){
              char tmp_str[64];
              strncpy(tmp_str, current_buffer + i + 1, j - i - 1);
              tmp_str[j - i - 1] = 0;
              int motor_index;
              sscanf(tmp_str, "%d", &motor_index);
              disable_motor(motor_index);
              // remove the current executed command
              strcpy(current_buffer, current_buffer + j + 1);
              break;
            }
            else{// the string is not completed ... so I must wait more...
              break; // for i
            }
          }
        else
          if (current_buffer[i] == 'L'){// locks motor
            // find the terminal character #
            int j = i + 1;
            for (; j < buffer_length && current_buffer[j] != '#'; j++);// parse until I find the termination char
            if (j < buffer_length){
              char tmp_str[64];
              strncpy(tmp_str, current_buffer + i + 1, j - i - 1);
              tmp_str[j - i - 1] = 0;
              int motor_index;
              sscanf(tmp_str, "%d", &motor_index);
              lock_motor(motor_index);
              // remove the current executed command
              strcpy(current_buffer, current_buffer + j + 1);
              break;
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
    if (steppers[m]->distanceToGo()){
      steppers[m]->run();
      is_one_motor_running = true;
    }
    else{
      steppers[m]->setCurrentPosition(0);  
    }
  if (!is_one_motor_running)
    is_command_running = 0;
}
//--------------------------------------------------------------------------------------------
void move_motor(int motor_index, int num_steps)
{
  digitalWrite(enable_pins[motor_index], LOW); // turn motor on
  //Serial.println("Moving motor1 forward at default step mode.");
  steppers[motor_index]->moveTo(num_steps); //move num_steps
  //Serial.println("Enter new option");
  Serial.println();
}
//--------------------------------------------------------------------------------------------
void set_motor_speed(int motor_index, int motor_speed)
{
    steppers[motor_index]->setMaxSpeed(motor_speed);
    steppers[motor_index]->setSpeed(motor_speed);
}
//--------------------------------------------------------------------------------------------
void set_motor_acceleration(int motor_index, int motor_acceleration)
{
    steppers[motor_index]->setAcceleration(motor_acceleration);
}
//--------------------------------------------------------------------------------------------
void disable_motor(int motor_index)
{
  digitalWrite(enable_pins[motor_index], HIGH); // disable motor
}
//--------------------------------------------------------------------------------------------
void lock_motor(int motor_index)
{
  digitalWrite(enable_pins[motor_index], LOW); // enable motor
}
//--------------------------------------------------------------------------------------------
//Reset pins to default states
void reset_pins()
{
  for (int m = 0; m < num_motors; m++){
    digitalWrite(step_pins[m], LOW);
    digitalWrite(dir_pins[m], LOW);
    digitalWrite(enable_pins[m], HIGH); // all motors are disabled now
  }
}
//--------------------------------------------------------------------------------------------


