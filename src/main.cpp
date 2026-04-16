#include <Arduino.h>
#include <Wire.h>

// Incluir los archivos de cabecera modulares
#include "./sensor/bh1750_sensor.h"
#include "./sensor/am2315c_sensor.h"
#include "./sensor/sen0913_sensor.h"
#include "./data/data_formatter.h"
#include "wifi_manager.h"
#include "http_client.h"

// --- Configuración de WiFi ---
const char* WIFI_SSID = "xxxxxx";
const char* WIFI_PASSWORD = "xxxxxxx";

// --- Configuración del Servidor ---
const char* SERVER_URL = "http://xxx.xxx.xx.xxx:xxxx"; // <-- AQUÍ INDICASTE TU URL

// --- Configuración ---

// Define el pin para el sensor de humedad de suelo analógico
// GPIO 36 es una buena opción porque es un pin ADC1 en muchos ESP32.
const int SOIL_MOISTURE_PIN = 36;

// Variable para el intervalo de medición en milisegundos.
// Esto puede cambiarse dinámicamente (por ejemplo, vía MQTT o HTTP más adelante).
unsigned long measurementInterval = 5000; // 5 segundos

// Variable para almacenar el tiempo de la última medición.
unsigned long lastMeasurementTime = 0;


void setup() {
  // Inicia la comunicación Serial para depuración.
  Serial.begin(115200);
  // Inicia el bus I2C. Esto es necesario para los sensores BH1750 y AM2315C.
  Wire.begin();

  // Conectar a la red WiFi
  setup_wifi(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Inicializando todos los sensores...");

  // Inicializa todos los sensores usando las funciones de los nuevos módulos.
  init_bh1750();
  init_am2315c();
  init_sen0913(SOIL_MOISTURE_PIN);

  // Valores de calibración de ejemplo. Ajusta según tus referencias reales.
  set_bh1750_calibration(0.0f, 1.0f);
  set_am2315c_calibration(0.0f, 1.0f, 0.0f, 1.0f);
  set_sen0913_calibration(4095, 1000);

  Serial.println("Inicialización completa. Iniciando mediciones...");
}

void loop() {
  // Usa un retardo no bloqueante para verificar si es hora de una nueva medición.
  // Este enfoque mantiene loop() sensible a otras tareas en el futuro.
  if (millis() - lastMeasurementTime >= measurementInterval) {
    // Actualiza el tiempo de la última medición.
    lastMeasurementTime = millis();

    // Lee datos de todos los sensores.
    float lux = read_bh1750_calibrated();
    float temp = 0.0f, hum = 0.0f;
    // La función read_am2315c devuelve true en caso de éxito.
    bool am2315c_success = read_am2315c_calibrated(temp, hum);
    int moisture_raw = read_sen0913();
    int moisture_percent = read_sen0913_percent();

    // Formatea los datos con nombres de campo personalizados en el JSON.
    String json_data = format_sensor_json(
        lux,
        temp,
        hum,
        moisture_raw,
        moisture_percent,
        "iluminacion_lux",
        "temperatura_c",
        "humedad_rh",
        "humedad_suelo_raw",
        "humedad_suelo_pct"
    );

    // Ejemplos de otros payloads JSON con datos diferentes:
    // String env_json = format_environment_json("temp_c", "humedad", temp, hum);
    // String moisture_json = format_moisture_json("nivel_seco", "nivel_humedo", moisture_raw, moisture_percent);

    // Imprime los datos JSON en el monitor Serial.
    // En un paso futuro, esta cadena puede enviarse vía MQTT o HTTP POST.
    Serial.println("--- New Measurement ---");
    Serial.println(json_data);

    // Enviar los datos al servidor configurado
    send_json_to_server(SERVER_URL, json_data);
  }
}