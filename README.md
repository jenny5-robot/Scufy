# jenny5-firmware
Arduino firmware for controlling the Jenny 5 robot

The firmware can control multiple stepper and DC motors and can read data from various sensors (ultrasound, buttons and rotary encoders). If it is uploaded on a Arduino Nano, it can control up to 4 stepper motors. This limitation si due to the fact that the robot is controlled by multiple Arduino Nano boards and each board has only 14 digital pins from each 2 are for Serial communication and each motor requires 3 pins (dir, step and enable).

There are several commands that can be sent to the firmware (from a Serial port connection). Each command terminates with #.

T# -- test connection. Outputs T#. This command is used to test if the connection to Arduino is alive.

Mx y# -- Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement.

Dx# -- Disables motor x. Outputs Dx# when done.

Lx# -- Locks motor x. Outputs Lx# when done.

SMx s a# -- Sets speed of motor x to s and the acceleration to a.

SPx min max home# -- Sets the parameters of a potentiometer. Min and max are the limits where it can move and home is from where we bring the robot when we start.

Ax n Uy Pz# -- Appends n sensors to motor x (like Ultrasound y and Potentiometer z).

Ux# -- Gets the distance from an ultrasound sensor. Outputs Ux d#.

Bx# -- Gets the state from a button. Outputs Bx s#.

Px# -- Gets the position of a potentiometer.  Outputs Px p#.

Ix# -- Gets the value of infrared sensor x. Outputs Ix v#.

GMx# -- Gets the parameters for motor x: speed acceleration num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs MPx s a 1 0 0#.

GPx# -- Gets the parameters for potentiometer x: min max home. Outputs Px l u h#.

GUx# -- Gets the parameters for ultrasound x: trig_pin echo_pin. Outputs UPx t e#.

CM n d1 s1 e1 d2 s2 e2# -- Creates the motors controller and set some of its parameters. n is the number of motors, d, s, e are dir, step and enable pins. Outputs CM# when done. This should be called only once because otherwise it will fragment the Arduino memmory too much.

CP n p1 l1 h1 _h1 p2 l2 h2 _h2# -- Creates the potentiometers controller and set some of its parameters. n is the number of potentiometers, p is the output pin, l, h and _h are bottom, upper and home position. Outputs CP# when done. This should be called only once because otherwise it will fragment the Arduino memmory too much.

CU n t1 e1 t2 e2# -- Creates the ultrasonic controller and set some of its parameters. n is the number of sonars, t and e are trigger and echo pins. Outputs CU# when done. This should be called only once because otherwise it will fragment the Arduino memmory too much.

CI n p1 l1 p2 l2# -- Creates the infrared controller and set some of its parameters. n is the number of infrared sensors, p is the analog pin and l is the low and e are trigger and echo pins. Outputs CU# when done. This should be called only once because otherwise it will fragment the Arduino memmory too much.

V# -- Ouputs version number.(eg: 2016.01.20.0#)
  
motor index is between 0 and number of declared motors - 1.
Each command is terminated with #.

Example:

M0 100#

D0#

This will move the motor number 0 with 100 steps and then will disable it.

Multiple commands can be sent once and they will be executed one by one: M0 100#D0#
