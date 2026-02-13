#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xF0, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

const byte LED1 = 2;
const byte LED2 = 3;
const byte PIN_BATERIA = A3;   // Batería 12V
const byte PIN_PANEL = A2;      // Panel Solar 12V

// Configuración divisor de voltaje 30k/10k
const float R1 = 30000.0;  // 30kΩ
const float R2 = 10000.0;  // 10kΩ
const float VREF = 5.0;    // Voltaje de referencia Arduino
const float FACTOR_DIVISION = (R1 + R2) / R2;  // = 4.0

// Calibración fina (ajustar según tu multímetro)
const float CALIBRACION = 1.04;  // Modificar si hay error sistemático

// Configuración de muestreo para reducir ruido
const int NUM_MUESTRAS = 10;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  // Configurar referencia analógica a 5V (default en UNO)
  analogReference(DEFAULT);
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.println("=============================");
  Serial.println("Monitor Bateria/Panel Solar");
  Serial.print("Divisor: R1=");
  Serial.print(R1/1000);
  Serial.print("k R2=");
  Serial.print(R2/1000);
  Serial.println("k");
  Serial.print("Factor division: ");
  Serial.println(FACTOR_DIVISION);
  Serial.print("Rango: 0-");
  Serial.print(VREF * FACTOR_DIVISION);
  Serial.println("V");
  Serial.print("Servidor: ");
  Serial.println(Ethernet.localIP());
  Serial.println("=============================\n");
}

// Función mejorada de lectura con promediado
float leerVoltaje(byte pin) {
  long suma = 0;
  
  // Tomar múltiples muestras
  for (int i = 0; i < NUM_MUESTRAS; i++) {
    suma += analogRead(pin);
    delay(2);  // Pequeña pausa entre lecturas
  }
  
  // Calcular promedio
  float lecturaPromedio = suma / (float)NUM_MUESTRAS;
  
  // Convertir a voltaje en el pin
  float voltajePin = (lecturaPromedio * VREF) / 1024.0;
  
  // Calcular voltaje real aplicando factor de división
  float voltajeReal = voltajePin * FACTOR_DIVISION * CALIBRACION;
  
  return voltajeReal;
}

// Función para estimar estado de batería 12V
String estadoBateria(float voltaje) {
  if (voltaje >= 12.6) return "CARGADA (100%)";
  else if (voltaje >= 12.4) return "BUENA (75-99%)";
  else if (voltaje >= 12.2) return "MEDIA (50-75%)";
  else if (voltaje >= 12.0) return "BAJA (25-50%)";
  else if (voltaje >= 11.8) return "MUY BAJA (<25%)";
  else if (voltaje < 11.8 && voltaje > 5.0) return "CRITICA!";
  else return "DESCONECTADA";
}

void loop() {
  // Mostrar lecturas por serial cada 5 segundos
  static unsigned long ultimoSerial = 0;
  if (millis() - ultimoSerial > 5000) {
    float vBat = leerVoltaje(PIN_BATERIA);
    float vPanel = leerVoltaje(PIN_PANEL);
    
    Serial.println("--- Lecturas ---");
    Serial.print("Bateria: ");
    Serial.print(vBat, 2);
    Serial.print("V [");
    Serial.print(estadoBateria(vBat));
    Serial.println("]");
    Serial.print("Panel: ");
    Serial.print(vPanel, 2);
    Serial.println("V");
    Serial.println();
    
    ultimoSerial = millis();
  }
  
  // Servidor web
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println(">>> Nueva conexion web <<<");
    
    String request = "";
    String firstLine = "";
    boolean isFirstLine = true;
    unsigned long timeout = millis();
    
    while (client.connected() && (millis() - timeout < 2000)) {
      if (client.available()) {
        timeout = millis();
        char c = client.read();
        request += c;
        
        if (isFirstLine && c != '\n' && c != '\r') {
          firstLine += c;
        } else if (c == '\n' && isFirstLine) {
          isFirstLine = false;
        }
        
        if (request.endsWith("\r\n\r\n") || request.endsWith("\n\n")) {
          break;
        }
      }
    }
    
    // Procesar comandos LED
    if (firstLine.indexOf("Data=1") > 0) digitalWrite(LED1, HIGH);
    else if (firstLine.indexOf("Data=2") > 0) digitalWrite(LED1, LOW);
    else if (firstLine.indexOf("Data=3") > 0) digitalWrite(LED2, HIGH);
    else if (firstLine.indexOf("Data=4") > 0) digitalWrite(LED2, LOW);
    
    // Leer voltajes actuales
    float voltajeBateria = leerVoltaje(PIN_BATERIA);
    float voltajePanel = leerVoltaje(PIN_PANEL);
    
    // Enviar respuesta HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Connection: close");
    client.println();
    
    // HTML
    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta charset='UTF-8'>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<meta http-equiv='refresh' content='10'>"); // Auto-refresh cada 10s
    client.println("<title>Monitor Solar</title>");
    client.println("<style>");
    client.println("body{font-family:Arial;max-width:800px;margin:20px auto;padding:20px;background:#f0f0f0}");
    client.println("h1{text-align:center;color:#2c3e50;border-bottom:3px solid #3498db;padding-bottom:10px}");
    client.println(".card{background:white;padding:20px;margin:15px 0;border-radius:10px;box-shadow:0 4px 6px rgba(0,0,0,0.1)}");
    client.println(".voltaje{font-size:48px;font-weight:bold;color:#3498db;text-align:center;margin:10px 0}");
    client.println(".estado{text-align:center;font-size:18px;padding:10px;border-radius:5px;margin-top:10px}");
    client.println(".bueno{background:#2ecc71;color:white}");
    client.println(".medio{background:#f39c12;color:white}");
    client.println(".malo{background:#e74c3c;color:white}");
    client.println("button{padding:12px 24px;margin:5px;font-size:16px;border:none;border-radius:5px;cursor:pointer}");
    client.println(".on{background:#27ae60;color:white}");
    client.println(".off{background:#c0392b;color:white}");
    client.println(".info{background:#ecf0f1;padding:10px;border-left:4px solid #3498db;margin:10px 0}");
    client.println("</style></head><body>");
    
    client.println("<h1>⚡ Monitor de Sistema Solar</h1>");
    
    // Card Batería
    client.println("<div class='card'>");
    client.println("<h2>🔋 Batería 12V</h2>");
    client.print("<div class='voltaje'>");
    client.print(voltajeBateria, 2);
    client.println(" V</div>");
    
    String estadoBat = estadoBateria(voltajeBateria);
    String claseBat = "medio";
    if (voltajeBateria >= 12.4) claseBat = "bueno";
    else if (voltajeBateria < 12.0) claseBat = "malo";
    
    client.print("<div class='estado ");
    client.print(claseBat);
    client.print("'>");
    client.print(estadoBat);
    client.println("</div>");
    
    client.print("<div class='info'>Lectura ADC: ");
    client.print(analogRead(PIN_BATERIA));
    client.println("/1023</div>");
    client.println("</div>");
    
    // Card Panel Solar
    client.println("<div class='card'>");
    client.println("<h2>☀️ Panel Solar</h2>");
    client.print("<div class='voltaje'>");
    client.print(voltajePanel, 2);
    client.println(" V</div>");
    
    client.print("<div class='estado ");
    if (voltajePanel >= 13.0) client.print("bueno");
    else if (voltajePanel >= 11.0) client.print("medio");
    else client.print("malo");
    client.print("'>");
    
    if (voltajePanel >= 13.0) client.print("GENERANDO (SOL)");
    else if (voltajePanel >= 11.0) client.print("GENERACION BAJA");
    else if (voltajePanel > 5.0) client.print("SIN SOL");
    else client.print("DESCONECTADO");
    client.println("</div>");
    
    client.print("<div class='info'>Lectura ADC: ");
    client.print(analogRead(PIN_PANEL));
    client.println("/1023</div>");
    client.println("</div>");
    
    // Card Entradas Digitales
    client.println("<div class='card'>");
    client.println("<h2>🔌 Entradas Digitales</h2>");
    client.print("D4: <strong>");
    client.print(digitalRead(4) == LOW ? "ACTIVO" : "INACTIVO");
    client.println("</strong><br>");
    client.print("D5: <strong>");
    client.print(digitalRead(5) == LOW ? "ACTIVO" : "INACTIVO");
    client.println("</strong>");
    client.println("</div>");
    
    // Card LEDs
    client.println("<div class='card'>");
    client.println("<h2>💡 Control de Salidas</h2>");
    
    client.print("<p>LED 1: <strong>");
    client.print(digitalRead(LED1) ? "ENCENDIDO" : "APAGADO");
    client.println("</strong><br>");
    client.println("<a href='/?Data=1'><button class='on'>ON</button></a>");
    client.println("<a href='/?Data=2'><button class='off'>OFF</button></a></p>");
    
    client.print("<p>LED 2: <strong>");
    client.print(digitalRead(LED2) ? "ENCENDIDO" : "APAGADO");
    client.println("</strong><br>");
    client.println("<a href='/?Data=3'><button class='on'>ON</button></a>");
    client.println("<a href='/?Data=4'><button class='off'>OFF</button></a></p>");
    client.println("</div>");
    
    // Info técnica
    client.println("<div class='info'>");
    client.println("📊 <strong>Configuración:</strong> Divisor 1,kΩ/1,5kΩ (Factor 4:1) | Rango: 0-20V | Auto-refresh: 10s");
    client.println("</div>");
    
    client.println("</body></html>");
    
    delay(50);
    client.flush();
    delay(50);
    client.stop();
    
    Serial.println(">>> Conexion cerrada <<<\n");
  }
}
