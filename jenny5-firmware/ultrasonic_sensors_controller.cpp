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
  for (int i = 0; i < ultrasonic_sensors_controller.num_sensors; i++) {
    byte result = ultrasonic_sensors_controller.sensors[i].sonar->check_timer();
    if (result == 1) { // event was received
      int distance = ultrasonic_sensors_controller.sensors[i].sonar->ping_result / US_ROUNDTRIP_CM;
      ultrasonic_sensors_controller.sensors[i].set_distance(distance);
      //  char tmp_str[30];
      //  sprintf(tmp_str, "U%d %d#", i, distance);
      //  Serial.write(tmp_str);
    }
    else if (result == 2) {
      ultrasonic_sensors_controller.sensors[i].set_distance(0); // no response within limits
    }
  }
}
//---------------------------------------------------------------------
void t_ultrasonic_sensors_controller::trigger (byte sensor_index)
{
  if (sensor_index < num_sensors) {
    sensors[sensor_index].trigger(echoCheck);
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
    int distance = sensors[m].get_last_read_distance();
    if (distance > -1) {
      char tmp_s[10];
      sprintf(tmp_s, "U%d %d#", m, distance);
      strcat(serial_out, tmp_s);
    }
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
  }
}
//---------------------------------------------------------------------
