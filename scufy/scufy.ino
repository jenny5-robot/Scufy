// Scufy - a general Arduino firmware for reading sensors and moving motors
// Scufy should run in 2KB of RAM (available from Arduino Nano / Uno).

// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

/*
	List of commands:

	T#
	- Test connection. 
	- Outputs T#.
	
	V#
	- Outputs version string (year.month.day.build_number): V2019.05.10.0#.

	// CREATE CONTROLLERS

	CS n dir1 step1 en1 dir2 step2 en2 ... dirn stepn enn# 
	- Creates the stepper motors controller and set some of its parameters.
	- n is the number of motors, dir*, step*, en* are dir, step and enable pins. 
	- Outputs CS# when done.
	- Example: CS 3 5 4 12 7 6 12 9 8 12#

	CD n pin1 dir11 dir12 en1 pin2 dir21 dir22 en2 ... pinn dirn1 dirn2 enn# 
	- Creates the dc motors controller (TB6612FNG) and set some of its parameters. 
	- n is the number of motors, pin is the PWM_pin, dir*1 and dir*2 are the direction pins and en* is the enable pins. 
	- Outputs CD# when done.
	
	CV n pin1 pin2 ... pinn# 
	- Creates the servo motors controller and set its pins. 
	- n is the number of motors, pin* are Arduino pins where the motors are connected.
	- Outputs CV# when done.

	CP n p1 p2 ... pn# 
	- Creates the potentiometers controller and set some of its parameters. 
	- n is the number of potentiometers, p* is the output pin. 
	- Outputs CP# when done.

	CU n t1 e1 t2 e2 ... tn en#
	- Creates the ultrasonic controller and set some of its parameters. 
	- n is the number of sonars, t* and e* are trigger and echo pins. 
	- Outputs CU# when done.

	CIA n p1 p2 ... pn# 
	- Creates the infrared controller and set some of its parameters. 
	- n is the number of infrared sensors, p* is the analog pin index where this sensor is connected to Arduino. 
	- Outputs CIA# when done.
	
	CID n p1 p2 ... pn# 
	- Creates the infrared digital controller and set some of its parameters. 
	- n is the number of infrared digital sensors, p* is the digital pin index. 
	- Outputs CID# when done.

	CB n p1 p2 ... pn# 
	- Creates the buttons controller and set some of its parameters. 
	- n is the number of button sensors, p* is the digital pin. 
	- Outputs CB# when done.
	
	CTR# 
	- Creates the Tera Ranger One controller. 
	- Outputs CTR# when done.
	
	CA n p1 p2 .. pn#
	- Creates the AS5147 controller. 
	- n is the number of sensors, p* are pins. 
	- Outputs CA# when done.
	- Example: CA 3 18 19 20#

	CL dir_pin, step_pin, enable_pin, infrared_pin#
	- Creates LiDAR which is composed from a stepper motor, a Tera Ranger One distance sensor and an infrared sensor for 0 position detection.
	- Outputs CL# when done.

	// ATTACH SENSORS TO MOTORS

	ASx n Py end1 end2 home direction Ak end1 end2 home direction # 
	- Attach to stepper motor x a list of n sensors (like Potentiometer y, Button z, AS5147 etc).
	- y is the sensor index. Note that 0 is the first index.
	- end1 and end2 specify the sensor angular position guarding the motor movement. 
	- home specifies the home position of the motor.
	- direction specifies if the increasing values for motor will also increase the values of the sensor.
	- Outputs ASx# when done.
	- Example: AS0 1 A0 280 320 300 1#

	// STEPPER COMMANDS

	SMx y#
	- Moves stepper motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement. 
	- First motor has index 0.
	- Outputs SMx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go.
	- Example: SM1 100#

	SHx#
	- Moves stepper motor x to home position. 
	- The first sensor in the list of sensors will establish the home position. The motor does nothing if no sensor is attached. 
	- Outputs SHx# when done.

	SDx# 
	- Disables stepper motor x. 
	- Outputs SDx# when done.

	SLx#
	- Locks stepper motor x in the current position. 
	- Outputs SLx# when done.

	SSx s a#
	- Sets speed of stepper motor x to s and the acceleration to a.
	- Outputs SSx# when done.

	STx#
	- Stops motor x.
	- Outputs STx# when done.

	SGx y#
	- Moves stepper motor x to y sensor position. The first sensor in list will give the position.
	- Outputs SMx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go.
	- Example: SM1 100#

	// SERVO COMMANDS

	VMx y# 
	- Moves servo motor x to y position.
	- Outputs VMx d# when done. If the move is completed d is 0, otherwise d is 1.

	VHx#
	- Moves servo motor x to home position.  
	- Outputs VHx#.

	// READ SENSORS COMMANDS

	RUx# 
	- Read the distance as measured by the ultrasonic sensor x. 
	- Outputs RUx distance# when the reading has been completed. 

	RBx# 
	- Read the status of the button x. 
	- Outputs RBx s# where s is either 0 or 1.

	RPx# 
	- Read the position of the potentiometer x. 
	- Outputs RPx p#, where p is the position.

	RIx#
	- Read the value of infrared sensor x. 
	- Outputs RIx value#.

	RT# 
	- Read the value of Tera Ranger One sensor. 
	- Outputs RTx distance#.

	RAx# 
	- Read the value of AS5147 sensor. 
	- Outputs RAx angle#.

	RMx# 
	- Free memory. 
	- Outputs RM v#, where v is the number of free bytes.


	// LiDAR commands
	LG#
	- Lidar Go - starts the LiDAR. A stream of distances is sent to the serial port. 
	- Each data from LiDAR has the format L angle distance#, where angle is between 0 and 359 and distance is read by TeraRanger One.

	LH#
	- Lidar Halt - stops the LiDAR.
	- Outputs LH# when done.

	LS speed acceleration#
	- Sets the speed and acceleration of the stepper motor of the LiDAR.
	- Outputs LS# when done.


	// OTHERS
	
	E#
	- The firmware can output this string if there is something wrong with a given command.

	I information#
	- The firmware can output this string containing useful information about the progress of a command.

  */


#include <Wire.h>
#include "config.h"

#include "ultrasonic_sensors.h"
#ifdef USE_POTENTIOMETERS
#include "potentiometers.h"
#endif
#include "as5147s.h"
#include "servos.h"
#include "stepper_motors.h"
#include "buttons.h"
#include "jenny5_types.h"
#include "infrared_analog_sensors.h"
#include "tera_ranger_one.h"
#ifdef USE_TB6612FNG
#include "dc_motors_TB6612FNG.h"
#endif
#include "tera_ranger_one_lidar.h"
#include "parse_and_execute.h"

#include "memoryfree.h"

t_steppers_controller steppers_controller;
t_servos_controller servos_controller;

#ifdef USE_TB6612FNG
t_dc_motors_controller_TB6612FNG dc_motors_controller_TB6612FNG;
#endif

#ifdef USE_POTENTIOMETERS
t_potentiometers_controller potentiometers_controller;
#endif
t_as5147s_controller as5147s_controller;
t_ultrasonic_sensors_controller ultrasonic_sensors_controller;
t_infrared_analog_sensors_controller infrared_analog_sensors_controller;
t_buttons_controller buttons_controller;
t_tera_ranger_one_controller tera_ranger_one_controller;
t_tera_ranger_one_lidar_controller* tera_ranger_one_lidar_controller;


const char* firmware_version = "2019.06.15.0"; // year.month.day.build number

#define MAX_BUFFER_LENGTH 65

char current_buffer[MAX_BUFFER_LENGTH];

//#define DEBUG

//--------------------------------------------------------------------------------------------
void setup()
{	
	current_buffer[0] = 0;

	tera_ranger_one_lidar_controller = NULL;

	Serial.begin(115200); //Open Serial connection
	
	while (!Serial);
	while (Serial.available() > 0) 
		Serial.read();

	Serial.write("T#");// initialization is over; must check for T# string (which is the alive test)
	Serial.flush();	
}
//--------------------------------------------------------------
void write_info(char *str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		if (str[i] == '#')
			Serial.write('$');
		else
			Serial.write(str[i]);

}
//--------------------------------------------------------------------------------------------
void loop()
{//Main loop
	char serial_out[MAX_BUFFER_LENGTH];
	serial_out[0] = 0;

	// check to see if there are new results from ultrasonic sensor
	ultrasonic_sensors_controller.update_results(serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		Serial.flush();
	}

	tera_ranger_one_controller.update_results(serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		Serial.flush();
	}

	if (Serial.available() || current_buffer[0]) {
		byte num_read = 0;
		char serial_buffer[65];

		int incomingByte = Serial.read();
		if (incomingByte != -1) {
			//if (incomingByte >= 'A' && incomingByte <= 'z' || incomingByte >= '0' && incomingByte <= '9' || incomingByte == ' ' || incomingByte == '#') {
				serial_buffer[0] = incomingByte;
				num_read = 1;
			//}
			//Serial.write("aici");
		}
		serial_buffer[num_read] = 0;// terminate the string
		if (serial_buffer[0] || current_buffer[0]) {
			if (serial_buffer[0])
				if (strlen(serial_buffer) + strlen(current_buffer) < MAX_BUFFER_LENGTH) // concat only if it fits
					strcat(current_buffer, serial_buffer);
			/*
			if (current_buffer[0]) {
				Serial.write("I ");
				Serial.flush();
				write_info(current_buffer);
				Serial.flush();
				Serial.write("/");
				Serial.flush();
				write_info(serial_buffer);
				Serial.flush();
				Serial.write("#");
				Serial.flush();
			}
			*/
			
#ifdef DEBUG
			Serial.write("initial buffer is=");
			Serial.write(current_buffer);
			Serial.println();
#endif

			// parse from the beginning until I find a M, D, L, S, A, P, B, U, G, T
			int current_buffer_length = strlen(current_buffer);
//			bool command_found = false;
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
				//		command_found = true;
						/*
							Serial.write("I ");
							Serial.write(current_buffer + i, j - i);
							Serial.write("-#");
							Serial.flush();
							*/
							parse_and_execute_commands(current_buffer + i, j - i, serial_out,
								steppers_controller,
								servos_controller,

#ifdef USE_TB6612FNG
								dc_motors_controller_TB6612FNG,
#endif

#ifdef USE_POTENTIOMETERS
								potentiometers_controller,
#endif
								as5147s_controller,
								ultrasonic_sensors_controller,
								infrared_analog_sensors_controller,
								buttons_controller,
								tera_ranger_one_controller,
								tera_ranger_one_lidar_controller,
								firmware_version
							);
						if (serial_out[0]) {
						//	while (!Serial);
							Serial.write(serial_out);
							Serial.flush();
						}


						// remove the current executed command
						/*
						Serial.write("I ");
						Serial.print(j);
						Serial.write("*#");
						Serial.flush();
						*/
						strcpy(current_buffer, current_buffer + j + 1);// not sure if this is good due to overlaps
						/*
						Serial.write("I ");
						Serial.write(current_buffer);
						Serial.write("^#");
						Serial.flush();
						*/
#ifdef DEBUG
						Serial.write("buffer left=");
						Serial.write(current_buffer);
						Serial.write("\n----------------\n");
						//Serial.println(strlen(current_buffer)); // buffer length
#endif

						break; //for i
					}
					else { // the string is not completed ... so I must wait for more...
						//Serial.write("I break not found#");
						//Serial.flush();
						break; // for i
					}
				}
			//if (strlen(current_buffer) && !command_found)// wrong string
				//current_buffer[0] = 0;
		}
	}
	steppers_controller.run_motors(&as5147s_controller,
#ifdef USE_POTENTIOMETERS		
		&potentiometers_controller,
#endif		
		&buttons_controller, serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		Serial.flush();
		serial_out[0] = 0;
	}
#ifdef USE_TB6612FNG
	dc_motors_controller_TB6612FNG.update_motors(&buttons_controller, serial_out);
	if (serial_out[0]) {
		Serial.write(serial_out);
		serial_out[0] = 0;
	}
#endif
	if (tera_ranger_one_lidar_controller) {
		tera_ranger_one_lidar_controller->run_motor(serial_out);
		if (serial_out[0]) {
			Serial.write(serial_out);
			Serial.flush();
			serial_out[0] = 0;
		}
	}
}
//------------------------------------------------------------------------------
