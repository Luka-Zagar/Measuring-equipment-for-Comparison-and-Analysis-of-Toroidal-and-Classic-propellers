#include <Arduino.h>
#include <Servo.h>

int const ESC_PIN = 9;
int const START_PIN = A4;  //should share a GND
int const TEST_PIN = 7;

int Starting_PWM_value = 1000;
int Finishing_PWM_value = 2000;

int Ramp_jump = 10; 
int Ramp_timer = 1500;  // 1.5 seconds

int Start_state = 0;
int Number_of_RampUps = 1;
int RampUp_number = 0;

Servo SkyWalker_ESC;

void setup() {
  Serial.begin(115200);
  
  pinMode(START_PIN, INPUT);

  SkyWalker_ESC.attach(ESC_PIN);
  SkyWalker_ESC.writeMicroseconds(1000); // send "s top" signal to ESC. Also necessary to arm the ESC.
}

void loop() {
  Start_state = analogRead(START_PIN);
  Serial.println("Waiting for HIGH pulse.");

  if (Start_state >= 700) { //begin the RampUp
  Serial. println(Start_state);
  delay(3000);
    
    if(RampUp_number < Number_of_RampUps) {
    Serial.println("RampUp beging. . .");

    for (int PWM_value = Starting_PWM_value; PWM_value <= Finishing_PWM_value; PWM_value = PWM_value + Ramp_jump) {
      SkyWalker_ESC.writeMicroseconds(PWM_value);
      Serial.println(PWM_value);
      delay(Ramp_timer);
    }
    RampUp_number = RampUp_number + 1;
    SkyWalker_ESC.writeMicroseconds(Starting_PWM_value);
    }
  }


}