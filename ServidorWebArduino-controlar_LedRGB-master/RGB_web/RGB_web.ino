#include <LedRGB.h>
#include "Tela.h"

byte mac[] = {
  0xDe, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192,168,1,10);
EthernetServer webServer(80);
EthernetClient connetion;

int cod;

LedRGB led(3, 5, 8);
Tela t;

void setup() {
   Serial.begin(9600);
   Ethernet.begin(mac,ip);
   webServer.begin();
   Serial.print("El servidor está en ");
   Serial.println(Ethernet.localIP());
}

void loop() {
  // Preparando el servidor para una nueva conexión.
  t.connetion = webServer.available();
  if(t.connetion){  // Cliente intentando conectarse al servidor.
    Serial.print("Nueva conexión de: ");
    boolean currentLineIsBlank = true; // Variable de control
    String recivido = ""; // Variable para almacenar la petición HTTP
    Serial.println("Conectado");
      while (t.connetion.connected()){
          if(t.connetion.available()){ // Comprobando lo que debe enviarse al servidor.
            char c = t.connetion.read(); // Recibiendo mensaje
            recivido.concat(c); // Añade los caracteres "c" a la cadena "recibido"
            cod=0;
              if(recivido.endsWith("/btVermelho")) {cod = 1;}
                if(recivido.endsWith("/btVerde")) {cod = 2;}
                  if(recivido.endsWith("/btAzul")) {cod = 3;}
                  if(recivido.endsWith("/btDesligar")) {cod = 4;}
                   if(recivido.endsWith("/btSair")) {cod = 5;}
              if(c == '\n' && currentLineIsBlank){
                 t.Tela_login(); // Abra la primera pantalla. (Tela)
                  break;
                  }
              if(cod == 1){
                led.red(1);
                led.green(0);
                led.blue(0);
                t.Tela_login();
                c = t.connetion.read(); 
                break;
              } else{
                if(cod == 2){
                  led.red(0);
                  led.green(1);
                  led.blue(0);
                  t.Tela_login();
                  c = t.connetion.read(); 
                  break;
                  } else {
                    if(cod == 3){
                      led.red(0);
                      led.green(0);
                      led.blue(1);
                      t.Tela_login();
                      c = t.connetion.read(); 
                      break; 
                    } else {
                      if(cod == 4){
                      led.red(0);
                      led.green(0);
                      led.blue(0);
                      t.Tela_login();
                      c = t.connetion.read(); 
                      break; 
                      } else {
                        if(cod == 5){ 
                          led.red(0);
                          led.green(0);
                          led.blue(0);
                          t.connetion.stop();
                          break;
                        }
                      }
                    }
                  }
                }
          }
      }
  }
delay(1);
t.connetion.stop();
}
