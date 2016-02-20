# jenny5-firmware
Arduino firmware for controlling the Jenny 5 robot

The firmware can control multiple stepper and DC motors and can read data from various sensors (ultrasound, buttons, potentiometers, infrared sensors etc). If it is uploaded on a Arduino Nano, it can control up to 4 stepper motors. This limitation si due to the fact that the robot is controlled by multiple Arduino Nano boards and each board has only 14 digital pins from each 2 are for Serial communication and each motor requires 3 pins (dir, step and enable).

There are several commands that can be sent to the firmware (from a Serial port connection). Each command terminates with #.

  T# // test connection. Returns T#.
  
  MSx y# // Moves stepper motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement. Outputs MSx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the distance to go.
  
  MDx y# // Moves dc motor x for y miliseconds. Outputs MDx d# when motor rotation is over. If movement was complete, then d is 0, otherwise is the time to go.
  
  MVx y# // Moves servo motor x for y steps.");
  
  HSx# // Moves stepper motor x to home position. The first sensor in the list of sensors must be the potentiometer which establish the home position. The motor does nothing if no potentiometer is attached. Returns HSx#.
  
  HDx# // Moves DC motor x to home position. The first sensor in the list of sensors must be the button which establish the home position. The motor does nothing if no potentiometer is attached. Returns HDx#.
  
  HVx# // Moves servo motor x to home position.  Returns HVx#.");
  
  DSx#  // Disables stepper motor x. Outputs DSx# when done.
  
  DDx#  // Disables dc motor x. Outputs DDx# when done.
  
  Lx#  // Locks stepper motor x. Outputs Lx# when done.
  
  SSx s a# // Sets speed of stepper motor x to s and the acceleration to a.
  
  SDx s# // Sets speed of motor x to s.
  
  SPx min max home# // Sets the parameters of a potentiometer. Min and max are the limits where it can move and home is from where we bring the robot when we start.
  
  ASx n Py Bz ... # // Attach to stepper motor x a list of n sensors (like Potentiometer y, Button z etc).
  
  ADx n Py Bz ... # // Attach to dc motor x a list of n sensors (like Potentiometer y, Button z etc).
  
  AVx n Py Bz ... # // Attach to servo motor x a list of n sensors (like Potentiometer y, Button z etc).
  
  Ux# // Gets the distance as measured by the ultrasonic sensor x. Outputs Ux d# when the reading has been completed. Note that this works in \"background\" and can output after some time (and in the meantime the program can do something else.
  
  Bx# // Gets the status of the button x. Outputs Bx s#
  
  Px# // Gets the position of the potentiometer x. Outputs Px p#
  
  Ix# // Gets the value of infrared sensor x. Outputs Ix v#
  
  GSx# // Gets the parameters for stepper motor x: speed acceleration num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GSx s a 1 0 0#
  
  GDx# // Gets the parameters for dc motor x: speed num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GDx s a 1 0 0#
  
  GVx# // Gets the parameters for dc motor x: speed num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs GVx s a 1 0 0#
  
  GPx# // Gets the parameters for potentiometer x: min max home. Outputs PPx l u h#
  
  GUx# // Gets the parameters for ultrasound x: trig_pin echo_pin. Outputs UPx t e#
  
  CS n d1 s1 e1 d2 s2 e2# // Creates the stepper motors controller and set some of its parameters. n is the number of motors, d, s, e are dir, step and enable pins. Outputs CS# when done.
  
  CD n p1 d11 d12 e1 p2 d21 d22 e2# // Creates the dc motors controller and set some of its parameters. n is the number of motors, p is the pwm_pin, d1 and d2 are the direction pins and e is the enable pins. Outputs CD# when done.
  
  CV n# // Creates the servo motors controller and set some of its parameters. n is the number of motors, Outputs CV# when done.

  CP n p1 l1 h1 _h1 _d1 p2 l2 h2 _h2 _d2# // Creates the potentiometers controller and set some of its parameters. n is the number of potentiometers, p is the output pin, l, h and _h are bottom, upper and home position, _d is the directon of the sensor relative to the direction in which the motor is moving. Outputs CP# when done.
  
  CU n t1 e1 t2 e2# // Creates the ultrasonic controller and set some of its parameters. n is the number of sonars, t and e are trigger and echo pins. Outputs CU# when done.
  
  CI n p1 l1 p2 l2# // Creates the infrared controller and set some of its parameters. n is the number of infrared sensors, p is the analog pin and l is the low and e are trigger and echo pins. Outputs CI# when done.
  
  CB n p1 p2# // Creates the buttons controller and set some of its parameters. n is the number of button sensors, p is the digital pin. Outputs CB# when done.
  
  V# // Outputs version string. eg: 2016.01.17.0#

motor index is between 0 and number of declared motors - 1.
Each command is terminated with #.

Example:

MS0 100#

DS0#

This will move the stepper motor number 0 with 100 steps and then will disable it.

Multiple commands can be sent once and they will be executed one by one: MS0 100#DS0#
