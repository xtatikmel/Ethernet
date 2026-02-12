#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 }; // RESERVED MAC ADDRESS
byte php_server[] = {192,168,1,20};
EthernetClient client;
long randNumber;
long previousMillis = 0;
unsigned long currentMillis = 0;
long interval = 5000; // READING INTERVAL

float vol = 0;	
float amp = 0;	
float tkw = 0;
float nkw = 0;
String data;

void setup() { 
  randomSeed(analogRead(0));
  delay(10000);
	Serial.begin(115200);
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP"); 
	}
	data = "";
}

void loop()
{
  randNumber = random(300);
	currentMillis = millis();
	if(currentMillis - previousMillis > interval) 
	{ // READ ONLY ONCE PER INTERVAL
		previousMillis = currentMillis;
		vol = randNumber*220/300;
		amp = randNumber*0.5/300;
    tkw = randNumber*16.50/300;
    nkw = randNumber*0.10/300;
	}

  
  data = "VALUE=";
  data += String(vol);
  data+="','";
  data += String(amp);
  data+="','";
  data += String(tkw);
  data+="','";
  data += String(nkw);
  
  //VALUE=Vol','Amp','Tkw','Nkw
   int rsp;
   rsp=client.connect(php_server ,80);
   Serial.print("RSP:");
   Serial.println(rsp);
	if (rsp) 
	{ 
		client.println("POST /add.php? HTTP/1.1"); 
    //Serial.println("POST /add.php? HTTP/1.1"); 
		client.println("Host:192.168.1.20"); 
		//Serial.println("Host:192.168.1.20"); 
		client.println("Content-Type: application/x-www-form-urlencoded"); 
		//Serial.println("Content-Type: application/x-www-form-urlencoded"); 
		client.print("Content-Length: "); 
		//Serial.println("Content-Length: "); 
		client.println(data.length()); 
		//Serial.println(data.length()); 
		client.println(); 
		//Serial.println(); 

		client.print(data); 
    //Serial.println(data); 
	} 
  else
  {
    Serial.println("Connect Error!");
  }


	if (client.connected())
	{ 
		client.stop();	
	}

	delay(300000); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
}


