/*
Web client
En este sketch Arduino se conecta a una pagina Web (http://www.google.com)
usando un Arduino Wiznet Ethernet shield.
Circuit:
* Ethernet shield ocupa los pins 10, 11, 12, 13
created 18 Dec 2009
modified 9 Apr 2012
by David A. Mellis
traducido J.M. Ruiz Nov. 2012
Implementado en| Dexter Newtron Labs 2026
*/
#include <SPI.h>
#include <Ethernet.h>
// Pone la dirección MAC en el controlador.
// Los Shields Ethernet más actuales Ethernet tienen una dirección MAC impresa en una
// etiqueta pegada en ellos
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,1,177); // Google
// Inicializar la biblioteca de cliente Ethernet
// Con la dirección IP y el puerto del servidor
// Que desea conectarse (por defecto es el puerto 80 para HTTP):
EthernetClient client;

void setup() {
// Abre el puetro serie para la comunicación:
Serial.begin(9600);
while (!Serial) {
; // esperar para para conectar. Necesario para Leonardo sólo
}
// inicia la conexión con Ethernet:
if (Ethernet.begin(mac) == 0) {
Serial.println("Falla la conexión configurada usand0 DHCP");
// No tiene sentido seguir adelante, así que no hace nada:
for(;;)
;
}
// intenta una segunda inicialización:
delay(1000);
Serial.println("conectando...");
// si tienes una conexión, informar a través del puerto serie:
if (client.connect(server, 80)) {
Serial.println("conectado");
// Make a HTTP request:
client.println("GET /search?q=arduino HTTP/1.0");
client.println();
}
else {
// kf no recibió una conexión con el servidor:
Serial.println("conexion fallida");
} 
}
 
  void loop()
  {
// Si hay bytes disponibles entrantes
// Del servidor los lee e imprime:
if (client.available()) {
char c = client.read();

Serial.print(c);
}
// Si el servidor está desconectado, se detiene el cliente:
if (!client.connected()) {
Serial.println();
Serial.println("disconnectar.");
client.stop();
// no hace nada:
for(;;)
;
} }
