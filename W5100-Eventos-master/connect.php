           

<?php
function Connection(){
$servername = "localhost";
$username = "idYour_Name";
$password = "YourPassword";
$database = "Your_DataBase";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

return $conn;

}
?>