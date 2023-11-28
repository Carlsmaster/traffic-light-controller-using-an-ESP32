
# ESP32 Traffic Light Controller

This project implements a traffic light controller using an ESP32 microcontroller. It includes a web server to configure the duration of each traffic light state.

## Components Used

- ESP32
- LEDs (for traffic lights)
- Sensor (for detecting vehicle presence)

## Dependencies

- [WiFi.h](https://www.arduino.cc/en/Reference/WiFi) - Library for connecting the ESP32 to Wi-Fi.
- [WebServer.h](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer) - Library for handling HTTP requests on the ESP32.
- [LittleFS.h](https://github.com/lorol/LITTLEFS) - Library for using LittleFS (ESP32 file system).
- [ArduinoJson.h](https://arduinojson.org/) - Library for working with JSON data.

## Setup

1. Connect the components according to the specified pins in the code.
2. Upload the code to your ESP32 using the Arduino IDE or any compatible platform.

## Configuration

1. Connect to the Wi-Fi network named `SSID NAME` with the password `PASSWORD`.
2. Open a web browser and navigate to the IP address displayed on the Serial Monitor.
3. Set the duration for each traffic light state using the web interface.

## Code Structure

- **`setup()`**: Initializes the ESP32, LittleFS, Wi-Fi, and sets up the web server. Also loads the configuration from the `settings.json` file.

- **`loop()`**: Handles incoming HTTP requests and controls the traffic light states based on sensor input.

- **`loadConfig()`**: Reads the duration configuration from the `settings.json` file.

- **`saveConfig()`**: Saves the duration configuration to the `settings.json` file.

- **`handleConnectionRoot()`**: Handles the root path request, displaying the current duration and providing a form to update it.

- **`handleSetDuration()`**: Handles the form submission, updating the duration configuration and triggering an ESP32 restart.

## LED Control

The LEDs are controlled using the `semaforo1` and `sidonSendState1` pins. Additional LED-related logic can be added in the `loop()` function.

## Notes

- The `duration` variable represents the time in milliseconds for each traffic light state.

- Configuration is stored in the `settings.json` file on the ESP32's LittleFS.

Feel free to customize this template based on your specific project details and requirements.
