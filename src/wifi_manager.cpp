#include "wifi_manager.h"
#include <WiFi.h>

void setup_wifi(const char* ssid, const char* password) {
    Serial.print("Conectando a la red WiFi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA); // Configura el ESP32 como estación (cliente WiFi)
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado exitosamente.");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
}

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}