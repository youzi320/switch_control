#include <Servo.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Arduino.h>

Servo myservo;  

void setup() {
  Serial.begin(115200);
  myservo.attach(18);  // 伺服馬達連接的PIN

}

void loop(){
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Serial.println("Command received: " + command);

    int deg = command.toInt();
    Serial.println("deg: " + deg);

    myservo.att20ach(18);
    myservo.write(deg);
    delay(2500);
    myservo.detach();
  }
}