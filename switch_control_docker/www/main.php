<?php
  session_start();
  if (!isset($_SESSION['auth_token']) || !isset($_COOKIE['auth_token']) || $_SESSION['auth_token'] !== $_COOKIE['auth_token']) {
      header('Location: login.html');
      exit();
  }
?>

<!DOCTYPE html>
<html lang="zh-TW">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>控制開關</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Noto+Sans+TC:wght@500&display=swap');

    body {
      font-family: 'Noto Sans TC', sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f4f4f4;
    }

    #sitebody {
      width: 650px;
      margin: 0 auto;
      text-align: center;
    }

    #title {
      margin-top: 100px;
      font-size: 70px;
      font-weight: bold;
    }

    #switch-container {
      margin: 50px auto;
      padding: 20px;
      background-color: #77c7fc;
      border: 2px solid black;
      border-radius: 8px;
      width: 450px;
      text-align: center;
    }

    .switch-item {
      margin: 20px 0;
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 20px;
      
    }

    .switch-item label {
      margin-right: 20px;
      font-size: 25px;
      
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
    }


    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      transition: 0.4s;
      border-radius: 34px;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      transition: 0.4s;
      border-radius: 50%;
    }

    input:checked + .slider {
      background-color: #4caf50;
    }

    input:checked + .slider:before {
      transform: translateX(26px);
    }
  </style>
</head>

<body>
    <div id="sitebody">
    
        <div id="title">控制開關</div>

        <div id="switch-container">

          <div class="switch-item">
              <label for="study-switch">書房</label>
              <label class="switch">
                <input type="checkbox" id="study-switch" onchange="handleCheckboxChange(this)">
                <span class="slider"></span>
              </label>
          </div>

          <div class="switch-item">
              <label for="bathroom-switch">廁所</label>
              <label class="switch">
                <input type="checkbox" id="bathroom-switch" onchange="handleCheckboxChange(this)">
                <span class="slider"></span>
              </label>

          </div>

        </div>

    </div>

    <script>
        function handleCheckboxChange(checkbox) {
            var command;
            if(checkbox.id == "study-switch"){
                command = "r_1";
            }else if(checkbox.id == "bathroom-switch"){
                command = "r_2";
            }
            if (checkbox.checked) {
                fetch('control.php', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                    body: 'command=' + command + '1'
                })
            } else {
                fetch('control.php', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                    body: 'command=' + command + '0'
                })
            }
        }
    </script>
</body>

</html>
