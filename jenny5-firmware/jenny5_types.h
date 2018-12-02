//---------------------------------------------------
#ifndef JENNY_5_TYPES_H
#define JENNY_5_TYPES_H

#define BUTTON 0
#define POTENTIOMETER 1
#define ULTRASOUND 2
#define INFRARED_ANALOG 3
#define INFRARED_DIGITAL 4
#define AS5147 5

#define MOTOR_JUST_STOPPED 0
#define MOTOR_STILL_RUNNING 1
#define MOTOR_DOES_NOTHING 2
//---------------------------------------------------

struct t_sensor_info {
	byte type;
	byte index;

	int min_pos;
	int max_pos;
	int home_pos;

	int8_t _direction;// 1 - same direction with motor, -1 - different direction
};
//---------------------------------------------------

#endif
