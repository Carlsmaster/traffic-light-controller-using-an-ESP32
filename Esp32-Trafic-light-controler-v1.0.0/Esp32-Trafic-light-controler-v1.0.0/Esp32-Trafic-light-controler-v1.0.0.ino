/*
  Traffic Light Control Timer via Access Point

  This project implements a traffic light controller using an ESP32 microcontroller. It includes a web server to configure the duration of each traffic light state.

  Note: Adjust GPIO pin numbers according to your hardware configuration.

  Author: Eduardo Valdez
*/

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Define Wi-Fi credentials
const char* ssid = "SSID NAME"; // Replace with your Wi-Fi network name
const char* password = "PASSWORD"; // Replace with your Wi-Fi password

WebServer server(80);

// Traffic light duration in milliseconds
unsigned long duration = 20000; // Default: 20 seconds
const char* settingsPath = "/settings.json";

// Pin for simulating a traffic light
const int Led = 14;

void setup() {
  Serial.begin(115200);

  // Initialize LittleFS for file storage
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS initialization failed.");
    return;
  }

  // Load configuration from settings file
  loadConfig();

  // Set up ESP32 as an access point
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("IP: ");
  Serial.print(ip);
  Serial.print(" SSID: ");
  Serial.println(ssid);

  // Display current duration on serial monitor
  Serial.print("Current Duration: ");
  Serial.print(duration / 1000); // Convert to seconds
  Serial.println(" seconds");

  // Set up web server routes
  server.on("/", HTTP_GET, handleConnectionRoot);
  server.on("/SetDuration", HTTP_POST, handleSetDuration);

  // Start the web server
  server.begin();
  Serial.println("Web server started");

  // Set up the pin for the traffic light
  pinMode(Led, OUTPUT);
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();

  // Simulate a traffic light (turn on)
  digitalWrite(Led, HIGH);

  // Additional logic related to other devices or sensors can be added here
}

// Function to load configuration from the settings file
void loadConfig() {
  File configFile = LittleFS.open(settingsPath, "r");
  if (configFile) {
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, buf.get());
    duration = doc["duration"];
    configFile.close();
  }
}

// Function to save configuration to the settings file
void saveConfig() {
  DynamicJsonDocument doc(1024);
  doc["duration"] = duration;

  File configFile = LittleFS.open(settingsPath, "w");
  if (!configFile) {
    Serial.println("Error opening configuration file for writing");
    return;
  }

  serializeJson(doc, configFile);
  configFile.close();
}

// Handler for the root path, displays the current duration and a form to update it
void handleConnectionRoot() {
  unsigned long durationInSeconds = duration / 1000;
  String html = "<html><head><title>Traffic Light Configuration</title></head>";
  html += "<body style='background-color: #0C89C7;'>";
  html += "<center><h1 style='color: #FFFAF8;'>Traffic Light Duration</h1>";
  html += "<h1>Seconds: " + String(durationInSeconds) + "</h1>";
  html += "<form action='/SetDuration' method='post'>";
  html += "<input type='number' name='duration' value='" + String(durationInSeconds) + "' min='0'>";
  html += "<center><button type='submit' style='color: #10B007; height: 50px; width: 100px;'>Set New Duration</button>";
  html += "</form>";
  html += "</center></body></html>";
  server.send(200, "text/html", html);
}

// Handler for updating the duration through the web form
void handleSetDuration() {
  if (server.hasArg("duration")) {
    String durationString = server.arg("duration");
    unsigned long newDurationInSeconds = durationString.toInt();
    unsigned long newDurationMillis = newDurationInSeconds * 1000;

    // Update duration if it has changed
    if (newDurationMillis != duration) {
      duration = newDurationMillis;
      saveConfig();
    }

    // Send response and restart ESP32
    server.send(200, "text/plain", "Duration updated to: " + durationString + " seconds");
    Serial.println("Duration updated to: " + durationString + " seconds");
    ESP.restart();
    Serial.println("Restart completed.");
  } else {
    // Handle invalid or missing duration parameter
    server.send(400, "text/plain", "Missing or invalid 'duration' parameter.");
  }
}
