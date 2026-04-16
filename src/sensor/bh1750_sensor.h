#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <BH1750.h>

// Inicializa el sensor BH1750.
// Asume que Wire.begin() ya se llamó en setup().
void init_bh1750();

// Lee el nivel de luz del sensor BH1750.
// Devuelve el nivel de luz en lux.
float read_bh1750();

// Establece la calibración para el sensor BH1750.
// offset: ajuste aditivo en lux.
// scale: ajuste multiplicativo de ganancia.
void set_bh1750_calibration(float offset, float scale = 1.0f);

// Lee el valor calibrado de luz en lux.
float read_bh1750_calibrated();

#endif // BH1750_SENSOR_H
