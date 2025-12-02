#include "bh1750_sensor.h"
#include <Wire.h>

// Create an instance of the BH1750 library
BH1750 lightMeter;

void init_bh1750() {
  // Initialize the BH1750 sensor
  // The begin method will try to initialize the sensor and returns true on success
  // We are using the continuous high resolution mode, which is the most common one
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    // If the sensor fails to initialize, you can handle the error here
    // For now, we'll just print a message to the serial port if it's available
    // In a real application, you might want to implement more robust error handling
    #if defined(ARDUINO) && defined(SERIAL_PORT_MONITOR)
    SERIAL_PORT_MONITOR.println(F("Error: BH1750 initialization failed."));
    #endif
  }
}

float read_bh1750() {
  // Read the light level from the sensor
  // The readLightLevel() method returns the value in lux
  float lux = lightMeter.readLightLevel();
  if (lux < 0) {
    // A negative value indicates a measurement error
    #if defined(ARDUINO) && defined(SERIAL_PORT_MONITOR)
    SERIAL_PORT_MONITOR.println(F("Error: Failed to read from BH1750 sensor."));
    #endif
    return 0; // Return a safe value in case of error
  }
  return lux;
}
