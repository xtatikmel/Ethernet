<?php

 include("connect.php");  
 
 $link=Connection();

$result=mysqli_query($link, "SELECT * FROM `SMS_Recebidos` ORDER BY `timeStamp` DESC");
 
?>

<html>

    <head>
        <title>Monitoramento - Eventos</title>
    </head>
<body>
  <a href='https://www.arduino.cc/' style='display: block'>
  <img alt='Monitoramento' height='200px; ' id='Header1_headerimg' src='https://w.1stopmove.com/uploads/blog_21.png' style='display: block' width='560px; '/>
</a>
   <h1>Monitoramento de Eventos</h1>
   

   <table border="1" cellspacing="0" cellpadding="0">
  <tr>
   <td align='center' width='100px'><b>&nbsp;Id Evento&nbsp;</b></td>  
   <td align='center' width='160px'><b>&nbsp;Data e Hora&nbsp;</b></td>
   <td align='center' width='240px'><b>&nbsp;Mensagem&nbsp;</b></td>
  </tr>

      <?php 
    if($result!==FALSE){
       while($row = mysqli_fetch_array($result)) {
          printf("<tr><td align='center'> &nbsp;%s </td><td> &nbsp;%s </td><td align='center'> &nbsp;  %s&nbsp;   </td></tr>", 
             $row["idEvent"], $row["timeStamp"], $row["SMS_Texto"]);
       }
       mysqli_free_result($result);
       mysqli_close($link);
    }
      ?>

   </table>
</body>
</html>