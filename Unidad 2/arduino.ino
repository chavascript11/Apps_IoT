#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "INFINITUME7D8";
const char* password = "kasumikasumi";

// Your local server address
const char* serverAddress = "http://192.168.1.77:7800";

// Pin definitions
const int buttonD4 = 36;
const int buttonD5 = 48;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Setup button pins as input
  pinMode(buttonD4, INPUT_PULLUP);
  pinMode(buttonD5, INPUT_PULLUP);
}

void loop() {
  // Check if the D4 button is pressed
  if (digitalRead(buttonD4) == LOW) {
    // Create JSON data for D4 button
    String json = "{\"action\":\"desc\",\"quantity\":1}";

    // Send HTTP POST request
    sendPostRequest(json);

    // Delay to avoid multiple requests from a single press
    delay(1000);
  }

  // Check if the D5 button is pressed
  if (digitalRead(buttonD5) == LOW) {
    // Create JSON data for D5 button
    String json = "{\"action\":\"desc\",\"quantity\":1}";

    // Send HTTP POST request
    sendPostRequest(json);

    // Delay to avoid multiple requests from a single press
    delay(1000);
  }
}

void sendPostRequest(String json) {
  WiFiClient client;  // Crear un objeto WiFiClient

  HTTPClient http;

  // Specify the content type
  http.addHeader("Content-Type", "application/json");

  // Send the POST request
  http.begin(client, serverAddress);  // Iniciar la conexión HTTP con el objeto WiFiClient
  int httpResponseCode = http.POST(json);  // Enviar el cuerpo JSON

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
    Serial.println(serverAdress);
  }

  // Free resources
  http.end();
}

