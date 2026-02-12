<!--Author: Martin Chlebovec alias: martinius96-->
<!--Donate: https://www.paypal.me/Chlebovec-->
<!--Personal website: https://arduino.php5.sk-->
<!--Email contact: martinius96@gmail.com-->
<!--Facebook contact: 100001242570317-->
<!--Use under LICENSE-->
!DOCTYPE html>
<html>
  <head>
    <title>Voice control NodeMCU - Webkit>
    <meta name="description" content="Voice control for development board NodeMCU. System use Webkit technology for speech-to-text recognition. Method is useful in each world language.">
    <meta name="keywords" content="nodemcu, board, dev, v3, lolin, webkit, speech, recognition, speech-to-text, translation, en-US">
    <meta charset="utf-8">
    <meta property='fb:admins' content='100001242570317'>
    <meta name='rating' content='General'>
    <meta name="country" content="Slovakia">
    <meta name='category' content='Tech'>
    <link rel="shortcut icon" type="image/png" href="https://www.codeproject.com/script/Membership/Images/octicons_github.png">
    <link rel="stylesheet" type="text/css" href="stylesheet.css">
</head>
    <body> 
    <div class="login">
      <?php if(isset($_POST['submit'])){   //submit of form after click on button, only if you have wrote something to input!
       $translation = $_POST['translation'];
    $translation = htmlspecialchars($translation); //dont run dangerous JS codes
     $translation = trim( $translation ); //delete spaces behind last character
     if($translation == ""){
    echo "I cannot write empty field!"; //dont write empty field to .txt file
  }elseif(strlen($translation) > 50){
    echo "Your string is too long!"; //dont write too long field to .txt file
  }else{
   

    
           if ($translation=="turn on" || $translation=="Turn on"){
      file_put_contents("translations.txt", 'Turn on');
           }else if($translation=="turn off" || $translation=="Turn off"){
               file_put_contents("translations.txt", 'Turn off');
     }
     }
     }
?>
    <form id="labnol" method="post" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']); ?>"><!--Form have submit action on self-->
    	<input type="text" name="translation" id="transcript" placeholder="For text input click there." />
      <img onclick="startDictation();" src="decibel.png" alt="For voice input click here!"><!--Start recording voice after click on Microphone photo-->
<!--Translation if automatically sent via Ajax! No need to aditional clicks etc.-->
         <input type="submit" name="submit" value="Send text input."><!--Click this if you wrote something with text to input-->

         <center><a href="translations.txt"><font color="white">Actual string in .txt file</font></a></center><!--Look to .txt file (try F5 too after click) for remove cache-->

    </form>
</div>
      
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
<script type="text/javascript">
    function startDictation() {

        if (window.hasOwnProperty('webkitSpeechRecognition')) {

            var recognition = new webkitSpeechRecognition();
            var result = '';

            recognition.continuous = false;
            recognition.interimResults = false;
            //change en-US to your national language. Support regional english too (Africa, England, Australia etc.)
            //For more language codes visit: https://msdn.microsoft.com/en-us/library/ms533052(v=vs.85).aspx
            recognition.lang = "en-US";
            recognition.start();

            recognition.onresult = function(e) {
                result = e.results[0][0].transcript;

                $.post(
                    "ajax.php", 
                    { translation: result }
                );

                recognition.stop();
            };

            recognition.onerror = function(e) {
                recognition.stop();
            }

        }
    }
</script>
<!--Author: Martin Chlebovec alias: martinius96-->
<!--Donate: https://www.paypal.me/Chlebovec-->
<!--Personal website: https://arduino.php5.sk-->
<!--Email contact: martinius96@gmail.com-->
<!--Facebook contact: 100001242570317-->
<!--Use under LICENSE-->
    </body>
    </html>
