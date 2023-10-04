#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "INFINITUME7D8";
const char *password = "kasumikasumi";

String serverTemperatureURL = "http://192.168.1.73:7800/temperature";
String serverHumidityURL = "http://192.168.1.73:7800/humidity";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void post_temperature(float temperature)
{
  DynamicJsonDocument json_doc(1024);
  json_doc["temperature"] = temperature;

  String json_str;
  serializeJson(json_doc, json_str);

  HTTPClient http;
  http.begin(serverTemperatureURL);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json_str);

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code (temperature): ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code (temperature): ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void post_humidity(float humidity)
{
  DynamicJsonDocument json_doc(1024);
  json_doc["humidity"] = humidity;

  String json_str;
  serializeJson(json_doc, json_str);

  HTTPClient http;
  http.begin(serverHumidityURL);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json_str);

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code (humidity): ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code (humidity): ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setup()
{
  Serial.begin(115200);

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

  // No se necesita llamar a dht.begin(); en el caso del DHT11

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("% Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));

  post_temperature(t);
  post_humidity(h);

  // Tu código de loop existente aquí...

  delay(5000); // Espera 5 segundos antes de enviar otra lectura
}