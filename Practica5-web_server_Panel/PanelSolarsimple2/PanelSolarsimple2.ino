#include <SPI.h>
#include <Ethernet.h>

// Configuración de red
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

// Pines
const int LED1 = 6;  // D6
const int LED2 = 7;  // D7
const int LED3 = 8;  // D8
const int LED4 = 9;  // D9

// Divisor de voltaje
const float R1 = 30000.0;
const float R2 = 10000.0;
const float FACTOR = (R1 + R2) / R2;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.print("Servidor en: ");
  Serial.println(Ethernet.localIP());
}

float leerVoltaje(int pin) {
  int lectura = analogRead(pin);
  float voltajePin = (lectura * 5.0) / 1024.0;
  return voltajePin * FACTOR;
}

void loop() {
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("Cliente conectado");
    
    String request = "";
    boolean lineaVacia = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        
        if (c == '\n' && lineaVacia) {
          
          // Procesar comandos LED
          if (request.indexOf("LED1=1") > 0) {
            digitalWrite(LED1, HIGH);
          } else if (request.indexOf("LED1=0") > 0) {
            digitalWrite(LED1, LOW);
          }
          
          if (request.indexOf("LED2=1") > 0) {
            digitalWrite(LED2, HIGH);
          } else if (request.indexOf("LED2=0") > 0) {
            digitalWrite(LED2, LOW);
          }
          
          if (request.indexOf("LED3=1") > 0) {
            digitalWrite(LED3, HIGH);
          } else if (request.indexOf("LED3=0") > 0) {
            digitalWrite(LED3, LOW);
          }
          
          if (request.indexOf("LED4=1") > 0) {
            digitalWrite(LED4, HIGH);
          } else if (request.indexOf("LED4=0") > 0) {
            digitalWrite(LED4, LOW);
          }
          
          // Decidir qué enviar
          if (request.indexOf("ajax_inputs") > 0) {
            // Petición AJAX -> XML
            enviarXML(client);
          } else {
            // Petición normal -> HTML
            enviarHTML(client);
          }
          
          break;
        }
        
        lineaVacia = (c == '\n');
      }
    }
    
    delay(10);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void enviarHTML(EthernetClient cl) {
  cl.println("HTTP/1.1 200 OK");
  cl.println("Content-Type: text/html");
  cl.println("Connection: close");
  cl.println();
  
  // Aquí va todo el HTML de arriba
  // O mejor: servir el archivo index.htm desde tarjeta SD
  
  cl.println("<!DOCTYPE html>");
  cl.println("<html><head><title>Redirigiendo...</title></head>");
  cl.println("<body><h1>Cargando interfaz...</h1>");
  cl.println("<p>Por favor coloca el archivo index.htm en la tarjeta SD</p>");
  cl.println("</body></html>");
}

void enviarXML(EthernetClient cl) {
  cl.println("HTTP/1.1 200 OK");
  cl.println("Content-Type: text/xml");
  cl.println("Connection: close");
  cl.println();
  
  cl.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  cl.println("<data>");
  
  // Voltajes analógicos
  cl.print("<analog>");
  cl.print(leerVoltaje(A0), 2);
  cl.println("</analog>");
  
  cl.print("<analog>");
  cl.print(leerVoltaje(A1), 2);
  cl.println("</analog>");
  
  // LDR
  cl.print("<ldr>");
  cl.print(analogRead(A5));
  cl.println("</ldr>");
  
  // LEDs (checkboxes usan "checked", botones usan "on/off")
  cl.print("<LED>");
  cl.print(digitalRead(LED1) ? "checked" : "unchecked");
  cl.println("</LED>");
  
  cl.print("<LED>");
  cl.print(digitalRead(LED2) ? "checked" : "unchecked");
  cl.println("</LED>");
  
  cl.print("<LED>");
  cl.print(digitalRead(LED3) ? "on" : "off");
  cl.println("</LED>");
  
  cl.print("<LED>");
  cl.print(digitalRead(LED4) ? "on" : "off");
  cl.println("</LED>");
  
  cl.println("</data>");
}
