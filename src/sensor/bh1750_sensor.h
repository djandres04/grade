#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <BH1750.h>

// Function to initialize the BH1750 sensor.
// Assumes Wire.begin() has been called in the main setup.
void init_bh1750();

// Function to read the light level from the BH1750 sensor.
// Returns the light level in lux.
float read_bh1750();

#endif // BH1750_SENSOR_H
