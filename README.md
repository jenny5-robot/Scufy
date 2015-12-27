# jenny5-firmware
Arduino firmware for controlling the Jenny 5 robot

The firmware can control multiple stepper and DC motors and can read data from various sensors (ultrasound, buttons and rotary encoders). If it is uploaded on a Arduino Nano, it can control up to 4 stepper motors. This limitation si due to the fact that the robot is controlled by multiple Arduino Nano boards and each board has only 14 digital pins from each 2 are for Serial communication and each motor requires 3 pins (dir, step and enable).

Jenny 5 firmware uses AccelStepper library for controlling the stepper motors. This library is available from:

http://www.airspayce.com/mikem/arduino/AccelStepper/

It is copied in the current folder.

There are several commands that can be sent to the firmware (from Serial port):

Mx y# -- Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement.

Dx# -- Disables motor x. Outputs Dx# when done.

Lx# -- Locks motor x. Outputs Lx# when done.

Sx y z# -- Sets speed of motor x to y and the acceleration to z.

Ax n Uy Pz# -- Appends n sensors to motor x (like Ultrasound y and Potentiometer z).

Ux# -- Gets the distance from an ultrasound sensor. Outputs Ux d#.

Bx# -- Gets the state from a button. Outputs Bx s#.

Px# -- Gets the position of a potentiometer.  Outputs Px p#.

Ix# // Gets the value of infrared sensor x. Outputs Ix v#.

GMx# // Gets the parameters for motor x: speed acceleration num_sensors sensor_index1, sensor_type1 sensor_index1, sensor_type1. Outputs MPx s a 1 0 0#.

GPx# // Gets the parameters for potentiometer x: min max home. Outputs Px l u h#.

G# // Debug string - unformated.
  
motor index is between 0 and number of declared motors - 1.
Each command is terminated with #.

Example:

M0 100#

D0#

This will move the motor number 0 with 100 steps and then will disable it.

Multiple commands can be sent once and they will be executed one by one: M0 100#D0#
