#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xF0, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

const byte LED1 = 2;
const byte LED2 = 3;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Servidor en: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("Nuevo cliente");
    
    String request = "";
    unsigned long timeout = millis();
    
    // ESPERAR Y LEER LA PETICIÓN (máximo 2 segundos)
    while (client.connected() && (millis() - timeout < 2000)) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  // Ver qué llega
        request += c;
        
        // Detectar fin de petición HTTP (doble salto de línea)
        if (request.endsWith("\r\n\r\n") || request.endsWith("\n\n")) {
          Serial.println("\n*** Peticion completa recibida ***");
          break;
        }
      }
    }
    
    // PROCESAR COMANDOS
    if (request.indexOf("GET /?Data=1") >= 0) {
      digitalWrite(LED1, HIGH);
      Serial.println("LED1 -> ON");
    }
    else if (request.indexOf("GET /?Data=2") >= 0) {
      digitalWrite(LED1, LOW);
      Serial.println("LED1 -> OFF");
    }
    else if (request.indexOf("GET /?Data=3") >= 0) {
      digitalWrite(LED2, HIGH);
      Serial.println("LED2 -> ON");
    }
    else if (request.indexOf("GET /?Data=4") >= 0) {
      digitalWrite(LED2, LOW);
      Serial.println("LED2 -> OFF");
    }
    
    // ENVIAR RESPUESTA
    Serial.println("Enviando respuesta...");
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    
    // HTML simplificado
    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta charset='UTF-8'>");
    client.println("<meta name='viewport' content='width=device-width'>");
    client.println("<title>Arduino</title>");
    client.println("<style>");
    client.println("body{font-family:Arial;text-align:center;margin:20px}");
    client.println("button{padding:10px 20px;margin:5px;font-size:16px}");
    client.println(".on{background:#4CAF50;color:white;border:none}");
    client.println(".off{background:#f44336;color:white;border:none}");
    client.println("</style></head><body>");
    
    client.println("<h1>Control Arduino</h1>");
    
    // Voltajes
    client.println("<h2>Voltajes</h2>");
    float v1 = (analogRead(A0) * 5.0 / 1024.0);
    float v2 = (analogRead(A1) * 3.3 / 1024.0);
    client.print("AN0: "); client.print(v1, 2); client.println(" V<br>");
    client.print("AN1: "); client.print(v2, 2); client.println(" V");
    
    // Entradas digitales
    client.println("<h2>Entradas Digitales</h2>");
    client.print("D4: "); 
    client.println(digitalRead(4) == LOW ? "PRESIONADO" : "LIBRE");
    client.println("<br>");
    client.print("D5: "); 
    client.println(digitalRead(5) == LOW ? "PRESIONADO" : "LIBRE");
    
    // LED 1
    client.println("<h2>LED 1</h2>");
    client.print("Estado: <b>");
    client.print(digitalRead(LED1) ? "ON" : "OFF");
    client.println("</b><br><br>");
    client.println("<a href='/?Data=1'><button class='on'>ENCENDER</button></a>");
    client.println("<a href='/?Data=2'><button class='off'>APAGAR</button></a>");
    
    // LED 2
    client.println("<h2>LED 2</h2>");
    client.print("Estado: <b>");
    client.print(digitalRead(LED2) ? "ON" : "OFF");
    client.println("</b><br><br>");
    client.println("<a href='/?Data=3'><button class='on'>ENCENDER</button></a>");
    client.println("<a href='/?Data=4'><button class='off'>APAGAR</button></a>");
    
    client.println("<br><br><hr><a href='/'>Actualizar</a>");
    client.println("</body></html>");
    
    // IMPORTANTE: Dar tiempo antes de cerrar
    delay(100);
    client.stop();
    
    Serial.println("Cliente desconectado\n");
  }
}
