# jenny5-firmware
Arduino firmware for controlling the Jenny 5 robot

The firmware can control multiple stepper and DC motors and can read data from various sensors (ultrasound, buttons and rotary encoders). If it is uploaded on a Arduino Nano, it can control up to 4 stepper motors. This limitation si due to the fact that the robot is controlled by multiple Arduino Nano boards and each board has only 14 digital pins from each 2 are for Serial communication and each motor requires 3 pins (dir, step and enable).

Jenny 5 firmware uses AccelStepper library for controlling the stepper motors. This library is available from:

http://www.airspayce.com/mikem/arduino/AccelStepper/

I have copied it in the current folder. I have tried to copy it in the "libraries" folder of Arduino, but I could not compile the program. It seems that the current version of Arduino does not correctly include files if the are not included directly in the .ino file. See here for more details:

http://arduino.stackexchange.com/questions/9840/problem-importing-a-library-in-h-file

There are several commands that can be sent to the firmware (from Serial port):

Mx y# -- Moves motor x with y steps. If y is negative the motor runs in the opposite direction. The motor remains locked at the end of the movement.

Dx# -- Disables motor x.

Lx# -- Locks motor x.

Sx y z# -- Sets speed of motor x to y and the acceleration to z.

Ax n Uy Pz# -- Appends n sensors to motor x (like Ultrasound y and Potentiometer z).

Ux# -- Gets the distance from an ultrasound sensor.

Bx# -- Gets the state from a button.

Px# -- Gets the position of a potentiometer.

motor index is between 0 and number of declared motors - 1.
Each command is terminated with #.

Example:

M0 100#

D0#

This will move the motor number 0 with 100 steps and then will disable it.

Multiple commands can be sent once and they will be executed one by one: M0 100#D0#
