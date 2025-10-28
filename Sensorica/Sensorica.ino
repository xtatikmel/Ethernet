/*--------------------------------------------------------------
  Program:      Sensorica

  Description:  Servidor web Arduino que sirve una página web básica.
                La página web se almacena en la tarjeta SD.
  
  Hardware:     Arduino Uno y escudo Ethernet oficial Arduino. 
                Debería funcionar con otros Arduinos y escudos 
                Ethernet compatibles. Tarjeta micro SD de 8Gb 
                formateada en FAT16
                
  Software:     Developed using Arduino 1.0.3 software
                Should be compatible with Arduino 1.0 +
                SD card contains web page called index.htm
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - SD card examples by David A. Mellis and
                  Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet
                - SD Card library documentation:
                  http://arduino.cc/en/Reference/SD

  Date:         24 septiembre 2024
 
  Author:       Jeferson Guevara
--------------------------------------------------------------*/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80

File webFile;
void setup()
{
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
    Serial.begin(9600);       // for debugging
    
    // inicializando SD card
    Serial.println("Inicializacion targeta SD...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - Fallo Inicializacion targeta SD!");
        return;    // init failed
    }
    Serial.println("ÉXITO - Targeta SD inicializada.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - ¡No puedo encontrar el archivo index.htm!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Archivo index.htm encontrado.");
}

void loop()
{
    EthernetClient client = server.available();  // intenta conseguir cliente

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    webFile = SD.open("index.htm");        // open web page file
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read()); // send web page to client
                        }
                        webFile.close();
                    }
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)

}