#include "am2315c_sensor.h"

// Instancia de la biblioteca Adafruit AHTX0
Adafruit_AHTX0 aht;

static float _aht_temp_offset = 0.0f;
static float _aht_temp_scale = 1.0f;
static float _aht_hum_offset = 0.0f;
static float _aht_hum_scale = 1.0f;

void init_am2315c() {
  if (!aht.begin()) {
    Serial.println(F("Error: Failed to find AHT sensor. Check wiring."));
  }
}

bool read_am2315c(float &temperature, float &humidity) {
  sensors_event_t humidity_event, temp_event;
  if (aht.getEvent(&humidity_event, &temp_event)) {
    temperature = temp_event.temperature;
    humidity = humidity_event.relative_humidity;
    return true;
  } else {
    Serial.println(F("Error: Failed to read from AHT sensor."));
    temperature = 0.0f;
    humidity = 0.0f;
    return false;
  }
}

void set_am2315c_calibration(float tempOffset, float tempScale, float humOffset, float humScale) {
  _aht_temp_offset = tempOffset;
  _aht_temp_scale = tempScale;
  _aht_hum_offset = humOffset;
  _aht_hum_scale = humScale;
}

bool read_am2315c_calibrated(float &temperature, float &humidity) {
  if (!read_am2315c(temperature, humidity)) {
    return false;
  }
  temperature = temperature * _aht_temp_scale + _aht_temp_offset;
  humidity = humidity * _aht_hum_scale + _aht_hum_offset;
  return true;
}
