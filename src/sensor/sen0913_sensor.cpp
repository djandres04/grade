#include "sen0913_sensor.h"

// Variable que guarda el número de pin del sensor
static uint8_t _sen0913_pin = 0;
static int _sen0913_dry_value = 0;
static int _sen0913_wet_value = 4095;

void init_sen0913(uint8_t pin) {
    if (pin != 0) {
        _sen0913_pin = pin;
        pinMode(_sen0913_pin, INPUT);
    } else {
        Serial.println(F("Error: Invalid pin number for SEN0913 sensor."));
    }
}

int read_sen0913() {
    if (_sen0913_pin != 0) {
        return analogRead(_sen0913_pin);
    } else {
        return -1;
    }
}

void set_sen0913_calibration(int dryValue, int wetValue) {
    _sen0913_dry_value = dryValue;
    _sen0913_wet_value = wetValue;
    if (_sen0913_dry_value == _sen0913_wet_value) {
        _sen0913_wet_value = _sen0913_dry_value > 0 ? _sen0913_dry_value - 1 : _sen0913_dry_value + 1;
    }
}

int read_sen0913_percent() {
    int raw = read_sen0913();
    if (raw < 0) {
        return -1;
    }

    int minValue = min(_sen0913_dry_value, _sen0913_wet_value);
    int maxValue = max(_sen0913_dry_value, _sen0913_wet_value);

    if (raw <= minValue) {
        return 0;
    }
    if (raw >= maxValue) {
        return 100;
    }

    float percent = 100.0f * (raw - minValue) / float(maxValue - minValue);
    return constrain(int(percent + 0.5f), 0, 100);
}
