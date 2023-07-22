// SDL_Arduino_INA3221 Library Test Code
// Version 1.0
// Luka Žagar   22 July 2023

#include <Arduino.h>
#include <Wire.h>
#include <SDL_Arduino_INA3221.h>

#define CHANNEL_1 1

SDL_Arduino_INA3221 ina3221(INA3221_ADDRESS, 0.004F);

void setup() {
  Serial.begin(115200);
  ina3221.begin();

  int MID = ina3221.getManufID();
  Serial.println(MID,HEX);
}

void loop() {
  float shuntvoltage1 = 0;
  float busvoltage1 = 0;
  float current_mA1 = 0;
  float loadvoltage1 = 0;
  float power1 = 0;
  
  busvoltage1 = ina3221.getBusVoltage_V(CHANNEL_1);
  shuntvoltage1 = ina3221.getShuntVoltage_mV(CHANNEL_1);
  current_mA1 = ina3221.getCurrent_mA(CHANNEL_1);  // [- means the battery is charging, + that it is discharging]
  
  loadvoltage1 = busvoltage1 + (shuntvoltage1 / 1000);
  power1 = loadvoltage1 * (current_mA1 / 1000);

  Serial.print("Bus Voltage: ");    Serial.print(busvoltage1);    Serial.print(" V \t");
  Serial.print("Load Voltage: ");   Serial.print(loadvoltage1);   Serial.print(" V \t");
  Serial.print("Shunt Voltage: ");  Serial.print(shuntvoltage1);  Serial.print(" mV \t");
  Serial.print("Current: ");        Serial.print(current_mA1);    Serial.print(" mA \t");
  Serial.print("Power: ");          Serial.print(power1);         Serial.println(" W \t");
}