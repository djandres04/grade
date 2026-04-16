#include "data_formatter.h"
#include <ArduinoJson.h>

String format_sensor_json(
    float lux,
    float temp,
    float humidity,
    int moisture_raw,
    int moisture_percent,
    const char* lux_key,
    const char* temp_key,
    const char* humidity_key,
    const char* moisture_raw_key,
    const char* moisture_percent_key
) {
    DynamicJsonDocument doc(256);

    doc[lux_key] = lux;
    doc[temp_key] = temp;
    doc[humidity_key] = humidity;
    doc[moisture_raw_key] = moisture_raw;
    doc[moisture_percent_key] = moisture_percent;

    String output;
    serializeJson(doc, output);

    return output;
}

String format_environment_json(
    const char* temp_key,
    const char* humidity_key,
    float temp,
    float humidity
) {
    DynamicJsonDocument doc(128);
    doc[temp_key] = temp;
    doc[humidity_key] = humidity;

    String output;
    serializeJson(doc, output);

    return output;
}

String format_moisture_json(
    const char* moisture_raw_key,
    const char* moisture_percent_key,
    int moisture_raw,
    int moisture_percent
) {
    DynamicJsonDocument doc(128);
    doc[moisture_raw_key] = moisture_raw;
    doc[moisture_percent_key] = moisture_percent;

    String output;
    serializeJson(doc, output);

    return output;
}
