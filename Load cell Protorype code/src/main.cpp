// HX711 Library Test Code (complete project  at https://RandomNerdTutorials.com/arduino-load-cell-hx711/)
// Version 1.0
// Luka Žagar   22 July 2023

#include <Arduino.h>
#include <HX711.h>

const int LOADCELL_DOUT_PIN = 25;
const int LOADCELL_SCK_PIN = 26;

HX711 scale;

void setup() {
  Serial.begin(115200);
  
  Serial.println("Initializing the scale...");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("Read: ");           Serial.print(scale.read()); // print a raw reading from the ADC
  Serial.print("Read average: ");   Serial.print(scale.read_average(20)); // print the average of 20 readings from the ADC
  Serial.print("Get value: ");      Serial.print(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("Get units: ");      Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight (not set) divided
            
  scale.set_scale(87.7);  // this value is obtained by calibrating the scale with known weights
  scale.tare(); // reset the scale to 0

  Serial.println("After setting up the scale:");
  Serial.print("Read: ");           Serial.print(scale.read()); // print a raw reading from the ADC
  Serial.print("Read average: ");   Serial.print(scale.read_average(20)); // print the average of 20 readings from the ADC
  Serial.print("Get value: ");      Serial.print(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("Get units: ");      Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight, divided
}

void loop() {
  Serial.print("One reading: ");    Serial.print(scale.get_units(), 1);     Serial.print(" g \t");
  Serial.print("Average: ");        Serial.print(scale.get_units(10), 5);   Serial.println(" g \t");
}