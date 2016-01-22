#include "ultrasonic_sensors_controller.h"

//---------------------------------------------------------------------
t_ultrasonic_sensors_controller::t_ultrasonic_sensors_controller (void)
{
	num_sensors = 0;
	sensors = NULL;
}
//---------------------------------------------------------------------
void echoCheck(void)
{
  for (int i = 0; i < ultrasonic_sensors_controller.num_sensors; i++)
    if (ultrasonic_sensors_controller.sensors[i].sonar->check_timer()) { // This is how you check to see if the ping was received.
      int distance = ultrasonic_sensors_controller.sensors[i].sonar->ping_result / US_ROUNDTRIP_CM;
      char tmp_str[30];
      sprintf(tmp_str, "U%d %d#", i, distance);
      Serial.write(tmp_str);
    }
}
//---------------------------------------------------------------------
void t_ultrasonic_sensors_controller::trigger (byte sensor_index)
{
  if (sensor_index < num_sensors){
    sensors[sensor_index].sonar->ping_timer(echoCheck);
  }
}
//---------------------------------------------------------------------
void t_ultrasonic_sensors_controller::set_sensor_pins(byte sensor_index, byte trig_pin, byte echo_pin)
{
  if (sensor_index < num_sensors)
    sensors[sensor_index].create_init(trig_pin, echo_pin);
}
//---------------------------------------------------------------------
void t_ultrasonic_sensors_controller::set_num_sensors(byte new_num_sensors)
{
  if (new_num_sensors != num_sensors){
    num_sensors = new_num_sensors;
    if (sensors)
      delete[] sensors;

    if (num_sensors > 0){

      byte trig_pins[4];
      trig_pins[0] = 8;
      trig_pins[1] = 5;
      trig_pins[2] = 8;
      trig_pins[3] = 11;
    
      byte echo_pins[4];
      echo_pins[0] = 9;
      echo_pins[1] = 6;
      echo_pins[2] = 9;
      echo_pins[3] = 12;
    
      sensors = new t_ultrasonic[num_sensors];
      
      for (byte m = 0; m < num_sensors; m++)
        sensors[m].create_init(trig_pins[m], echo_pins[m]);
    }
    else
      sensors = NULL;
  }
}
//---------------------------------------------------------------------
byte t_ultrasonic_sensors_controller::get_num_sensors(void)
{
  return num_sensors;
}
//---------------------------------------------------------------------
void t_ultrasonic_sensors_controller::get_sensor_pins(byte sensor_index, byte *trig_pins, byte *echo_pins)
{
  sensors[sensor_index].get_sensor_pins(trig_pins, echo_pins);
}
//---------------------------------------------------------------------
