#ifndef SEN0913_SENSOR_H
#define SEN0913_SENSOR_H

#include <Arduino.h>

// Function to initialize the SEN0913 sensor.
// It requires the analog pin number it is connected to.
void init_sen0913(uint8_t pin);

// Function to read the raw value from the sensor.
// Returns an integer value, typically from an ADC (e.g., 0-4095 on ESP32).
int read_sen0913();

#endif // SEN0913_SENSOR_H
