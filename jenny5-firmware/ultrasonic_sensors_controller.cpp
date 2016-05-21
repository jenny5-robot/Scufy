#include "ultrasonic_sensors_controller.h"

//---------------------------------------------------------------------
t_ultrasonic_sensors_controller::t_ultrasonic_sensors_controller (void)
{
  num_sensors = 0;
  sensors = NULL;
}
//---------------------------------------------------------------------
void t_ultrasonic_sensors_controller::trigger (byte sensor_index)
{
  if (sensor_index < num_sensors) {
    sensors[sensor_index].trigger();
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
  if (new_num_sensors != num_sensors) {
    num_sensors = new_num_sensors;
    if (sensors)
      delete[] sensors;

    if (num_sensors > 0) {

      sensors = new t_ultrasonic[num_sensors];
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
void t_ultrasonic_sensors_controller::update_results(char *serial_out)
{
  serial_out[0] = 0;
  for (byte m = 0; m < num_sensors; m++) {
    if (sensors[m].check_echo()) {
      int distance = sensors[m].get_last_read_distance();
      if (distance > -1) {
        char tmp_s[10];
        sprintf(tmp_s, "U%d %d#", m, distance);
        strcat(serial_out, tmp_s);
      }
    }
    /*
      else {
      // distance is -1;
      // I must check if no more than 100 miliseconds have passed
      unsigned long millis_start = sensors[m].get_millis_start();
      if (millis_start) {
        unsigned long millis_now = millis();
        if (millis_now > sensors[m].get_millis_start() + 100) {
          char tmp_s[10];
          sprintf(tmp_s, "U%d 0#", m);
          strcat(serial_out, tmp_s);
          sensors[m].set_millis_start(0);
        }
      }
      }
    */
  }
}
//---------------------------------------------------------------------
