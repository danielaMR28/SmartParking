#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>

// Replace with your network credentials
const char* ssid = "iPhone DMR";
const char* password = "contraseña";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

// Define pins for all 5 sensors
const int irSensorPins[5] = {4, 16, 17, 18, 19}; // Pins for 5 IR sensors
const int ledPins[5] = {23, 22, 21, 5, 13};     // Optional LED indicators for each spot

// Get Sensor Readings and return JSON object
String getSensorReadings(){
  String jsonString = "{";
  
  // Read all 5 sensors
  for (int i = 0; i < 5; i++) {
    int irValue = digitalRead(irSensorPins[i]);
    String status = (irValue == LOW) ? "0" : "1";
    
    // Update the JSON string
    jsonString += "\"ir_sensor" + String(i+1) + "\":\"" + status + "\"";
    
    // Add comma if not the last sensor
    if (i < 4) {
      jsonString += ",";
    }
    
    // Optional: Control LEDs based on sensor readings
    digitalWrite(ledPins[i], (irValue == LOW) ? HIGH : LOW);
  }
  
  jsonString += "}";
  return jsonString;
}

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char*)data;
    // Check if the message is "getReadings"
    if (message.equals("getReadings")) {
      // if it is, send current sensor readings
      String sensorReadings = getSensorReadings();
      Serial.println(sensorReadings);
      notifyClients(sensorReadings);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);
  
  // Initialize all sensor pins as INPUT
  for (int i = 0; i < 5; i++) {
    pinMode(irSensorPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
  
  initWiFi();
  initLittleFS();
  initWebSocket();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");

  // Start server
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    String sensorReadings = getSensorReadings();
    Serial.println(sensorReadings);
    notifyClients(sensorReadings);
    lastTime = millis();
  }
  ws.cleanupClients();
}
