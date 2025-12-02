#include "sen0913_sensor.h"

// Variable to store the pin number for the sensor
static uint8_t _sen0913_pin = 0;

void init_sen0913(uint8_t pin) {
    if (pin != 0) {
        _sen0913_pin = pin;
        // Set the pin mode to INPUT for analog reading
        pinMode(_sen0913_pin, INPUT);
    } else {
        #if defined(ARDUINO) && defined(SERIAL_PORT_MONITOR)
        SERIAL_PORT_MONITOR.println(F("Error: Invalid pin number for SEN0913 sensor."));
        #endif
    }
}

int read_sen0913() {
    if (_sen0913_pin != 0) {
        // Read the analog value from the specified pin
        return analogRead(_sen0913_pin);
    } else {
        // Return an error indicator if the pin was not initialized
        return -1;
    }
}
