# Estación de Monitoreo Ambiental con ESP32

Este proyecto utiliza un microcontrolador ESP32 (placa Denky32) para recopilar datos de múltiples sensores ambientales y los formatea en una cadena JSON para su posterior procesamiento o transmisión.

## Características

- Lectura de intensidad de luz ambiental.
- Lectura de temperatura y humedad.
- Lectura de humedad del suelo.
- Formateo de datos en formato JSON.
- Arquitectura modular para cada sensor.

## Hardware Requerido

- Placa de desarrollo ESP32 (el código está configurado para una `denky32`).
- Sensor de luz **BH1750** (I2C).
- Sensor de temperatura y humedad **AM2315C** o compatible con AHT20 (I2C).
- Sensor de humedad del suelo **SEN0913** (Analógico).
- Cables de conexión (jumpers).

## Software y Librerías

- [Visual Studio Code](https://code.visualstudio.com/) con la extensión [PlatformIO IDE](https://platformio.org/platformio-ide).

Las siguientes librerías son gestionadas automáticamente por PlatformIO a través del archivo `platformio.ini`:

- `claws/BH1750 @ ^1.3.0`
- `bblanchon/ArduinoJson @ ^7.0.4`
- `adafruit/Adafruit AHTX0 @ ^2.0.5`

## Cómo Empezar

1.  **Clonar el Repositorio:**
    ```bash
    git clone <URL-del-repositorio>
    cd <nombre-del-repositorio>
    ```

2.  **Abrir en PlatformIO:**
    -   Abre Visual Studio Code.
    -   Haz clic en el ícono de PlatformIO en la barra lateral izquierda.
    -   Selecciona "Open Project" y elige la carpeta del proyecto que clonaste.

3.  **Conectar el Hardware:**
    -   Asegúrate de que todos los sensores y la placa ESP32 estén conectados correctamente según los pines definidos en el código.
        -   **Sensores I2C (BH1750, AM2315C):** Conecta SDA y SCL a los pines I2C correspondientes de tu ESP32.
        -   **Sensor Analógico (SEN0913):** Conecta la salida de datos al GPIO 34.

4.  **Construir y Subir:**
    -   Con la placa ESP32 conectada a tu computadora, utiliza los botones de PlatformIO en la barra de estado de VSCode:
        -   Haz clic en **PlatformIO: Build** (el ícono del checkmark) para compilar el proyecto.
        -   Haz clic en **PlatformIO: Upload** (el ícono de la flecha) para flashear el firmware en el ESP32.

5.  **Monitorear la Salida:**
    -   Haz clic en **PlatformIO: Serial Monitor** (el ícono del enchufe) para ver la salida de datos JSON. La velocidad de baudios está configurada en `115200`.

## Salida de Ejemplo (JSON)

Cada 5 segundos, verás una salida en el monitor serie similar a esta:

```json
{
  "light": 123.45,
  "temperature": 25.50,
  "humidity": 45.80,
  "soil_moisture": 550
}
```
