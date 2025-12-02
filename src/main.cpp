#include <Arduino.h>
#include <Wire.h>

// Include the new modular header files
#include "bh1750_sensor.h"
#include "am2315c_sensor.h"
#include "sen0913_sensor.h"
#include "data_formatter.h"

// --- Configuration ---

// Define the pin for the analog soil moisture sensor
// GPIO 34 is a good choice as it's an ADC1 pin on many ESP32 boards.
const int SOIL_MOISTURE_PIN = 34;

// Variable to hold the measurement interval in milliseconds.
// This can be changed dynamically (e.g., via MQTT or HTTP later).
unsigned long measurementInterval = 5000; // 5 seconds

// Variable to store the timestamp of the last measurement.
unsigned long lastMeasurementTime = 0;


void setup() {
  // Start Serial communication for debugging.
  Serial.begin(115200);
  // Start the I2C bus. This is required for the BH1750 and AM2315C sensors.
  Wire.begin();

  Serial.println("Initializing all sensors...");

  // Initialize all sensors using the functions from the new modules.
  init_bh1750();
  init_am2315c();
  init_sen0913(SOIL_MOISTURE_PIN);

  Serial.println("Sensor initialization complete. Starting measurements...");
}

void loop() {
  // Use a non-blocking delay to check if it's time to take a new measurement.
  // This approach keeps the loop() responsive for other tasks in the future.
  if (millis() - lastMeasurementTime >= measurementInterval) {
    // Update the last measurement time.
    lastMeasurementTime = millis();

    // Read data from all sensors.
    float lux = read_bh1750();
    float temp, hum;
    // The read_am2315c function returns true on success.
    bool am2315c_success = read_am2315c(temp, hum);
    int moisture = read_sen0913();

    // Format the collected data into a JSON string.
    String json_data = format_data_to_json(lux, temp, hum, moisture);

    // Print the JSON data to the Serial monitor.
    // In a future step, this string can be sent over MQTT or an HTTP POST request.
    Serial.println("--- New Measurement ---");
    Serial.println(json_data);
  }

  // You can add other non-blocking tasks here.
}