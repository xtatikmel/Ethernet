<?php 
// Author: Martin Chlebovec alias: martinius96
// Donate: https://www.paypal.me/Chlebovec 
// Personal website: https://arduino.php5.sk
// Email contact: martinius96@gmail.com
// Facebook contact: 100001242570317
// Use under LICENSE
   $translation = $_POST['translation'];
    $translation = htmlspecialchars($translation);
     $translation = trim( $translation );
     if($translation == ""){
    echo "I cannot write empty field!";
  }elseif(strlen($translation) > 50){
    echo "Your string is too long!";
  }else{
      if ($translation=="turn on" || $translation=="Turn on"){
      file_put_contents("translations.txt", 'Turn on');
           }else if($translation=="turn off" || $translation=="Turn off"){
               file_put_contents("translations.txt", 'Turn off');
     }
     }
     
?>
