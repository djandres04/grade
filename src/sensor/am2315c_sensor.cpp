#include "am2315c_sensor.h"

// Create an instance of the Adafruit AHTX0 library
Adafruit_AHTX0 aht;

void init_am2315c() {
  // Initialize the AHT sensor
  if (!aht.begin()) {
    #if defined(ARDUINO) && defined(SERIAL_PORT_MONITOR)
    SERIAL_PORT_MONITOR.println(F("Error: Failed to find AHT sensor. Check wiring."));
    #endif
  }
}

bool read_am2315c(float &temperature, float &humidity) {
  sensors_event_t humidity_event, temp_event;
  
  // getEvent() reads both temperature and humidity
  if (aht.getEvent(&humidity_event, &temp_event)) {
    temperature = temp_event.temperature;
    humidity = humidity_event.relative_humidity;
    return true;
  } else {
    // If reading fails, set values to a known error state and return false
    #if defined(ARDUINO) && defined(SERIAL_PORT_MONITOR)
    SERIAL_PORT_MONITOR.println(F("Error: Failed to read from AHT sensor."));
    #endif
    temperature = 0.0f;
    humidity = 0.0f;
    return false;
  }
}
