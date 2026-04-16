#ifndef DATA_FORMATTER_H
#define DATA_FORMATTER_H

#include <Arduino.h>

// Formatea los datos de sensores en una cadena JSON.
// Recibe los valores y usa keys por defecto o personalizadas.
String format_sensor_json(
    float lux,
    float temp,
    float humidity,
    int moisture_raw,
    int moisture_percent,
    const char* lux_key = "light_lux",
    const char* temp_key = "temperature_c",
    const char* humidity_key = "humidity_rh",
    const char* moisture_raw_key = "soil_moisture_raw",
    const char* moisture_percent_key = "soil_moisture_percent"
);

// Formatea solo los datos de temperatura y humedad.
String format_environment_json(
    const char* temp_key,
    const char* humidity_key,
    float temp,
    float humidity
);

// Formatea solo los datos del sensor de humedad de suelo.
String format_moisture_json(
    const char* moisture_raw_key,
    const char* moisture_percent_key,
    int moisture_raw,
    int moisture_percent
);

#endif // DATA_FORMATTER_H
