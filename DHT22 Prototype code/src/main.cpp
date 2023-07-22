// DTH22 Library Test Code
// Version 1.0
// Luka Žagar   22 July 2023

#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

float humidity;
float temperature;
float heat_index;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();    // Read temperature as Celsius (the default)

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  heat_index = dht.computeHeatIndex(temperature, humidity, false);

  Serial.print("Humidity: ");     Serial.print(humidity);     Serial.print(" % \t");
  Serial.print("Temperature: ");  Serial.print(temperature);  Serial.print(" °C \t");
  Serial.print("Heat index: ");   Serial.print(heat_index);   Serial.println(" °C");
}