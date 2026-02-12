#include <DHT.h>


#define gap1 1.5 // Overlap setting
#define gap2 1.5 // Overlap setting

int hset1 = 70; // Heat setting
int cset1 = 80; // Cool setting
int hset2 = 60; // Heat setting
int cset2 = 80; // Cool setting
String mode1 = "S";
String mode2 = "S";

#define DHT1PIN 2     // what pin we're connected to
//#define DHT1TYPE DHT11   // DHT 11
#define DHT1TYPE DHT22   // DHT 22  (AM2302)
//#define DHT1TYPE DHT21   // DHT 21 (AM2301)

#define DHT2PIN 5     // what pin we're connected to
#define DHT2TYPE DHT11   // DHT 11
//#define DHT2TYPE DHT22   // DHT 22  (AM2302)
//#define DHT2TYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 240 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.240")
byte gateway[] = { 192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

String readString;

int led1 = 22;
int led2 = 23;
int led3 = 24;
int led4 = 25;
int led5 = 26;
int led6 = 27;
int led7 = 28;
int led8 = 29;
int led9 = 30;
int led10 = 31;
int led11 = 32;
int led12 = 33;
int led13 = 34;
int led14 = 35;
int led15 = 36;
int led16 = 37;


void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
  pinMode(led11, OUTPUT);
  pinMode(led12, OUTPUT);
  pinMode(led13, OUTPUT);
  pinMode(led14, OUTPUT);
  pinMode(led15, OUTPUT);
  pinMode(led16, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  digitalWrite(led6, HIGH);
  digitalWrite(led7, HIGH);
  digitalWrite(led8, HIGH);
  digitalWrite(led9, HIGH);
  digitalWrite(led10, HIGH);
  digitalWrite(led11, HIGH);
  digitalWrite(led12, HIGH);
  digitalWrite(led13, HIGH);
  digitalWrite(led14, HIGH);
  digitalWrite(led15, HIGH);
  digitalWrite(led16, HIGH);


// Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  dht1.begin();
  dht2.begin();

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature(true);
  float f1 = dht1.readTemperature(true);
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t1) || isnan(h1)) {
    Serial.println("Failed to read from DHT 1");
  } else {
    Serial.print("Humidity1: ");
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperature1: ");
    Serial.print(t1);
    Serial.println(" *F");
  }

  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature(true);
  float f2 = dht2.readTemperature(true);
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t2) || isnan(h2)) {
    Serial.println("Failed to read from DHT 2");
  } else {
    Serial.print("Humidity2: ");
    Serial.print(h2);
    Serial.print(" %\t");
    Serial.print("Temperature2: ");
    Serial.print(t2);
    Serial.println(" *F");
  }

  if ((hset1)>(cset1)) {
    int hset1 = 70;
    int cset1 = 80;
  }

  if ((hset1<(f1-gap1))&&(f1<(cset1-gap1))) {
    mode1.replace("H", "S");
    mode1.replace("C", "S");
    digitalWrite(led1,HIGH); // kill all outputs
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
  }
  if (hset1>=(f1+gap1)) {
    mode1.replace("S", "H");
    mode1.replace("C", "H");
    digitalWrite(led2,HIGH); // kill all cooling output
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
    digitalWrite(led1,LOW); // activate heat output
  }
  if (f1>(cset1+gap1)) {
    mode1.replace("S", "C");
    mode1.replace("H", "C");
    digitalWrite(led1,HIGH); // kill heat output
    digitalWrite(led3,LOW); // activate cooling 1 output
    delay(1000);
    digitalWrite(led4,LOW); // activate cooling 2 output
    delay(1000);
    digitalWrite(led2,LOW); // activate fan output

   }

  if ((hset2)>(cset2)) {
    int hset2 = 60;
    int cset2 = 80;
  }

  if ((hset2<(f2-gap2))&&(f2<(cset2-gap2))) {
    mode2.replace("H", "S");
    mode2.replace("C", "S");
    digitalWrite(led6,HIGH); // kill all outputs
    digitalWrite(led7,HIGH);
  }
  if (hset2>=(f2+gap2)) {
    mode2.replace("S", "H");
    mode2.replace("C", "H");
    digitalWrite(led7,HIGH); // kill cool output
    digitalWrite(led6,LOW); // activate heat output
  }
  if (f2>(cset2+gap2)) {
    mode2.replace("S", "C");
    mode2.replace("H", "C");
    digitalWrite(led6,HIGH); // kill heat output
    digitalWrite(led7,LOW); // activate cooling output
  }

 delay(3000);
 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
        }
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 10");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
        
          // output the value of DHT 1
          client.println("<H2>");
          client.println("HOUSE:");
          client.println("<br />");  
          client.print("Humidity: ");
          client.println("</H2>");
          client.println("<p />");
          client.println("<H1>");
          client.print(h1);
          client.print(" %\t");
          client.println("</H1>");
          client.println("<p />"); 
          client.println("<H2>");
          client.print("Temperature: ");
          client.println("</H2>");
          client.println("<H1>");
          client.print(t1);
          client.println(" &#176;");
          client.println("F");
          client.println("</H1>");
          client.println("<br />");  

          // output the value of DHT 2
          client.println("<H2>");
          client.println("THEATER:");
          client.println("<br />");  
          client.print("Humidity: ");
          client.println("</H2>");
          client.println("<p />");
          client.println("<H1>");
          client.print(h2);
          client.print(" %\t");
          client.println("</H1>");
          client.println("<p />"); 
          client.println("<H2>");
          client.print("Temperature: ");
          client.println("</H2>");
          client.println("<H1>");
          client.print(t2);
          client.println(" &#176;");
          client.println("F");
          client.println("</H1>");
          client.println("<br />");  

          // print menu
          client.println("Zone 1 Control");     
          client.println("<br />");     
          client.println("<a href=\"/?heatpos1\"\">Turn UP Heat</a>");
          client.println("<a href=\"/?heatneg1\"\">Turn DOWN Heat</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?coolpos1\"\">Turn UP Cool</a>");
          client.println("<a href=\"/?coolneg1\"\">Turn DOWN Cool</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("Zone 2 Control");     
          client.println("<br />");     
          client.println("<a href=\"/?heatpos2\"\">Turn UP Heat</a>");
          client.println("<a href=\"/?heatneg2\"\">Turn DOWN Heat</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?coolpos2\"\">Turn UP Cool</a>");
          client.println("<a href=\"/?coolneg2\"\">Turn DOWN Cool</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("Manual Control");     
          client.println("<br />");     
          client.println("Zone 1 Control");     
          client.println("<br />");     
          client.println("<a href=\"/?button1on\"\">Turn ON HEAT</a>");
          client.println("<a href=\"/?button1off\"\">Turn OFF HEAT</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button2on\"\">Turn ON Fan</a>");
          client.println("<a href=\"/?button2off\"\">Turn OFF Fan</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button3on\"\">Turn ON A/C 1</a>");
          client.println("<a href=\"/?button3off\"\">Turn OFF A/C 1</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button4on\"\">Turn ON A/C 2</a>");
          client.println("<a href=\"/?button4off\"\">Turn OFF A/C 2</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button5off\"\">Enable System</a>");
          client.println("<a href=\"/?button5on\"\">Disable System</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("Zone 2 Control");     
          client.println("<br />");     
          client.println("<a href=\"/?button6on\"\">Turn ON HEAT</a>");
          client.println("<a href=\"/?button6off\"\">Turn OFF HEAT</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button7on\"\">Turn On COOL</a>");
          client.println("<a href=\"/?button7off\"\">Turn Off COOL</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button8off\"\">Enable System</a>");
          client.println("<a href=\"/?button8on\"\">Disable System</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button9on\"\">Turn On EXT1</a>");
          client.println("<a href=\"/?button9off\"\">Turn Off EXT1</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button10on\"\">Turn On EXT2</a>");
          client.println("<a href=\"/?button10off\"\">Turn Off EXT2</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button11on\"\">Turn On EXT3</a>");
          client.println("<a href=\"/?button11off\"\">Turn Off EXT3</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button12on\"\">Turn On EXT4</a>");
          client.println("<a href=\"/?button12off\"\">Turn Off EXT4</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button13on\"\">Turn On EXT5</a>");
          client.println("<a href=\"/?button13off\"\">Turn Off EXT5</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button14on\"\">Turn On EXT6</a>");
          client.println("<a href=\"/?button14off\"\">Turn Off EXT6</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button15on\"\">Turn On EXT7</a>");
          client.println("<a href=\"/?button15off\"\">Turn Off EXT7</a><br />");   
          client.println("<br />");     
          client.println("<br />");  
          client.println("<a href=\"/?button16on\"\">Turn On EXT8</a>");
          client.println("<a href=\"/?button16off\"\">Turn Off EXT8</a><br />");   
          client.println("<br />");     
          client.println("<p>Copyright: Jacob D Johnson 11/4/2018</p>");  
          client.println("<br />"); 

          // Variables
          client.print("<temp1>"); 
          client.print(t1); 
          client.print("</temp1>"); 
          client.println("<br />"); 
          client.print("<humid1>"); 
          client.print(h1); 
          client.print("</humid1>"); 
          client.println("<br />"); 
          client.print("<HeatVar1>"); 
          client.print(hset1); 
          client.print("</HeatVar1>"); 
          client.println("<br />"); 
          client.print("<CoolVar1>"); 
          client.print(cset1); 
          client.print("</CoolVar1>"); 
          client.println("<br />"); 
          client.print("<mode1>"); 
          client.print(mode1); 
          client.print("</mode1>"); 
          client.println("<br />"); 
          
          client.print("<temp2>"); 
          client.print(t2); 
          client.print("</temp2>"); 
          client.println("<br />"); 
          client.print("<humid2>"); 
          client.print(h2); 
          client.print("</humid2>"); 
          client.println("<br />"); 
          client.print("<HeatVar2>"); 
          client.print(hset2); 
          client.print("</HeatVar2>"); 
          client.println("<br />"); 
          client.print("<CoolVar2>"); 
          client.print(cset2); 
          client.print("</CoolVar2>"); 
          client.println("<br />"); 
          client.print("<mode2>"); 
          client.print(mode2); 
          client.print("</mode2>"); 
          client.println("<br />"); 
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
               //controls the Arduino if you press the buttons
           if (readString.indexOf("?heatpos1") >0){
               (hset1 += 1);
           }
           if (readString.indexOf("?heatneg1") >0){
               (hset1 -= 1);
           }
           if (readString.indexOf("?coolpos1") >0){
               (cset1 += 1);
           }
           if (readString.indexOf("?coolneg1") >0){
               (cset1 -= 1);
           }
           if (readString.indexOf("?heatpos2") >0){
               (hset2 += 1);
           }
           if (readString.indexOf("?heatneg2") >0){
               (hset2 -= 1);
           }
           if (readString.indexOf("?coolpos2") >0){
               (cset2 += 1);
           }
           if (readString.indexOf("?coolneg2") >0){
               (cset2 -= 1);
           }
           if (readString.indexOf("?button1on") >0){
               digitalWrite(led1, LOW);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(led1, HIGH);
           }
           if (readString.indexOf("?button2on") >0){
               digitalWrite(led2, LOW);
           }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(led2, HIGH);
           }
           if (readString.indexOf("?button3on") >0){
               digitalWrite(led3, LOW);
           }
           if (readString.indexOf("?button3off") >0){
               digitalWrite(led3, HIGH);
           }
           if (readString.indexOf("?button4on") >0){
               digitalWrite(led4, LOW);
           }
           if (readString.indexOf("?button4off") >0){
               digitalWrite(led4, HIGH);
           }
           if (readString.indexOf("?button5on") >0){
               digitalWrite(led5, LOW);
           }
           if (readString.indexOf("?button5off") >0){
               digitalWrite(led5, HIGH);
           }
           if (readString.indexOf("?button6on") >0){
               digitalWrite(led6, LOW);
           }
           if (readString.indexOf("?button6off") >0){
               digitalWrite(led6, HIGH);
           }
           if (readString.indexOf("?button7on") >0){
               digitalWrite(led7, LOW);
           }
           if (readString.indexOf("?button7off") >0){
               digitalWrite(led7, HIGH);
           }
           if (readString.indexOf("?button8on") >0){
               digitalWrite(led8, LOW);
           }
           if (readString.indexOf("?button8off") >0){
               digitalWrite(led8, HIGH);
           }
           if (readString.indexOf("?button9on") >0){
               digitalWrite(led9, LOW);
           }
           if (readString.indexOf("?button9off") >0){
               digitalWrite(led9, HIGH);
           }
           if (readString.indexOf("?button10on") >0){
               digitalWrite(led10, LOW);
           }
           if (readString.indexOf("?button10off") >0){
               digitalWrite(led10, HIGH);
           }
           if (readString.indexOf("?button11on") >0){
               digitalWrite(led11, LOW);
           }
           if (readString.indexOf("?button11off") >0){
               digitalWrite(led11, HIGH);
           }
           if (readString.indexOf("?button12on") >0){
               digitalWrite(led12, LOW);
           }
           if (readString.indexOf("?button12off") >0){
               digitalWrite(led12, HIGH);
           }
           if (readString.indexOf("?button13on") >0){
               digitalWrite(led13, LOW);
           }
           if (readString.indexOf("?button13off") >0){
               digitalWrite(led13, HIGH);
           }
           if (readString.indexOf("?button14on") >0){
               digitalWrite(led14, LOW);
           }
           if (readString.indexOf("?button14off") >0){
               digitalWrite(led14, HIGH);
           }
           if (readString.indexOf("?button15on") >0){
               digitalWrite(led15, LOW);
           }
           if (readString.indexOf("?button15off") >0){
               digitalWrite(led15, HIGH);
           }
           if (readString.indexOf("?button16on") >0){
               digitalWrite(led16, LOW);
           }
           if (readString.indexOf("?button16off") >0){
               digitalWrite(led16, HIGH);
           }
            //clearing string for next read
            readString="";  

  }
}
