#include "tera_ranger_one.h"
#include "tera_ranger_one_utils.h"

#include <wire.h>
#include <arduino.h>

//-------------------------------------------------------
t_tera_ranger_one_controller::t_tera_ranger_one_controller(void)
{
  last_read_distance = -1;
  trigger_started = false;
}
//-------------------------------------------------------
void t_tera_ranger_one_controller::create_init(void)
{
  last_read_distance = -1;
  millis_start = 0;

  Wire.begin();

  trigger_started = false;
}
//-------------------------------------------------------
void t_tera_ranger_one_controller::trigger(void)
{
  if (!trigger_started) {// don't call trigger again if the echo has not been received.
    last_read_distance = -1;

    Wire.beginTransmission(0x30);      // Transmit to TR1 (THIS IS THE I2C BASE ADDRESS, CHANGE HERE IN CASE IT IS DIFFERENT)
    Wire.write(0x00);  // Sends trigger byte
    Wire.endTransmission();

    Wire.requestFrom(0x30, 3);

    trigger_started = true;
  }
}
//-------------------------------------------------------
bool t_tera_ranger_one_controller::check_echo(void)
{
  if (trigger_started) {
    if (Wire.available() == 3) {
      byte buf[3];
      buf[0] = Wire.read();
      buf[1] = Wire.read();
      buf[2] = Wire.read();

      int distance = (buf[0] << 8) + buf[1]; // Calculate distance in mm

      int crc = crc8(buf, 2);                // Calculate checksum

      if ((crc - buf[2]) == 0)
        last_read_distance = distance;
      else
        last_read_distance = -1;

      trigger_started = false;
    }
    else
      return false;
  }
  else
    return false;
}
//-------------------------------------------------------
int t_tera_ranger_one_controller::get_last_read_distance(void)
{
  return last_read_distance;
}
//-------------------------------------------------------
void t_tera_ranger_one_controller::update_results(char *serial_out)
{
  serial_out[0] = 0;

    if (check_echo()) {
      int distance = get_last_read_distance();
      if (distance > -1) {
        char tmp_s[10];
        sprintf(tmp_s, "TR %d#", distance);
        strcat(serial_out, tmp_s);
      }
    }
}
//---------------------------------------------------------------------
