#include <Servo.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

typedef struct struct_message { // 接收資料的結構
  String command;
} struct_message;

struct_message myData;

Servo myservo;  

// 資料接收回調函式
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  if (len < sizeof(myData)) {
        Serial.println("Data length mismatch");
        return;
  }
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.println();
  if(myData.command.length() > 2 && myData.command[3] == '1'){
    myservo.attach(18);
    myservo.write(98);
    delay(2500);
    myservo.detach();
  }else if(myData.command.length() > 2 && myData.command[3] == '0'){
    myservo.attach(18);
    myservo.write(30);
    delay(2500);
    myservo.detach();
  }
}

void setup() {
  Serial.begin(115200);
  
  myservo.attach(18);  // 伺服馬達連接的PIN
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
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
  
}