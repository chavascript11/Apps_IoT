#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "chavascript phone";
const char *password = "rayomacuin";

const char *serverName = "http://192.168.176.37:7800/";

unsigned long lastTime = 0;
const unsigned long timerDelay = 5000;

void post_data(const char *action, int quantity) {
  DynamicJsonDocument json_chido(1024);
  json_chido["action"] = action;
  json_chido["quantity"] = quantity;

  String json_str;
  serializeJson(json_chido, json_str);

  WiFiClient client;  // Crear un objeto WiFiClient

  HTTPClient http;
  http.begin(client, serverName);  // Utilizar el objeto WiFiClient

  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json_str);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

// Resto del código sin cambios


// Resto del código sin cambios


void post_asc() {
  post_data("asc", 1);
}

void post_desc() {
  post_data("desc", 1);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

// Resto del código sin cambios

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    if (data == "+") {
      post_asc();
    } else if (data == "-") {
      post_desc();
    } else {
      Serial.println("Invalid command");
    }
  }

  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;  // Crear un objeto WiFiClient
      HTTPClient http;

      String serverPath = serverName;
      http.begin(client, serverPath.c_str());  // Utilizar el objeto WiFiClient

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
