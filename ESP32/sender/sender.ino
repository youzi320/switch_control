#include <Arduino.h>
#include <Servo.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

Servo myservo;  

typedef struct struct_message { // 發送資料的結構
  String command;
} struct_message;
struct_message myData;

int led_pin = 2;

// 資料發送回調函式
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  digitalWrite(led_pin , HIGH);
  delay(500);
  digitalWrite(led_pin , LOW);
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}


void setup() {
  Serial.begin(115200); // 設置端口波特率
  Serial.println("ESP32 Ready"); // 確保 ESP32 準備就緒
  pinMode (led_pin , OUTPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  uint8_t primaryChan = 6;
  wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  esp_wifi_set_channel(primaryChan, secondChan);
  Serial.println(WiFi.channel());
  esp_now_register_send_cb(OnDataSent);
  
}

uint8_t broadcastAddress_r1[] = {0x40, 0x22, 0xD8, 0x74, 0xBF, 0xC4};  // r1 mac
uint8_t broadcastAddress_mid[] = {0xCC, 0xDB, 0xA7, 0x47, 0x1E, 0x34};  // mid mac
uint8_t broadcastAddress[6];

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // 接收來自 PHP 的指令
    Serial.println("Command received: " + command);

    // 根據指令執行操作
    if (command[2] == '1') {
      memcpy(broadcastAddress, broadcastAddress_r1, 6 * sizeof(uint8_t));
    } else if (command[2] == '2') {
      memcpy(broadcastAddress, broadcastAddress_mid, 6 * sizeof(uint8_t));
    }

    myData.command = command;
    
    // 設定 peerInfo
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 6;
    peerInfo.encrypt = false;
    // 檢查裝置是否配對成功
    if (esp_now_add_peer(&peerInfo) != ESP_OK && esp_now_add_peer(&peerInfo) != ESP_ERR_ESPNOW_EXIST) {
      Serial.println("Failed to add peer");
      return;
    }
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    // 檢查數據是否發送成功
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    
  }
}