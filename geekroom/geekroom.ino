#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>

// Set up GPIO
#define SENSOR 2

// Set up network
const char* networkName = "<YOUR_SSID>";
const char* networkPassword = "<YOUR_PASSWORD>";

// Http connection
char server[] = "<YOUR_SERVER_URL>";
int port = 80;
char requestPath[] = "/";
const char* bearerToken = "<YOUR_BEARER>";

WiFiClient client;
HttpClient httpClient = HttpClient(client, server, port);

bool isMoving = false;

void setup()
{
  pinMode(SENSOR, INPUT);
  connectToWiFi();
}

void loop() {
  if (!isWiFiConnected()) {
    connectToWiFi();
  }

  if (digitalRead(SENSOR) == HIGH) {
    if (!isMoving) {
      isMoving = true;
      sendState(isMoving);
    }
  } else if (isMoving) {
    isMoving = false;
    sendState(isMoving);
  }

  delay(1000);
}

void sendState(bool state) {
  String stateStr = getStateString(state);

  httpClient.beginRequest();
  httpClient.post(requestPath);
  httpClient.sendHeader("Content-Type", "application/json");
  httpClient.sendHeader("Content-Length", stateStr.length());
  httpClient.sendHeader("Authorization", "Bearer "  + String(bearerToken));
  httpClient.beginBody();
  httpClient.print(stateStr);
  httpClient.endRequest();
}

String getStateString(bool state) {
  String stateStr = "";

  if (state == HIGH) {
    stateStr = "{\"isBusy\": true}";
  } else {
    stateStr = "{\"isBusy\": false}";
  }

  return stateStr;
}

void connectToWiFi() {
  WiFi.begin(networkName, networkPassword);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(5000);
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
