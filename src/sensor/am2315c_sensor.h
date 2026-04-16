#ifndef AM2315C_SENSOR_H
#define AM2315C_SENSOR_H

#include <Adafruit_AHTX0.h>

// Inicializa el sensor AM2315C/AHT.
// Asume que Wire.begin() ya se llamó en setup().
void init_am2315c();

// Lee temperatura y humedad del sensor.
// Devuelve true si la lectura fue exitosa, false en caso contrario.
// Los valores se retornan a través de parámetros por referencia.
bool read_am2315c(float &temperature, float &humidity);

// Establece valores de calibración para temperatura y humedad.
// tempOffset/humOffset son correcciones aditivas.
// tempScale/humScale son ajustes multiplicativos.
void set_am2315c_calibration(float tempOffset, float tempScale, float humOffset, float humScale);

// Lee temperatura y humedad calibradas.
bool read_am2315c_calibrated(float &temperature, float &humidity);

#endif // AM2315C_SENSOR_H
