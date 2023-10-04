#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "INFINITUME7D8";
const char *password = "kasumikasumi";

String serverName = "http://192.168.1.73:7800/";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

int contador = 0; // Variable para el contador inicializada en 0

const int botonAumentarPin = 4; // Pin del botón para aumentar el contador
const int botonDisminuirPin = 5; // Pin del botón para disminuir el contador

void post_data(String action, int quantity)
{
  DynamicJsonDocument json_chido(1024);
  json_chido["action"] = action;
  json_chido["quantity"] = quantity;

  String json_str;
  serializeJson(json_chido, json_str);

  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(json_str);

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void post_asc()
{
  post_data("asc", 1);
}

void post_desc()
{
  post_data("desc", 1);
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

  // Configura los pines de los botones como entrada
  pinMode(botonAumentarPin, INPUT);
  pinMode(botonDisminuirPin, INPUT);

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  // Leer el estado de los botones
  int estadoBotonAumentar = digitalRead(botonAumentarPin);
  int estadoBotonDisminuir = digitalRead(botonDisminuirPin);

  if (estadoBotonAumentar == HIGH)
  {
    // Incrementa el contador cuando se presiona el botón de aumentar
    contador++;
    post_asc(); // Envía datos al servidor
  }
  else if (estadoBotonDisminuir == HIGH)
  {
    // Disminuye el contador cuando se presiona el botón de disminuir
    contador--;
    post_desc(); // Envía datos al servidor
  }

  // Muestra el valor actual del contador en el puerto serie
  Serial.print("Contador: ");
  Serial.println(contador);

  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;

      String serverPath = serverName;

      http.begin(serverPath.c_str());

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
