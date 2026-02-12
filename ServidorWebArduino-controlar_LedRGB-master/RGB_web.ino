#include <LedRGB.h>
#include "Tela.h"

byte mac[] = {
  0xDe, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192,168,1,10);
EthernetServer webServer(80);
EthernetClient conexao;

int cod;

LedRGB led(3, 5, 8);
Tela t;

void setup() {
   Serial.begin(9600);
   Ethernet.begin(mac,ip);
   webServer.begin();
   Serial.print("Servidor esta em ");
   Serial.println(Ethernet.localIP());
}

void loop() {
  // Preparando servidor para uma nova conexão
  t.conexao = webServer.available();
  if(t.conexao){  // Cliente tentando se conectar no server
    Serial.print("Nova conexao");
    boolean currentLineIsBlank = true; // Variavel de controle
    String recebido;
    Serial.println("Conectado");
      while (t.conexao.connected()){
          if(t.conexao.available()){ // Verificando o que tem para ser enviado p/ servidor
            char c = t.conexao.read(); // Recebendo mensagem
            recebido.concat(c); // Anexa os caracteres de "c" a string "recebido"
            cod=0;
              if(recebido.endsWith("/btVermelho")) {cod = 1;}
                if(recebido.endsWith("/btVerde")) {cod = 2;}
                  if(recebido.endsWith("/btAzul")) {cod = 3;}
                  if(recebido.endsWith("/btDesligar")) {cod = 4;}
                   if(recebido.endsWith("/btSair")) {cod = 5;}
              if(c == '\n' && currentLineIsBlank){
                 t.Tela_login(); // Abre a primeira tela
                  break;
                  }
              if(cod == 1){
                led.red(1);
                led.green(0);
                led.blue(0);
                t.Tela_login();
                c = t.conexao.read(); 
                break;
              } else{
                if(cod == 2){
                  led.red(0);
                  led.green(1);
                  led.blue(0);
                  t.Tela_login();
                  c = t.conexao.read(); 
                  break;
                  } else {
                    if(cod == 3){
                      led.red(0);
                      led.green(0);
                      led.blue(1);
                      t.Tela_login();
                      c = t.conexao.read(); 
                      break; 
                    } else {
                      if(cod == 4){
                      led.red(0);
                      led.green(0);
                      led.blue(0);
                      t.Tela_login();
                      c = t.conexao.read(); 
                      break; 
                      } else {
                        if(cod == 5){ 
                          led.red(0);
                          led.green(0);
                          led.blue(0);
                          t.conexao.stop();
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
t.conexao.stop();
}
