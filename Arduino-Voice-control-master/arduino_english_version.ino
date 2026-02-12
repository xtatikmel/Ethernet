// Author: Martin Chlebovec alias: martinius96
// Support: https://www.paypal.me/Chlebovec
// Personal web: https://arduino.php5.sk
// Email contact: martinius96@gmail.com
// Facebook contact: 100001242570317
// Use under license!
// Don't use digital pins 4, 11, 12, 13
#include <SPI.h>
#include <Ethernet.h>
const int led = 5;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //MAC address
char serverName[] = "www.arduino.php5.sk"; // website for example test
IPAddress ip(192, 168, 2, 40); //IP address
EthernetClient client; //client mode
String readString; //our parsing variable
int x=0; //line counter
char lf=10; //line feed
void setup(){
 pinMode(led, OUTPUT);
 if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP unsucessful, trying again.");
    Ethernet.begin(mac, ip); //try with our IP (static)
  }
  Serial.begin(9600); 
}
void loop(){
if (client.connect(serverName, 80)) { //Try HTTP connection
    Serial.println("Connected");
    client.println("GET /PHP_en/translations.txt HTTP/1.1"); //stiahni text zo suboru
    client.println("Host: www.arduino.php5.sk"); //host --> rovnako ako v serverName
    client.println("Connection: close");  //ukonc pripojenie
    client.println(); //koniec requestu
  } 
  else {
    Serial.println("Connection unsucessful."); //Chybova hlaska o neuspesnom pripojeni na webserver
    Serial.println();
  }
  while(client.connected() && !client.available()) delay(1); //little wait...
  while (client.connected() || client.available()) { //if we are connected.. read
    char c = client.read(); //read bytes from buffer
    Serial.print(c); //raw feed from web with header
    if (c==lf) x=(x+1); //count lines
    else if (x==12) readString += c; //build string
   } 
if(readString=="Turn on"){
 digitalWrite(led, HIGH);
 }
 else if(readString=="Turn off"){
 digitalWrite(led, LOW);
 }
 else{
 Serial.println("Unsupported string, change on-line your voice/text input."); //bad string
Serial.println(readString); //write to serial monitor what we got in variable
 }
  readString = (""); //clear String 
  x=0; //clear line counter
  client.stop(); //stop request
  delay(5000); //again run loop after 5 seconds
} 
