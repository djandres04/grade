#ifndef AM2315C_SENSOR_H
#define AM2315C_SENSOR_H

#include <Adafruit_AHTX0.h>

// Function to initialize the AM2315C/AHT sensor.
// Assumes Wire.begin() has been called in the main setup.
void init_am2315c();

// Function to read temperature and humidity from the sensor.
// It returns true on a successful read, false otherwise.
// The values are returned via the reference parameters.
bool read_am2315c(float &temperature, float &humidity);

#endif // AM2315C_SENSOR_H
