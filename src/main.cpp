#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_timer.h>
#include <math.h>

// Incluir los archivos de cabecera modulares
#include "./sensor/bh1750_sensor.h"
#include "./sensor/am2315c_sensor.h"
#include "./sensor/sen0913_sensor.h"
#include "wifi_manager.h"

// --- Configuración de WiFi ---
const char* WIFI_SSID = "xxxxxx";
const char* WIFI_PASSWORD = "xxxxxxx";

// --- Configuración del Servidor ---
const char* API_BASE_URL = "https://aeronic.herokuapp.com/api/telemetry/";
const char* API_TOKEN = "xxxxxxxxxxxxxx";

// Certificado raíz (Amazon Root CA 1) para TLS
const char* ROOT_CA = \
"-----BEGIN CERTIFICATE-----\n" \
"xxxxxxxxxxxxxxxx/xxxxxxxxxxxx\n" \
"xxxxxxxxxxxxxxxxxxxxxxx\n" \
"-----END CERTIFICATE-----\n";

WiFiClientSecure secureClient;

// --- Configuración de Hardware ---
const int LED_PIN = 13;
const int SOIL_MOISTURE_PIN = 36;

// Patrones LED
enum LedPattern { LED_OFF, LED_SOLID, LED_BLINK_SLOW, LED_BLINK_FAST, LED_BLINK_RESTART };
volatile LedPattern ledPattern = LED_OFF;
bool ledOutputState = false;
unsigned long ledLastToggle = 0;

void setLedPattern(LedPattern p) {
  ledPattern = p;
  if (p == LED_SOLID) {
    digitalWrite(LED_PIN, HIGH);
    ledOutputState = true;
  } else if (p == LED_OFF) {
    digitalWrite(LED_PIN, LOW);
    ledOutputState = false;
  }
  ledLastToggle = millis();
}

void ledService() {
  unsigned long now = millis();
  unsigned long interval = 0;
  switch (ledPattern) {
    case LED_BLINK_SLOW: interval = 500; break;
    case LED_BLINK_FAST: interval = 150; break;
    case LED_BLINK_RESTART: interval = 80; break;
    case LED_SOLID:
      if (!ledOutputState) { digitalWrite(LED_PIN, HIGH); ledOutputState = true; }
      return;
    case LED_OFF:
    default:
      if (ledOutputState) { digitalWrite(LED_PIN, LOW); ledOutputState = false; }
      return;
  }

  if (now - ledLastToggle >= interval) {
    ledOutputState = !ledOutputState;
    digitalWrite(LED_PIN, ledOutputState ? HIGH : LOW);
    ledLastToggle = now;
  }
}

// Intervalo de medición en milisegundos (1 minuto)
unsigned long measurementInterval = 60000; 
uint64_t lastMeasurementUs = 0;

// Reinicio periódico preventivo
const unsigned long RESTART_MS = 24UL * 60UL * 60UL * 1000UL; 
unsigned long bootMillis = 0;

// Envía el payload con backoff exponencial topado
bool sendWithRetries(const String& fullUrl, const char* payload, int maxRetries = 4, unsigned long initialDelayMs = 10000) {
  int attempt = 0;
  const unsigned long MAX_BACKOFF_MS = 2UL * 60UL * 1000UL; // Tope de 2 minutos

  while (attempt <= maxRetries) {
    // Reconexión WiFi si se perdió la señal
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi desconectado. Intentando reconectar...");
      setLedPattern(LED_BLINK_SLOW);
      WiFi.disconnect();
      WiFi.reconnect();
      
      // Esperar hasta 5 segundos para ver si reconecta antes de abortar el intento
      unsigned long startWait = millis();
      while(WiFi.status() != WL_CONNECTED && millis() - startWait < 5000) {
        ledService();
        delay(100);
      }
      
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Fallo al reconectar WiFi.");
        return false; // Abortar envío, se intentará en el próximo ciclo de medición
      }
      Serial.println("WiFi reconectado exitosamente.");
    }

    Serial.printf("Intento %d de %d para enviar datos...\n", attempt + 1, maxRetries + 1);
    setLedPattern(LED_BLINK_FAST);
    
    HTTPClient http;
    http.begin(secureClient, fullUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
      Serial.printf("HTTP código: %d\n", httpResponseCode);
      if (httpResponseCode >= 200 && httpResponseCode < 300) {
        http.end();
        setLedPattern(LED_SOLID);
        return true; // Éxito
      }
    } else {
      Serial.printf("Error en POST: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();

    if (attempt == maxRetries) break; // Si fue el último intento, salir del bucle

    // Calcular backoff exponencial (initialDelayMs * 2^attempt)
    unsigned long waitMs = initialDelayMs * (1 << attempt); 
    if (waitMs > MAX_BACKOFF_MS) waitMs = MAX_BACKOFF_MS; // Topar al máximo permitido

    Serial.printf("Reintentando en %lu ms...\n", waitMs);

    // Espera no bloqueante
    unsigned long waited = 0;
    const unsigned long step = 100;
    while (waited < waitMs) {
      ledService();
      delay(step);
      waited += step;
    }
    attempt++;
  }

  Serial.println("Todos los reintentos fallaron. Se descartará este paquete.");
  setLedPattern(LED_BLINK_SLOW);
  return false;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(LED_PIN, OUTPUT);
  setLedPattern(LED_BLINK_FAST);

  setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  secureClient.setCACert(ROOT_CA);

  bootMillis = millis();

  init_bh1750();
  init_am2315c();
  init_sen0913(SOIL_MOISTURE_PIN);

  set_bh1750_calibration(0.0f, 1.0f);
  set_am2315c_calibration(0.0f, 1.0f, 0.0f, 1.0f);
  set_sen0913_calibration(4095, 1000);

  Serial.println("Inicialización completa. Iniciando mediciones...");
  setLedPattern(LED_SOLID);
}

void loop() {
  ledService();

  if (millis() - bootMillis >= RESTART_MS) {
    Serial.println("Reiniciando por mantenimiento preventivo...");
    setLedPattern(LED_BLINK_RESTART);
    unsigned long waited = 0;
    while (waited < 2000) { ledService(); delay(100); waited += 100; }
    ESP.restart();
  }

  uint64_t nowUs = (uint64_t)esp_timer_get_time();
  if (nowUs - lastMeasurementUs >= (uint64_t)measurementInterval * 1000ULL) {
    lastMeasurementUs = nowUs;

    float lux = read_bh1750_calibrated();
    float temp = 0.0f, hum = 0.0f;
    bool am2315c_success = read_am2315c_calibrated(temp, hum);
    int moisture_raw = read_sen0913();
    int moisture_percent = read_sen0913_percent();

    // Optimización del Heap usando snprintf y un buffer de char estático
    char payloadBuffer[200];
    snprintf(payloadBuffer, sizeof(payloadBuffer), 
             "iluminacion_lux=%.2f&temperatura_c=%.2f&humedad_rh=%.2f&humedad_suelo_raw=%d&humedad_suelo_pct=%d", 
             lux, temp, hum, moisture_raw, moisture_percent);

    Serial.println("--- Nueva Medición ---");
    Serial.println(payloadBuffer);

    String fullUrl = String(API_BASE_URL) + API_TOKEN;
    
    // Llamada a la función de envío (la propia función verifica el WiFi)
    bool sent = sendWithRetries(fullUrl, payloadBuffer, 4, 10000);
    if (!sent) {
      Serial.println("Envío fallido. El dispositivo intentará de nuevo en el próximo ciclo.");
    }
  }
}