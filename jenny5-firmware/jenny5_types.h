//---------------------------------------------------
#ifndef JENNY_5_TYPES
#define JENNY_5_TYPES

#define BUTTON 0
#define POTENTIOMETER 1
#define ULTRASOUND 2
//---------------------------------------------------

struct t_sensor_info {
  byte  type;
  byte      index;
};
//---------------------------------------------------

#endif