#include "Adafruit_Sensor.h"
#include "debug_log.h"

/**************************************************************************/
/*!
    @brief  Prints sensor information to serial console
*/
/**************************************************************************/

static const char * getSensorType(sensors_type_t type)
{
  switch (type)
  {
  case SENSOR_TYPE_ACCELEROMETER:
    return "Acceleration (m/s2)";
  case SENSOR_TYPE_MAGNETIC_FIELD:
    return "Magnetic (uT)";
  case SENSOR_TYPE_ORIENTATION:
    return "Orientation (degrees)";
  case SENSOR_TYPE_GYROSCOPE:
    return "Gyroscopic (rad/s)";
  case SENSOR_TYPE_LIGHT:
    return "Light (lux)";
  case SENSOR_TYPE_PRESSURE:
    return "Pressure (hPa)";
  case SENSOR_TYPE_PROXIMITY:
    return "Distance (cm)";
  case SENSOR_TYPE_GRAVITY:
    return "Gravity (m/s2)";
  case SENSOR_TYPE_LINEAR_ACCELERATION:
    return "Linear Acceleration (m/s2)";
  case SENSOR_TYPE_ROTATION_VECTOR:
    return "Rotation vector";
  case SENSOR_TYPE_RELATIVE_HUMIDITY:
    return "Relative Humidity (%)";
  case SENSOR_TYPE_AMBIENT_TEMPERATURE:
    return "Ambient Temp (C)";
  case SENSOR_TYPE_OBJECT_TEMPERATURE:
    return "Object Temp (C)";
  case SENSOR_TYPE_VOLTAGE:
    return "Voltage (V)";
  case SENSOR_TYPE_CURRENT:
    return "Current (mA)";
  case SENSOR_TYPE_COLOR:
    return "Color (RGBA)";
  }
}

void Adafruit_Sensor::printSensorDetails(void) {
  sensor_t sensor;
  getSensor(&sensor);
  LOG("------------------------------------\r\n"
      "Sensor:       %s\r\n"
      "Type:         %s\r\n"
      "Driver Ver:   %d\r\n"
      "Unique ID:    %d\r\n"
      "Min Value:    %f\r\n"
      "Max Value:    %f\r\n"
      "Resolution:   %f\r\n"
      "------------------------------------\r\n", 
      sensor.name,
      getSensorType((sensors_type_t)sensor.type),
      sensor.version
      sensor.sensor_id,
      sensor.min_value,
      sensor.max_value,
      sensor.resolution
      );
}
