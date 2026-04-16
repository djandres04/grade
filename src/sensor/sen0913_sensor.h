#ifndef SEN0913_SENSOR_H
#define SEN0913_SENSOR_H

#include <Arduino.h>

// Inicializa el sensor SEN0913.
// Requiere el número del pin analógico al que está conectado.
void init_sen0913(uint8_t pin);

// Lee el valor bruto del sensor.
// Devuelve un entero, normalmente de un ADC (por ejemplo, 0-4095 en ESP32).
int read_sen0913();

// Establece los valores de calibración de humedad de suelo:
// dryValue: valor ADC cuando el sensor está seco.
// wetValue: valor ADC cuando el sensor está completamente húmedo.
void set_sen0913_calibration(int dryValue, int wetValue);

// Lee el porcentaje de humedad calibrado (0-100).
int read_sen0913_percent();

#endif // SEN0913_SENSOR_H
