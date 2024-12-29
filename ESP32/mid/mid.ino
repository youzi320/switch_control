#include <Servo.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

typedef struct struct_message { // 接收資料的結構
  String command;
} struct_message;

struct_message myData_recv, myData_send;

Servo myservo;  
uint8_t broadcastAddress_r1[] = {0x40, 0x22, 0xD8, 0x74, 0xBF, 0xC4};  // r1 mac
uint8_t broadcastAddress_r2[] = {0xCC, 0xDB, 0xA7, 0x46, 0xF7, 0x20};  // r2 mac
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

// 資料接收回調函式
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  if (len < sizeof(myData_recv)) {
        Serial.println("Data length mismatch");
        return;
  }
  memcpy(&myData_recv, incomingData, sizeof(myData_recv));

  if(myData_recv.command.length() > 2 && myData_recv.command[2] == '2'){
    myData_send.command = myData_recv.command;
    Serial.println("send: " + myData_send.command);

    // 設定 peerInfo
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, broadcastAddress_r2, 7);
    peerInfo.channel = 6;
    peerInfo.encrypt = false;

    // 檢查裝置是否配對成功
    if (esp_now_add_peer(&peerInfo) != ESP_OK && esp_now_add_peer(&peerInfo) != ESP_ERR_ESPNOW_EXIST) {
      Serial.println("Failed to add peer");
      return;
    }
    esp_now_add_peer(&peerInfo);
    esp_err_t result = esp_now_send(broadcastAddress_r2, (uint8_t *) &myData_send, sizeof(myData_send));
    
    // 檢查數據是否發送成功
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Ready"); // 確保 ESP32 準備就緒
  pinMode (led_pin , OUTPUT);

  // 初始化 ESP-NOW
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
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
 
}