/*--------------------------------------------------------------
  Program:      Practica5-web_server_Panel
                Monitor Solar con Gauge

  Description:  Arduino web server para monitoreo de panel solar,
                batería 12V y salida PWM. Utiliza AJAX para
                actualizar valores en tiempo real con gauges.
                La página web se almacena en la tarjeta SD.
  
  Hardware:     Arduino Uno + Ethernet Shield
                SD card (FAT16)
                A2: Voltaje Panel Solar
                A3: Voltaje Batería 12V
                A4: Salida PWM (control)
                A5: LDR (intensidad luz solar)
                D6-D9: LEDs de control
                Digital: 3 LEDs estado (rojo, amarillo, verde)
                
  Software:     Arduino 1.0.5+
                SD card contiene index.htm
  
  Date:         2026
  Author:       Modified for Solar Panel Monitoring
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define REQ_BUF_SZ   80

// Configuración de red
byte mac[] = { 0xD3, 0xAD, 0xB5, 0xAF, 0x05, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
File webFile;
char HTTP_req[REQ_BUF_SZ] = {0};
char req_index = 0;

// Pines analógicos (A0 y A1 usados por Ethernet Shield)
const byte PIN_PANEL = A2;    // Voltaje Panel Solar
const byte PIN_BATERIA = A3;  // Voltaje Batería 12V
const byte PIN_PWM = A4;      // Salida PWM (control)
const byte PIN_LDR = A5;      // LDR intensidad luz

// Pines digitales para LEDs de estado
const byte LED_RED = 2;       // Descargando (ROJO)
const byte LED_YELLOW = 3;    // Equilibrio (AMARILLO)
const byte LED_GREEN = 5;     // Generando (VERDE)

// LEDs de control (D6-D9)
boolean LED_state[4] = {0};   // Estado de D6-D9

// Configuración del divisor de voltaje
const float R1 = 1200.0;      // 30kΩ
const float R2 = 1500.0;      // 10kΩ
const float VREF = 5.0;
const float FACTOR_DIVISION = (R1 + R2) / R2;
const float CALIBRACION = 0.94;
const int NUM_MUESTRAS = 10;

void setup()
{
    // Deshabilitar chip Ethernet
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);
    
    // Inicializar SD
    Serial.println("Inicializando SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - Fallo inicializacion SD!");
        return;
    }
    Serial.println("OK - SD inicializada");
    
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - No encontrado index.htm!");
        return;
    }
    Serial.println("OK - Archivo index.htm encontrado");
    
    // LEDs de estado
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    
    // LEDs de control D6-D9
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    
    Ethernet.begin(mac, ip);
    server.begin();
    
    Serial.print("Servidor en: ");
    Serial.println(Ethernet.localIP());
}

// Leer voltaje con promediado
float leerVoltaje(byte pin) {
  long suma = 0;
  for (int i = 0; i < NUM_MUESTRAS; i++) {
    suma += analogRead(pin);
    delay(2);
  }
  float lecturaPromedio = suma / (float)NUM_MUESTRAS;
  float voltajePin = (lecturaPromedio * VREF) / 1024.0;
  float voltajeReal = voltajePin * FACTOR_DIVISION * CALIBRACION;
  return voltajeReal;
}

// Leer voltaje sin conversión para gauge
int leerADC(byte pin) {
  long suma = 0;
  for (int i = 0; i < NUM_MUESTRAS; i++) {
    suma += analogRead(pin);
    delay(2);
  }
  return suma / NUM_MUESTRAS;
}

// Actualizar LEDs de estado basado en voltajes
void actualizarLEDsEstado() {
  float vBat = leerVoltaje(PIN_BATERIA);
  float vPanel = leerVoltaje(PIN_PANEL);
  
  // Determinar estado del sistema
  boolean descargando = (vBat < 12.0 && vPanel < 12.5);
  boolean generando = (vPanel > 13.0 && vBat < 12.8);
  boolean equilibrio = !descargando && !generando;
  
  digitalWrite(LED_RED, descargando ? HIGH : LOW);
  digitalWrite(LED_YELLOW, equilibrio ? HIGH : LOW);
  digitalWrite(LED_GREEN, generando ? HIGH : LOW);
}

void loop()
{
    // Actualizar LEDs de estado cada cierto tiempo
    static unsigned long ultimaActualizacion = 0;
    if (millis() - ultimaActualizacion > 500) {
      actualizarLEDsEstado();
      ultimaActualizacion = millis();
    }
    
    EthernetClient client = server.available();
    if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;
                    req_index++;
                }
                
                if (c == '\n' && currentLineIsBlank) {
                    client.println("HTTP/1.1 200 OK");
                    
                    // Procesar request AJAX
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        SetLEDs();
                        XML_response(client);
                    }
                    else {  // Request de página web
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        webFile = SD.open("index.htm");
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read());
                            }
                            webFile.close();
                        }
                    }
                    
                    Serial.print(HTTP_req);
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
        delay(1);
        client.stop();
    }
}

void SetLEDs(void) {
    // LEDs D6-D9 (controlados por checkbox/button en HTML)
    if (StrContains(HTTP_req, "LED1=1")) {
        LED_state[0] = 1;
        digitalWrite(6, HIGH);
    }
    else if (StrContains(HTTP_req, "LED1=0")) {
        LED_state[0] = 0;
        digitalWrite(6, LOW);
    }
    if (StrContains(HTTP_req, "LED2=1")) {
        LED_state[1] = 1;
        digitalWrite(7, HIGH);
    }
    else if (StrContains(HTTP_req, "LED2=0")) {
        LED_state[1] = 0;
        digitalWrite(7, LOW);
    }
    if (StrContains(HTTP_req, "LED3=1")) {
        LED_state[2] = 1;
        digitalWrite(8, HIGH);
    }
    else if (StrContains(HTTP_req, "LED3=0")) {
        LED_state[2] = 0;
        digitalWrite(8, LOW);
    }
    if (StrContains(HTTP_req, "LED4=1")) {
        LED_state[3] = 1;
        digitalWrite(9, HIGH);
    }
    else if (StrContains(HTTP_req, "LED4=0")) {
        LED_state[3] = 0;
        digitalWrite(9, LOW);
    }
}

// Enviar respuesta XML con valores de voltajes, PWM y LDR
void XML_response(EthernetClient cl) {
    int panel_adc = leerADC(PIN_PANEL);
    int bateria_adc = leerADC(PIN_BATERIA);
    int pwm_adc = leerADC(PIN_PWM);
    int ldr_adc = leerADC(PIN_LDR);
    
    float vPanel = leerVoltaje(PIN_PANEL);
    float vBat = leerVoltaje(PIN_BATERIA);
    float vPWM = leerVoltaje(PIN_PWM);
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    
    // Valores ADC para gauges
    cl.print("<panel_gauge>");
    cl.print(panel_adc);
    cl.println("</panel_gauge>");
    
    cl.print("<bateria_gauge>");
    cl.print(bateria_adc);
    cl.println("</bateria_gauge>");
    
    cl.print("<pwm_gauge>");
    cl.print(pwm_adc);
    cl.println("</pwm_gauge>");
    
    // Voltajes calculados
    cl.print("<panel_volt>");
    cl.print(vPanel, 2);
    cl.println("</panel_volt>");
    
    cl.print("<bateria_volt>");
    cl.print(vBat, 2);
    cl.println("</bateria_volt>");
    
    cl.print("<pwm_volt>");
    cl.print(vPWM, 2);
    cl.println("</pwm_volt>");
    
    // LDR
    cl.print("<ldr>");
    cl.print(ldr_adc);
    cl.println("</ldr>");
    
    // Estados de LEDs de control (D6-D9)
    for (int i = 0; i < 4; i++) {
        cl.print("<LED>");
        cl.print(LED_state[i] ? "on" : "off");
        cl.println("</LED>");
    }
    
    // Estados de LEDs de estado (rojo, amarillo, verde)
    cl.print("<LED_RED>");
    cl.print(digitalRead(LED_RED) ? "on" : "off");
    cl.println("</LED_RED>");
    
    cl.print("<LED_YELLOW>");
    cl.print(digitalRead(LED_YELLOW) ? "on" : "off");
    cl.println("</LED_YELLOW>");
    
    cl.print("<LED_GREEN>");
    cl.print(digitalRead(LED_GREEN) ? "on" : "off");
    cl.println("</LED_GREEN>");
    
    cl.print("</inputs>");
}

void StrClear(char *str, char length) {
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

char StrContains(char *str, char *sfind) {
    char found = 0;
    char index = 0;
    char len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }
    return 0;
}

