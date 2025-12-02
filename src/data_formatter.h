#ifndef DATA_FORMATTER_H
#define DATA_FORMATTER_H

#include <Arduino.h>

// Function to format sensor data into a JSON string.
// Takes sensor values as input and returns a formatted String object.
String format_data_to_json(float lux, float temp, float humidity, int moisture);

#endif // DATA_FORMATTER_H
