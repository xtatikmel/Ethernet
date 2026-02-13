/*--------------------------------------------------------------
Programa: eth_websrv_LED

Descripción: Servidor web Arduino que genera una página web que permite al usuario controlar un LED.

Hardware: - Arduino Uno y shield Ethernet oficial de Arduino. Debería funcionar con otros Arduinos y shields Ethernet compatibles.
- LED y resistencia conectados en serie entre el pin 2 de Arduino y GND.

Software: Desarrollado con Arduino 1.0.3. Debería ser compatible con Arduino 1.0 y versiones posteriores.

Referencias: - Ejemplo de servidor web de David A. Mellis y modificado por Tom Igoe.
- Documentación de la biblioteca Ethernet:
http://arduino.cc/en/Reference/Ethernet
  Date:         11 January 2013
 
  Author:       W.A. Smith, http://startingelectronics.org
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>

// Dirección MAC de la etiqueta de protección Ethernet debajo de la placa
byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xF0, 0xED };//Ponemos la dirección MAC de la Ethernet Shield
IPAddress ip(192,168,1,177); //Asignamos  la IP al Arduino
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto
 
String HTTP_req;          // almacena la solicitud HTTP
boolean LED_status = 0;   // Estado del LED, apagado por defecto

void setup()
{
    Ethernet.begin(mac, ip);  // inicializar dispositivo Ethernet
    server.begin();           // empezar a escuchar a los clientes
    Serial.begin(9600);       // para diagnóstico
    pinMode(2, OUTPUT);       // LED en el pin 2
}

void loop()
{
    EthernetClient client = server.available();  // intentar obtener cliente

    if (client) {  // cliente obtenido?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // datos del cliente disponibles para leer
                char c = client.read(); // leer 1 byte (carácter) del cliente
                HTTP_req += c;  // guardar la solicitud HTTP 1 carácter a la vez
                // la última línea de la solicitud del cliente está en blanco y termina con \n
                // responder al cliente solo después de recibir la última línea
                if (c == '\n' && currentLineIsBlank) {
                    // enviar un encabezado de respuesta HTTP estándar
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // enviar página web
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Arduino LED Control</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>LED</h1>");
                    client.println("<p>Haga clic para encender y apagar el LED.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox(client);
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    Serial.print(HTTP_req);
                    HTTP_req = "";    // Terminado con la solicitud, cadena vacía para la próxima solicitud
                    break;
                }
                // cada línea de texto recibida del cliente termina con \r\n
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

// Cambiar LED y enviar HTML para la casilla de verificación LED
void ProcessCheckbox(EthernetClient cl)
{
    if (HTTP_req.indexOf("LED2=2") > -1) {  // ver si se hizo clic en la casilla de verificación
        // se hizo clic en la casilla de verificación, cambiar el estado del LED
        if (LED_status) {
            LED_status = 0;
        }
        else {
            LED_status = 1;
        }
    }
    
    if (LED_status) {    // encender LED
        digitalWrite(2, HIGH);
        // la casilla de verificación está marcada
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\" checked>LED2");
    }
    else {              // apagar LED
        digitalWrite(2, LOW);
        // la casilla de verificación no está marcada
        cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \
        onclick=\"submit();\">LED2");
    }
}
