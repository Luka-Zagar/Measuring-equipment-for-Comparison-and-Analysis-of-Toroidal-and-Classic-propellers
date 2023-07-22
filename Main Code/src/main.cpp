// Main Code
// Version 1.0
// Luka Žagar   22 July 2023

#include <Arduino.h>
#include <Wire.h>
#include <SDL_Arduino_INA3221.h>
#include <HX711.h>
#include <DHT.h>

#define INA3221_CHANNEL_1 1

#define DHTPIN 4
#define DHTTYPE DHT22

const int LOADCELL_DOUT_PIN = 25;
const int LOADCELL_SCK_PIN = 26;
const int MOTOR_SIGNAL_PIN = 2;

const float SCALE_CALIBRATION = 87.7;

float shuntvoltage1;
float busvoltage1;
float current1;
float loadvoltage1;
float power1;

float scale_reading;
float scale_reading_average;

float humidity;
float temperature;
float heat_index;

int SignalValue;
int MotorPower;

SDL_Arduino_INA3221 ina3221(INA3221_ADDRESS, 0.004F);
HX711 scale;
DHT dht(DHTPIN, DHTTYPE);

void INA3221_setup() {
  ina3221.begin();

  int MID = ina3221.getManufID();
  Serial.println(MID,HEX);
}

void LOADCELL_setup() {
  Serial.println("Initializing the scale...");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("Read: ");           Serial.print(scale.read()); // print a raw reading from the ADC
  Serial.print("Read average: ");   Serial.print(scale.read_average(20)); // print the average of 20 readings from the ADC
  Serial.print("Get value: ");      Serial.print(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("Get units: ");      Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight (not set) divided
            
  scale.set_scale(SCALE_CALIBRATION);  // this value is obtained by calibrating the scale with known weights
  scale.tare(); // reset the scale to 0

  Serial.println("After setting up the scale:");
  Serial.print("Read: ");           Serial.print(scale.read()); // print a raw reading from the ADC
  Serial.print("Read average: ");   Serial.print(scale.read_average(20)); // print the average of 20 readings from the ADC
  Serial.print("Get value: ");      Serial.print(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()
  Serial.print("Get units: ");      Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight, divided
} 

void INA3221_update() {
  busvoltage1 = ina3221.getBusVoltage_V(INA3221_CHANNEL_1);
  shuntvoltage1 = ina3221.getShuntVoltage_mV(INA3221_CHANNEL_1);
  current1 = ina3221.getCurrent_mA(INA3221_CHANNEL_1);  // [- means the battery is charging, + that it is discharging]
  
  loadvoltage1 = busvoltage1 + (shuntvoltage1 / 1000);
  power1 = loadvoltage1 * (current1 / 1000);
}

void LOADCELL_update() {
  scale_reading = (scale.get_units(), 1);
  scale_reading_average = (scale.get_units(10), 5);
  }

void PWM_update() {
  SignalValue = pulseIn(MOTOR_SIGNAL_PIN, HIGH);
  MotorPower = map(SignalValue, 1000, 2000, 0, 100);
}

void DHT22_update() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();    // Read temperature as Celsius (the default)

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  heat_index = dht.computeHeatIndex(temperature, humidity, false);
}

void SENSORS_dataprint() {
  Serial.print("Bus Voltage: ");    Serial.print(busvoltage1);    Serial.print(" V \t");
  Serial.print("Load Voltage: ");   Serial.print(loadvoltage1);   Serial.print(" V \t");
  Serial.print("Shunt Voltage: ");  Serial.print(shuntvoltage1);  Serial.print(" mV \t");
  Serial.print("Current: ");        Serial.print(current1);       Serial.print(" mA \t");
  Serial.print("Power: ");          Serial.print(power1);         Serial.print(" W \t");

  Serial.print("PWM Signal: ");     Serial.print(SignalValue);    Serial.print("\t");
  Serial.print("Power in %: ");     Serial.print(MotorPower);     Serial.print(" % \t");

  Serial.print("One reading: ");    Serial.print(scale_reading);            Serial.print(" g \t");
  Serial.print("Average: ");        Serial.print(scale_reading_average);    Serial.print(" g \t");

  Serial.print("Humidity: ");       Serial.print(humidity);       Serial.print(" % \t");
  Serial.print("Temperature: ");     Serial.print(temperature);   Serial.print(" °C \t");
  Serial.print("Heat index: ");      Serial.print(heat_index);    Serial.println(" °C");
}

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_SIGNAL_PIN, INPUT);
  
  dht.begin();

  INA3221_setup();
  LOADCELL_setup();
}

void loop() {
  INA3221_update();
  LOADCELL_update();
  PWM_update();
  DHT22_update();

  SENSORS_dataprint();
}