#include <Ethernet_W5500.h>
#include <DS1307.h>
#include <SPI.h>
#include <MFRC522.h>
//DS1307 rtc(A4, A5);
#define SS_PIN 9   //PINO SDA
#define RST_PIN 8  //PINO DE RESET
#define buzzer 3   //pino buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN);  //PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS
EthernetClient client;

#if defined(WIZ550io_WITH_MACADDRESS)  // Use assigned MAC address of WIZ550io
#else
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#endif

char server[] = "urlsite.com.br";
long randNumer;
void setup() {
  Serial.begin(9600);  //INICIALIZA A SERIAL
  SPI.begin();         //INICIALIZA O BARRAMENTO SPI
  mfrc522.PCD_Init();  //INICIALIZA MFRC522
  pinMode(buzzer, OUTPUT);
  Serial.println("RANDO:");
  randomSeed(analogRead(0));
  // disable rfid
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  // disable sd card
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);


  // start the Ethernet connection:
#if defined(WIZ550io_WITH_MACADDRESS)
  if (Ethernet.begin() == 0) {
#else
  if (Ethernet.begin(mac) == 0) {
  }
#endif
    Serial.println("Failed to configure Ethernet using DHCP");
#if defined(WIZ550io_WITH_MACADDRESS)
    Ethernet.begin(ip);
#endif

    delay(1500);
    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());

    Serial.println("connecting...");

    /*
    * Fim Define IP
    */
    acesso(5);
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();
  }


  void loop() {

    pinMode(4, OUTPUT);  //ANTES DE INICIAR O ETHERNET, DESABILITA O SDCARD
    digitalWrite(4, HIGH);

    // Look for new cards
    if (mfrc522.PICC_IsNewCardPresent()) {
      // Select one of the cards
      if (mfrc522.PICC_ReadCardSerial()) {


        Serial.print("UID da tag :");

        String conteudo = "";
        byte letra;
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
          conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        acesso(1);
        Serial.println();
        Serial.print("Mensagem : ");
        conteudo.trim();


        if (client.connect(server, 80)) {  // REPLACE WITH YOUR SERVER ADDRESS
          randNumer = random(0, 255);
          String ler = "GET /api/api.php?e=lp";
          ler += "&tag=";
          ler += (String)conteudo;
          ler += "&stToken=";
          ler += (String)randNumer;
          ler += " HTTP/1.1";
          client.println(ler);
          client.println("Host: hcf.adeweb.com.br");
          client.println();
          Serial.println(ler);

          delay(1000);
          /*
     *  REsposta
     */
          int count = 0;
          //zzzz
          int novoTag = 0;
          ///xxx
          int entradasaida = 0;
          //ssss
          int notentradasaida = 0;
          int erro = 0;

          char c = 'i';
          while (client.connected()) {
            if (client.available()) {

              c = client.read();


              //novotag
              if (c == 'X') {
                count = count + 1;
                entradasaida = entradasaida + 1;
                Serial.println(c);
              }

              if (c == 'Z') {
                count = count + 1;
                novoTag = novoTag + 1;
              }

              if (c == 'S') {
                count = count + 1;
                notentradasaida = notentradasaida + 1;
              }
              if (c == 'W') {
                count = count + 1;
                erro = erro + 1;
              }

              if (entradasaida == 6) {
                acesso(2);
                entradasaida = 0;
                count = 0;
                break;
              }
              if (notentradasaida == 6) {
                acesso(5);
                entradasaida = 0;
                count = 0;
                break;
              }

              if (novoTag == 6) {
                acessoNegado(3);
                novoTag = 0;
                count = 0;
                break;
              }
              if (erro == 6) {
                acessoNegado(3);
                erro = 0;
                count = 0;
                break;
              }
            }
          }
        }

        if (client.connected()) {
          client.stop();
          // DISCONNECT FROM THE SERVER
        }
      }
    }
  }

  void acesso(int bips) {
    for (int j = 0; j < bips; j++) {
      tone(buzzer, 450);
      delay(200);
      noTone(buzzer);
      delay(500);
    }
  }

  void acessoNegado(int bips) {
    for (int j = 0; j < bips; j++) {
      tone(buzzer, 500);
      delay(300);
      noTone(buzzer);
      delay(500);
    }
  }
