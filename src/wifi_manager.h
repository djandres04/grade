#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

// Inicializa la conexión WiFi con el SSID y contraseña proporcionados
void setup_wifi(const char* ssid, const char* password);

// Verifica si el ESP32 está conectado al WiFi (útil para reconexiones)
bool is_wifi_connected();

#endif // WIFI_MANAGER_H