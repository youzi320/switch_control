<?php
session_start();
$connect = mysqli_connect(
    'db',
    'admin_lamp',
    'admin',
    'lamp_docker'
 );

if ($connect->connect_error) {
    die("Connection failed: " . $connect->connect_error);
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $username = $connect->real_escape_string($_POST['username']); //防止基本SQL injection
    $password = $connect->real_escape_string($_POST['password']); //防止基本SQL injection

    $query = "SELECT * FROM user WHERE username = '$username' AND password = '$password'";
    $result = mysqli_query($connect, $query);

    if ($result && $result->num_rows > 0) {
        $token = bin2hex(random_bytes(16));
        setcookie("auth_token", $token, time() + 86400); // Cookie 1 天後過期
        $_SESSION['auth_token'] = $token;
        header('Location: main.php'); 
        exit(); 
    } else {
        header('Location: login.html');
    }
}
?>
