#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "INFINITUME7D8";
const char *password = "kasumikasumi";

String serverLed = "http://192.168.1.73:7800/led"; // Endpoint para obtener el estado del LED

int ledPin = 2; // Pin al que está conectado el LED

void getLedState()
{
  HTTPClient http;
  http.begin(serverLed);

  int httpResponseCode = http.GET();

  if (httpResponseCode == 200)
  {
    // Leer el JSON de la respuesta
    String payload = http.getString();
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, payload);

    // Obtener el estado del LED desde el JSON
    bool ledState = jsonDoc["status"];
    
    // Actualizar el estado del LED en la placa Arduino
    digitalWrite(ledPin, ledState ? HIGH : LOW);

    Serial.print("Estado del LED: ");
    Serial.println(ledState);
  }
  else
  {
    Serial.print("Error al obtener el estado del LED. Código de respuesta: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Apagar el LED al inicio

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Obtener el estado actual del LED desde el servidor y actualizar el LED en la placa Arduino
  getLedState();
  delay(1000); // Esperar 1 segundos antes de volver a consultar el estado
}