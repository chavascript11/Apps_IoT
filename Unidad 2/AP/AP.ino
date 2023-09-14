#include <ESP8266WiFi.h>

const char *ssid = "deepweb";
const char *password = "password";

void setup()
{
  Serial.begin(115200);
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
}

void loop() {}