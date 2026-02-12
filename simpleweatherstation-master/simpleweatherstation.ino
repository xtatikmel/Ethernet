

/*
   Telnet server and data client (opendata.fsftn.org). 
   
   Created:     Vassilis Serasidis
   Date:        30 Jul 2014
   Version:     1.01
   Home:        http://www.serasidis.gr
   email:       avrsite@yahoo.gr , info@serasidis.gr
   
   Tested with Arduino IDE 1.5.7 and Ethernet v1.57
  
   Modified by crakensio for Data logging  
   Date: 4 APR 2016
   Version 2.0
   home: http://crakensio.wordpress.com
   email: crazysarath8055@gmail.com
  
  */
  
#include <stdlib.h>
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
DHT dht; //Object creation

char PhantDataServer[] = "data.sparkfun.com";
#define PUBLIC_KEY  "xxxxxxxxxxxxxxxx" //Your phant public_key
#define PRIVATE_KEY "xxxxxxxxxxxxxxxx" //Your phant private_key

#define PHANT_UPDATE_TIME 60000        //Update SparkFun data server every 60000 ms (1 minute).

#define TIMEOUT 1000 //1 second timout

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xAE, 0xCD };
IPAddress ip(192, 168, 1, 22); //Your local IP if DHCP fails.
IPAddress dnsServerIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Initialize the Ethernet client.
EthernetClient client;

unsigned long timer1 = 0;
unsigned long timer2 = 0;

int failedResponse = 0;
float humidity1;
float temperature1;
String shumidity;
String stemperature;
char buff1[7]; //buffer for conversion
char buff2[7]; //buffer for conversion

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void setup()
{

  //Initiallize the serial port.
  Serial.begin(9600);
  Serial.println("-= phant data client =-\n");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0)
  {
  //  Serial.println("Failed to configure Ethernet using DHCP");
    // DHCP failed, so use a fixed IP address:
    Ethernet.begin(mac, ip, dnsServerIP, gateway, subnet);
  }
  Serial.print("LocalIP:\t\t");
  Serial.println(Ethernet.localIP());
  Serial.print("SubnetMask:\t\t");
  Serial.println(Ethernet.subnetMask());
  Serial.print("GatewayIP:\t\t");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP:\t\t");
  Serial.println(Ethernet.dnsServerIP());
  dht.setup(5);
  
}
  
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void loop()
{
  
  delay(dht.getMinimumSamplingPeriod());
  humidity1 = dht.getHumidity();
  temperature1 = dht.getTemperature();
  String shumidity=dtostrf(humidity1,3,2,buff1); //converting float to string
  String stemperature=dtostrf(temperature1,3,2,buff2); //converting float to string
   //Serial.println(shumidity); //Just for testing purpose
   //Serial.println(stemperature);
  //Serial.println("temp and humi got");
  //Update sparkfun data server every 60 seconds.
  if(millis() > timer1 + PHANT_UPDATE_TIME)
  {
      timer1 = millis(); //Update timer1 with the current time in miliseconds.
      sendToPhantDataServer(shumidity, stemperature); //Send data to sparkfun data server.
      //Serial.println("main ends"); //added only for testing.

  }
}

void sendToPhantDataServer(String humidity,String temperature)
{ 
  Serial.println("send to started");
    //Establish a TCP connection to sparkfun server.
    if (client.connect(PhantDataServer, 80))
    {
        //if the client is connected to the server...
        if(client.connected())
        {
            Serial.println("Sending data to SparkFun...\n");
            // send the HTTP PUT request:
            
            client.print("GET /input/");
            client.print(PUBLIC_KEY);
            client.print("?private_key=");
            client.print(PRIVATE_KEY);
            client.print("&humidity=");
            client.print(humidity);    //send the number stored in 'humidity' string. Select only one.
            client.print("&temperature=");   // I don't use it. I add it as an example.
            client.println(temperature);       
           
  
            delay(1000); //Give some time to Sparkfun server to send the response to ENC28J60 ethernet module.
            timer2 = millis();
            while((client.available() == 0)&&(millis() < timer2 + TIMEOUT)); //Wait here until server respond or timer2 expires. 
            
            // if there are incoming bytes available
            // from the server, read them and print them:
            while(client.available() > 0)
            {
                char inData = client.read();
                Serial.print(inData);
            }      
            Serial.println("\n");   
            client.stop(); //Disconnect the client from server.  
         }
     } 
}

