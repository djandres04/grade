#include "bh1750_sensor.h"
#include <Wire.h>

// Instancia de la biblioteca BH1750
BH1750 lightMeter;

static float _bh1750_offset = 0.0f;
static float _bh1750_scale = 1.0f;

void init_bh1750() {
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("Error: BH1750 initialization failed."));
  }
}

float read_bh1750() {
  float lux = lightMeter.readLightLevel();
  if (lux < 0) {
    Serial.println(F("Error: Failed to read from BH1750 sensor."));
    return 0;
  }
  return lux;
}

void set_bh1750_calibration(float offset, float scale) {
  _bh1750_offset = offset;
  _bh1750_scale = scale;
}

float read_bh1750_calibrated() {
  float rawLux = read_bh1750();
  float calibratedLux = (rawLux + _bh1750_offset) * _bh1750_scale;
  return calibratedLux < 0 ? 0 : calibratedLux;
}
