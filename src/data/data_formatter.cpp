#include "data_formatter.h"
#include <ArduinoJson.h>

String format_data_to_json(float lux, float temp, float humidity, int moisture) {
    // Define the JSON document.
    // The size can be adjusted based on the complexity of the JSON.
    // Use the ArduinoJson Assistant to calculate the required size:
    // https://arduinojson.org/v7/assistant/
    StaticJsonDocument<256> doc;

    // Add sensor values to the JSON document
    // Using descriptive keys for clarity
    doc["light_lux"] = lux;
    doc["temperature_c"] = temp;
    doc["humidity_rh"] = humidity;
    doc["soil_moisture_raw"] = moisture;

    // Serialize the JSON document to a String
    String output;
    serializeJson(doc, output);

    return output;
}
