#include <SPI.h>
#include <Ethernet.h>

#define sensibilidade2 A5


int sensorSensibilidade2;

int flagRain=0;
int count=0;
float media=0;
int soma=0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192,168,x,x);         //Inserir o endereço IP

EthernetClient client;
char server[] = "XXXX.000webhostapp.com";

void setup ()
{
  Serial.begin(9600);
  // attempt a DHCP connection:
  Serial.println("Attempting to get an IP address using DHCP:");
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
    Serial.println("failed to get an IP address using DHCP, trying manually");
    Ethernet.begin(mac, ip);
  }
  
 Serial.print("My address:");
 Serial.println(Ethernet.localIP());
}
   
void loop ()
{
  
sensorSensibilidade2 = analogRead(sensibilidade2);
soma = soma + sensorSensibilidade2;
Serial.println(sensorSensibilidade2);

if (count>9)
{
media = soma/10;
chuva();
Serial.print("media:");
Serial.println(media);
soma=0;
count=0;  
  }
count++;
delay(100);
}

void envia_MSG(){
if (client.connect(server, 80)) {

        // print to serial monitor
        Serial.println("connected...");
        Serial.println("ARDUINO: forming HTTP request message");

          if (flagRain==1)
          {
            client.println("GET /add.php?MSG_Texto1=Sensor%20Seco_Arduino HTTP/1.1");
            client.println("Host: XXXX.000webhostapp.com");            
            //client.println(server);
            client.println("Connection: close");
            client.println();
            Serial.println("ARDUINO: HTTP message sent");
            delay(1000);
            
             }

           if (flagRain==0)         
           {
      
            client.println("GET /add.php?MSG_Texto1=Inicio%20Chuva_Arduino HTTP/1.1");
            client.println("Host: XXXX.000webhostapp.com");
            //client.println(server);
            client.println("Connection: close");
            client.println();
            Serial.println("ARDUINO: HTTP message sent");        
           delay(1000);
           
         }
    
if(client.available())
        {
            while(client.available())
            {
                char c = client.read();
                Serial.print(c);
            }
        }
        else
        {
            Serial.println("ARDUINO: no response received / no response received in time");
        }
 
        client.stop();
    }
    // do nothing forever after:
   // while(true);
}



void chuva()

{
  
        if (flagRain==0)
           {
                    if(media<960)
           {
            envia_MSG();
         flagRain=1;
          }                     
          }
           
           if (flagRain==1)           
           {
                    if(media>1005) 
           {
            envia_MSG();
            flagRain=0;
          }
          }
}  




