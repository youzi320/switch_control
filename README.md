# switch_control
 開關控制系統，透過 Docker 部署 LAMP 環境
## 背景探討
### 相關應用(系統)簡介:
#### 在VirtualBox中建立Ubuntu VM，並在 Ubuntu VM中用Docker搭建LAMP環境
 - Ubuntu版本: 24.04.1
 - VirtualBox 版本: 7.0.20
### 相關應用套件簡介:
使用Docker搭建LAMP環境，其中:
- Apache: 建立伺服器，讓使用者可以連到網頁
- MySQL: 存放使用者帳號密碼的資料庫
- PHP: 用來與資料庫建立連線、驗證使用者身分、用USB端口傳送訊號給ESP32
- ngrok: 透過port forwarding，讓不同設備可以連到網頁伺服器
- Arduino IDE: 設定USB端口，測試電腦與ESP32的連線

### ESP32
本專案用到4個ESP32，分別可標記為:
1. sender: 連接電腦，用來接收使用者請求，傳送訊號給其他ESP32
2. receiver_1: 連接伺服馬達，接收到訊號時開啟或關閉電源
3. receiver_2: 連接伺服馬達，接收到訊號時開啟或關閉電源
4. mid: 負責轉發訊號，避免receive_2離sender太遠無法接收到訊號
 
## 建置專案

1. **git clone**
   ```=
   git clone https://github.com/youzi320/switch_control/
   ```
2. **用 /ESP32/find_mac/find_mac.ino ，查看r1(receive_1),r2(receive_2),mid的MAC**
3. **在 /ESP32/sender/sender.ino 與 /ESP32/mid/mid.ino 程式碼中修改r1(receive_1),r2(receive_2),mid的MAC**
4. **準備四個ESP32，將/ESP32中sender,mid,receive_1, receive_2,的程式碼上傳至ESP32中**
5. **將連接sender ESP32的USB插入電腦，並在Virtual Box中勾此USB設備(上方Devices->USB->找到設備)**
6. **將USB設備新增讀寫權限**
   ```=
   sudo chmod 666 /dev/ttyUSB0
   ```
7. **在VM中下載Arduino IDE測試ESP32通訊，並調整Baud，測試完後關掉Arduino IDE**
8. **Docker Compose**
   ```=
   cd switch_control
   docker compose up -d
   ```
9. **打開瀏覽器，預設登入帳號為: admin ，密碼為: admin**
10. **將傳入recive_1, receive_2程式碼的ESP32接上伺服馬達，並安裝在電源旁**
11. **透過ngrok進行端口轉發，即可在行動裝置上控制電源開關**

## 搭建LAMP環境中的檔案說明
### /switch_control_docker/compose.yml
- PHP: 在devices參數中，將VM的USB端口掛載到container，使container可以操作USB連接的設備。
- Apache: 在volumes參數中，將VM本機端存放的網頁掛載到container中。
- MySQL: 在environment參數中，設定資料庫名稱，以及登入資料庫的帳號密碼。在volumes參數中，將已經設定好的表格(使用者帳號密碼)掛載到container，重啟環境時資料不會消失。
- phpMyAdmin: 在environment參數中的PMA_HOST，指向配置好的MySQL資料庫。
### /switch_control_docker/www
- login.html : 登入頁面
- login.php : 查詢資料庫，並設置 Cookie 與 Session
- main.php : 驗證登入，控制開關頁面
- control.php : 透過USB端口傳送訊號給ESP32(sender)
