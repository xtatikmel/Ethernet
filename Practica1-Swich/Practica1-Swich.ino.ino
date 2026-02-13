/*--------------------------------------------------------------
  Program:      eth_websrv_switch

  Description:  El servidor web Arduino muestra el estado de un interruptor en una página web. 
                No utiliza la tarjeta SD.
                El estado del interruptor se muestra en la página web y se actualiza cada segundo. 
                El programa fue desarrollado y probado usando un Arduino Uno y un escudo Ethernet oficial de Arduino. 
                Debería funcionar con otros Arduinos y escudos Ethernet compatibles.
  
  Hardware:     Arduino Uno y shield Ethernet oficial de Arduino. 
                Debería funcionar con otros Arduinos y shields Ethernet compatibles. 
                
  Software:     Desarrollado con Arduino 1.0.3.
                Debería ser compatible con Arduino 1.0 y versiones posteriores.
  
  References:   - Ejemplo de servidor web de David A. Mellis y 
                  modificado por Tom Igoe
                  Modificado por Gargamel
                - Documentación de la biblioteca Ethernet:
                  http://arduino.cc/en/Reference/Ethernet

  Date:         12 January 2026
 
  Author:       W.A. Smith, http://startingelectronics.org
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>

// Dirección MAC de la etiqueta de protección Ethernet debajo de la placa
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177); // Dirección IP, puede necesitar cambiarse según la red
EthernetServer server(80);  // crear un servidor en el puerto 80

void setup()
{
    Ethernet.begin(mac, ip);    // inicializar dispositivo Ethernet
    server.begin();             // empezar a escuchar a los clientes
    pinMode(3, INPUT);  // pin de entrada para el interruptor
}

void loop()
{
    EthernetClient client = server.available();  // intentar obtener cliente

    if (client) {  // cliente obtenido?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // datos del cliente disponibles para leer
                char c = client.read(); // leer 1 byte (carácter) del cliente
                // la última línea de la solicitud del cliente está en blanco y termina con \n
                // responder al cliente solo después de recibir la última línea
                if (c == '\n' && currentLineIsBlank) {
                    // enviar un encabezado de respuesta HTTP estándar
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connnection: close");
                    client.println();
                    // enviar página web
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Estado del interruptor de lectura de Arduino</title>");
                    client.println("<meta http-equiv=\"refresh\" content=\"1\">");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>Interruptor</h1>");
                    client.println("<p>El estado del interruptor es:</p>");
                    GetSwitchState(client);
                    client.println("</body>");
                    client.println("</html>");
                    break;
                }
                // Cada línea de texto recibida del cliente termina con \r\n
                if (c == '\n') {
                    // último carácter en la línea de texto recibida
                    // comenzando una nueva línea con el siguiente carácter leído
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // se recibió un carácter de texto del cliente
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // dar tiempo al navegador web para recibir los datos
        client.stop(); // cerrar la conexión
    } // end if (client)
}

void GetSwitchState(EthernetClient cl)
{
    if (digitalRead(3)) {
        cl.println("<p>ON</p>");
    }
    else {
        cl.println("<p>OFF</p>");
    }
}
