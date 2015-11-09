# jenny5-firmware
Arduino firmware for controlling the Jenny 5 robot

The firmware can control 4 motors (there is a constant defined in the program for this purpose). This limitation si due to the fact that the robot is controlled by multiple Arduino Nano boards and each board has 14 digital pins from each 2 are for Serial communication and each motor requires 3 pins (dir, step and enable).

Jenny 5 firmware uses AccelStepper library for controlling the stepper motors. This library can be downloaded from:

http://www.airspayce.com/mikem/arduino/AccelStepper/

and it must be copied in the "libraries" folder of Arduino.

There are 4 commands that can be sent to the firmware (from Serial port):

Mx y# -- Moves motor x with y steps. If y is negative the motor runs in the opposite direction.

Dx# -- Disables motor x.

Sx y# -- Sets speed of motor x to y.

Ax y# -- Sets acceleration of motor x to y.

motor index is between 0 and 3.
Each command is terminated with #.

Example:

M0 100#

D0#

This will move the motor number 0 with 100 steps and then will disable it.