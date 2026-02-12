<?php
    include("connect.php");
    
    $link=Connection();

 $MSG_Texto=$_GET["Evento1"];
 //utilizado abaixo para correção do fusohorario
 date_default_timezone_set('America/Sao_Paulo');
 $date = date("Y-m-d G:i:s");

 $query = "INSERT INTO `id-YourBd_bd`.`SMS_Recebidos` (`timeStamp`,`SMS_Texto`) 
  VALUES ('$date','".$MSG_Texto."')"; 
   
    mysqli_query($link, $query);
 mysqli_close($link);
     
    header("Location: index.php");
?>