<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $command = $_POST['command'];
    if ($command) {
        $port = fopen("/dev/ttyUSB0", "w+"); 
        if (!$port) {
            die("Failed to open serial port.");
        }
        
        fwrite($port, $command); 
        fclose($port);
    }
}
?>
