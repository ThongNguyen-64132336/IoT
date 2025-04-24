#include <WiFi.h>       // Thư viện Wifi
#include "DHTesp.h"     // Thư viện cảm biến nhiệt độ / độ ẩm
#include "ThingSpeak.h" // Thư viện ThingSpeak

const int DHT_PIN = 15;

const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

const int myChannelNumber = 2934052;
const char* myApiKey = "F086PWHDE17UKQB8";
const char* server = "api.thingspeak.com";

DHTesp dhtSensor;
WiFiClient client;

void setup() {
  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wifi chưa kết nối...");
  }
  Serial.println("Wifi đã kết nối!");
  Serial.println("Local IP: " + String(WiFi.localIP()));

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  ThingSpeak.setField(1, data.temperature);
  ThingSpeak.setField(2, data.humidity);

  int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);

  Serial.println("Temp: " + String(data.temperature) + "°C");
  Serial.println("Humidity: " + String(data.humidity) + "%");
  if (x == 200) {
    Serial.println("Data pushed successfully");
  } else {
    Serial.println("Push error: " + String(x));
  }

  Serial.println("---");
  delay(5000);
}
