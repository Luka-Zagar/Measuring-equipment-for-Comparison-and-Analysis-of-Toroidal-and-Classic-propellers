// PWM Read on ESP32 Test Code
// Version 1.0
// Luka Žagar   22 July 2023

#include <Arduino.h>

#define SIGNALPIN_1 2

int SignalValue;
int MotorPower;

void setup() {
  Serial.begin(115200);
  pinMode(SIGNALPIN_1, INPUT);
}

void loop() {
  SignalValue = pulseIn(SIGNALPIN_1, HIGH);
  MotorPower = map(SignalValue, 1000, 2000, 0, 100);
  
  Serial.print("PWM Signal: ");   Serial.print(SignalValue);  Serial.print("\t");
  Serial.print("Power in %: ");   Serial.println(MotorPower);
}
