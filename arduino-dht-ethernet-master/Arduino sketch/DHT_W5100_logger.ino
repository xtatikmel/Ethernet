#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>
#include <stdlib.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "mysite.com";
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,11);
EthernetClient client;
dht DHT;
#define DHT11_PIN 2

void setup() {
  Serial.begin(9600);
}

void loop()
{

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  if (client.connect(server, 80)) {
    
    DHT.read11(DHT11_PIN);
    char buffr[20];
    
    String PostData="macaddr=testaddr";
    PostData=PostData+"&temperature=";
    PostData=PostData+dtostrf(DHT.temperature,5,1,buffr);
    PostData=PostData+"&humidity=";
    PostData=PostData+dtostrf(DHT.humidity,5,1,buffr);

    client.println("POST /arduino/index.php HTTP/1.1");
    client.println("Host:  mysite.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    
    // wait
    Serial.println("Will wait");
    

  } 
  else {
    Serial.println("Client not connected - could not send data.");
  }
  
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  delay(5000);
  client.flush();
  client.stop();
  delay(1800000); // 30 min = 1800000 ms
    
  // if the server's disconnected, stop the client:
  //if (!client.connected()) {
  //  client.stop();
  //  // do nothing forevermore:
  //  while(true);
  //}
}
