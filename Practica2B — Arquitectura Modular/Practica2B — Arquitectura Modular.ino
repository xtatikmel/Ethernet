#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xF0, 0xED };//Ponemos la dirección MAC de la Ethernet Shield
IPAddress ip(192,168,1,177); //Asignamos  la IP al Arduino
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto
 
void setup()
{
  Serial.begin(9600);
 
  // Inicializamos la comunicación Ethernet y el servidor
  Ethernet.begin(mac, ip);
  //server.begin();???
  webServer.begin();
  Serial.print("El servidor está en ");
  Serial.println(Ethernet.localIP());
  

}
void loop()
{
    EthernetClient client = server.available(); //Creamos un cliente Web
    //Verificamos si se detecte un cliente a través de una petición HTTP
    if (client) {
      Serial.println("Nuevo cliente");
      boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacía
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();//Leemos la petición HTTP carácter por carácter
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        if(cadena.length()<50)
        {
            cadena.concat(c);//concatenmos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String
            
}