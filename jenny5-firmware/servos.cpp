#include "servos.h"

//-------------------------------------
t_servos_controller::t_servos_controller()
{
	num_motors = 0;
}
//-------------------------------------------------------------------------------
void t_servos_controller::delete_memory(void)
{
	if (servos) {
		delete[] servos;
		servos = NULL;
	}
}
//-------------------------------------
void t_servos_controller::set_params(int servo_index, byte pin)
{
	servos[servo_index].attach(pin);
}
//-------------------------------------
void t_servos_controller::set_num_servos(byte new_num_motors)
{
	if (new_num_motors != num_motors) {
		num_motors = new_num_motors;
		if (servos)
			delete[] servos;

		if (num_motors > 0) {
			servos = new Servo[num_motors];
		}
		else
			servos = NULL;
	}
}
//-------------------------------------
byte t_servos_controller::get_num_servos(void)
{
	return num_motors;
}
//-------------------------------------
void t_servos_controller::set_position(byte servo_index, byte position)
{
	servos[servo_index].write(position);
}
//-------------------------------------
byte t_servos_controller::get_position(byte servo_index)
{
	return servos[servo_index].read();
}
//-------------------------------------