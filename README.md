# Scufy - the Arduino firmware

Arduino firmware for reading sensors and moving motors.

The firmware can control multiple stepper and DC motors and can read data from various sensors (ultrasound, buttons, potentiometers, infrared sensors etc). If it is uploaded on a Arduino Nano, it can control up to 4 stepper motors. This limitation si due to the fact that the robot is controlled by multiple Arduino Nano boards and each board has only 14 digital pins from each 2 are for Serial communication and each motor requires 3 pins (dir, step and enable).

There are several commands that can be sent to the firmware (from a Serial port connection). 
Each command terminates with #.

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


motor index is between 0 and number of declared motors - 1.
Each command is terminated with #.

Example:

MS0 100#

DS0#

This will move the stepper motor number 0 with 100 steps and then will disable it.

Multiple commands can be sent once and they will be executed one by one: MS0 100#DS0#
