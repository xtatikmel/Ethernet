# Propuesta de Integración de Proyectos Externos en Nuevas Prácticas

## Progresión Actual de Aprendizaje

Las prácticas existentes siguen esta progresión lógica:

```
P1: Fundamentos → Cliente HTTP, Servidor básico, LED, Switch
P2: Interactividad → Control de I/O, AJAX, Voltímetro
P4: Almacenamiento → SD card, páginas web externas, XML, Gauges
P5: Complejidad → Múltiples I/O, Arduino Mega, Panel Solar
P6: IoT en la nube → ThingSpeak
```

Los proyectos externos del workspace se pueden integrar como **nuevas prácticas** siguiendo esta misma progresión. A continuación el análisis de cada uno y dónde encajan.

---

## 1. AlarmasUT-main — Servidor de Alarmas con UI Web

**Directorio:** [`AlarmasUT-main/`](../AlarmasUT-main/)

### Qué hace
Interfaz web moderna para un sistema de alarmas con Arduino UNO + Ethernet Shield W5100. Incluye un servidor Node.js (Express) para desarrollo local y una UI con Bootstrap 5, animaciones CSS (WOW.js/Animate.css) y jQuery.

### Tecnología
- Arduino UNO + Ethernet Shield W5100
- Frontend: HTML5, Bootstrap 5, jQuery, WOW.js, Animate.css
- Backend de prueba: Node.js + Express (puerto 8080)

### Dónde integrar: **Práctica 7 — Interfaz Web Moderna**

**Justificación:** Después de ThingSpeak (P6), el siguiente paso lógico es mejorar la **calidad visual** de las interfaces. Este proyecto introduce frameworks CSS modernos (Bootstrap) y animaciones, algo que no se ha cubierto en las prácticas anteriores donde el HTML es básico.

**Objetivos de la práctica:**
1. Diseñar interfaces web responsivas con Bootstrap para Arduino
2. Integrar animaciones CSS en páginas servidas desde SD
3. Usar jQuery para interacción dinámica con el hardware

**Consideraciones:**
- Las librerías CSS/JS (Bootstrap, jQuery, WOW.js) son pesadas para servir desde Arduino directamente
- Opción A: Servir desde SD card (requiere espacio)
- Opción B: Cargar desde CDN (requiere Internet)
- El servidor Node.js es solo para desarrollo/pruebas en PC, no se sube al Arduino

---

## 2. arduino-dht-ethernet-master — Logger de Temperatura/Humedad

**Directorio:** [`arduino-dht-ethernet-master/`](../arduino-dht-ethernet-master/)

### Qué hace
Arduino como **cliente HTTP** que lee temperatura y humedad con sensor DHT11 y envía los datos por **POST** a un servidor PHP cada 30 minutos. El servidor PHP almacena los datos y los muestra en gráficos.

### Tecnología
- Arduino UNO + Ethernet Shield W5100 + DHT11 (pin 2)
- Librerías: `SPI.h`, `Ethernet.h`, `dht.h`, `stdlib.h`
- Backend: PHP + MySQL (servidor externo)
- Protocolo: HTTP POST con datos URL-encoded
- Intervalo: 30 minutos entre envíos

### Dónde integrar: **Práctica 3 — Sensores y Envío de Datos (POST)**

**Justificación:** Hay un salto entre P2 (voltímetro/AJAX) y P4 (SD card). Esta práctica llena ese vacío introduciendo:
- Sensor DHT11 (temperatura/humedad) — primer sensor real
- HTTP POST (hasta ahora solo se ha usado GET)
- Arduino como cliente que envía datos a un servidor externo

**Objetivos de la práctica:**
1. Conectar y leer sensor DHT11
2. Enviar datos por HTTP POST a un servidor
3. Entender la diferencia entre GET y POST
4. Implementar intervalos de envío con `delay()`

**Consideraciones:**
- Requiere librería `dht.h` instalada en Arduino IDE
- Necesita un servidor PHP accesible (puede ser local con XAMPP/WAMP)
- El script PHP (`chartdata.php`, `index.htm`) está incluido en el proyecto
- Ajustar `server[]` con la IP/dominio de tu servidor PHP
- El intervalo de 30 min es largo para pruebas; reducir a 5-10 segundos durante desarrollo

---

## 3. Arduino-W5100-mysql-master — Envío de Datos a MySQL

**Directorio:** [`Arduino-W5100-mysql-master/`](../Arduino-W5100-mysql-master/)

### Qué hace
Arduino envía datos simulados (voltaje, amperaje, kW) a un servidor PHP/MySQL cada 5 minutos mediante HTTP POST. Simula lecturas de un medidor eléctrico.

### Tecnología
- Arduino + Ethernet Shield W5100
- Librerías: `Ethernet.h`, `SPI.h`
- Backend: PHP (`add.php`) + MySQL
- Protocolo: HTTP POST
- Datos: Voltaje, Amperaje, kW total, kW neto (simulados con `random()`)

### Dónde integrar: **Práctica 3 (complemento) — Base de Datos**

**Justificación:** Complementa al proyecto DHT-Ethernet. Mientras el DHT envía datos reales de sensores, este proyecto muestra cómo estructurar datos para una base de datos MySQL. Se pueden combinar ambos en una sola práctica con dos ejercicios.

**Objetivos adicionales:**
1. Estructurar datos para inserción en MySQL
2. Crear script PHP que reciba y almacene datos
3. Usar `millis()` en lugar de `delay()` para intervalos no bloqueantes

**Consideraciones:**
- Usa `delay(300000)` (5 min) — reducir para pruebas
- Los datos son simulados con `random()` — reemplazar con lecturas reales de sensores
- Requiere servidor con PHP + MySQL (XAMPP, WAMP o hosting)
- El archivo `add.php` y `connect.php` están en [`W5100-Eventos-master/`](../W5100-Eventos-master/)

---

## 4. W5100-Eventos-master — Monitoreo de Eventos con Sensor de Lluvia

**Directorio:** [`W5100-Eventos-master/`](../W5100-Eventos-master/)

### Qué hace
Monitorea un **sensor de lluvia** y envía eventos a una base de datos web (phpMyAdmin/MySQL) en hosting gratuito (000webhost). Incluye scripts PHP completos para recibir y mostrar datos.

### Tecnología
- Arduino + Ethernet Shield W5100 + Sensor de lluvia
- Backend: PHP + MySQL (000webhost)
- Archivos: `W5100-Eventos.ino`, `add.php`, `connect.php`, `index.php`

### Dónde integrar: **Práctica 3 (ejercicio avanzado) — Eventos en la Nube**

**Justificación:** Es la evolución natural del envío de datos: no solo enviar lecturas periódicas sino **eventos** (cuando llueve). Introduce el concepto de monitoreo basado en eventos vs. monitoreo periódico.

**Objetivos adicionales:**
1. Detectar eventos con sensores (lluvia)
2. Enviar solo cuando ocurre un evento (eficiencia)
3. Desplegar backend en hosting gratuito

**Consideraciones:**
- 000webhost puede tener limitaciones de velocidad/disponibilidad
- Alternativa: usar servidor local para pruebas
- Los scripts PHP están completos y listos para usar

---

## 5. arduino-sms-web-master — Sistema SMS por Web

**Directorio:** [`arduino-sms-web-master/`](../arduino-sms-web-master/)

### Qué hace
Interfaz web para enviar **SMS** usando Arduino Nano + Ethernet Shield W5100 + módulo GSM SIM900. El usuario ingresa número y mensaje en una página web y Arduino lo envía por SMS.

### Tecnología
- Arduino Nano + Ethernet Shield W5100 + SIM900 (GSM)
- Librerías: `SPI.h`, `Ethernet.h`, `SIM900.h`, `sms.h`, `SoftwareSerial.h`
- GSM TX → pin 2, RX → pin 3
- Módulo EFCOM v1.2 (GSM_ON → D6, GSM_RESET → D5)

### Dónde integrar: **Práctica 8 — Comunicación GSM/SMS**

**Justificación:** Introduce un canal de comunicación completamente nuevo (GSM/SMS) además de Ethernet. Es un nivel avanzado que combina dos interfaces de comunicación simultáneas.

**Objetivos de la práctica:**
1. Configurar módulo GSM SIM900 con Arduino
2. Crear interfaz web para envío de SMS
3. Combinar Ethernet + GSM en un mismo proyecto
4. Manejar SoftwareSerial para comunicación con el módulo GSM

**Consideraciones:**
- Requiere módulo GSM SIM900 y tarjeta SIM activa con saldo
- Conflicto de pines: GSM usa D2, D3 (SoftwareSerial) + D5, D6 (control)
- El código original usa Arduino Nano — adaptar para UNO
- Modificar `GSM.h` para los pines correctos del módulo EFCOM
- Red configurada con IP estática `10.220.10.171` — ajustar

---

## 6. Arduino-Voice-control-master — Control por Voz

**Directorio:** [`Arduino-Voice-control-master/`](../Arduino-Voice-control-master/)

### Qué hace
Arduino como **cliente HTTP** que consulta un servidor web para obtener comandos de voz (texto). Un sitio web con reconocimiento de voz (Web Speech API) escribe comandos en un archivo de texto, y Arduino los lee periódicamente para controlar un LED.

### Tecnología
- Arduino + Ethernet Shield W5100 + LED (pin 5)
- Librerías: `SPI.h`, `Ethernet.h`
- Backend: PHP (`ajax.php`, `index.php`) + archivo `translations.txt`
- Reconocimiento de voz: Web Speech API (navegador)
- Protocolo: HTTP GET cada 5 segundos

### Dónde integrar: **Práctica 9 — Control por Voz vía Web**

**Justificación:** Es el nivel más avanzado de interacción: control por voz. Combina tecnologías web modernas (Web Speech API) con Arduino. Requiere entender la arquitectura cliente-servidor completa.

**Objetivos de la práctica:**
1. Implementar reconocimiento de voz en el navegador
2. Crear backend PHP que almacene comandos
3. Arduino como cliente que consulta comandos periódicamente
4. Parsear respuestas HTTP para extraer comandos

**Consideraciones:**
- Web Speech API requiere HTTPS en producción (Chrome)
- El servidor web debe ser accesible desde Arduino
- No usar pines 4, 11, 12, 13 (Ethernet Shield)
- El archivo `translations.txt` actúa como "buzón" de comandos
- Latencia de ~5 segundos entre comando de voz y acción

---

## 7. DualDHTthermostat-master — Termostato Dual con DHT

**Directorio:** [`DualDHTthermostat-master/`](../DualDHTthermostat-master/)

### Qué hace
Termostato web con **2 sensores DHT** (DHT22 + DHT11) que controla calefacción y refrigeración con modos automáticos. Interfaz web para ajustar temperaturas y modos.

### Tecnología
- Arduino + Ethernet Shield W5100 + DHT22 (pin 2) + DHT11 (pin 5)
- Librerías: `DHT.h`, `SPI.h`, `Ethernet.h`
- Modos: Heat (H), Cool (C), Auto (A), Standby (S)
- Configuración: gap de overlap, setpoints de temperatura

### Dónde integrar: **Práctica 5B — Termostato Inteligente (alternativa al Panel Solar)**

**Justificación:** Tiene la misma complejidad que el Panel Solar (P5) pero con un enfoque diferente: control de clima. Puede ser una práctica alternativa o complementaria.

**Objetivos de la práctica:**
1. Usar múltiples sensores DHT simultáneamente
2. Implementar lógica de control (termostato con histéresis)
3. Crear interfaz web para configuración de parámetros
4. Manejar modos de operación (Heat/Cool/Auto/Standby)

**Consideraciones:**
- Requiere librería `DHT.h` instalada
- DHT22 es más preciso que DHT11 — comparar lecturas
- Resistencia pullup de 10kΩ entre pin de datos y VCC
- Temperaturas en Fahrenheit — convertir a Celsius si se desea
- El gap de overlap (1.5°) evita oscilaciones rápidas del relé

---

## 8. rfid-main — Lector RFID con Registro Web

**Directorio:** [`rfid-main/`](../rfid-main/)

### Qué hace
Lector **RFID MFRC522** que registra horas de inicio/fin de entregas. Los datos se envían a un sitio web. Incluye buzzer para confirmación audible.

### Tecnología
- Arduino + Ethernet Shield W5500 + MFRC522 (RFID) + Buzzer (pin 3)
- Librerías: `Ethernet_W5500.h`, `DS1307.h`, `SPI.h`, `MFRC522.h`
- RFID: SDA → pin 9, RST → pin 8
- RTC DS1307 (I2C: A4, A5)

### Dónde integrar: **Práctica 10 — Control de Acceso RFID**

**Justificación:** Introduce un periférico completamente nuevo (RFID) y el concepto de identificación/autenticación. Es una aplicación práctica muy demandada (control de acceso, registro de asistencia).

**Objetivos de la práctica:**
1. Configurar lector RFID MFRC522 con Arduino
2. Leer y procesar tarjetas/tags RFID
3. Registrar eventos con timestamp (RTC DS1307)
4. Enviar registros a servidor web

**Consideraciones:**
- Usa Ethernet **W5500** (no W5100) — verificar compatibilidad de librería
- Conflicto SPI: RFID y Ethernet comparten bus SPI — manejar chip select
- Pin 9 para RFID SS, Pin 4 para SD — deshabilitar SD si no se usa
- El código deshabilita RFID (`digitalWrite(9, HIGH)`) al inicio para inicializar Ethernet primero
- Requiere tarjetas/tags RFID MIFARE para pruebas

---

## 9. ServidorWebArduino-controlar_LedRGB-master — Control de LED RGB

**Directorio:** [`ServidorWebArduino-controlar_LedRGB-master/`](../ServidorWebArduino-controlar_LedRGB-master/)

### Qué hace
Servidor web con arquitectura **modular** (clases C++) para controlar un LED RGB. Separa la lógica de las "pantallas" (páginas HTML) en archivos `.cpp/.h` independientes.

### Tecnología
- Arduino UNO + Ethernet Shield W5100 + LED RGB
- Arquitectura: `RGB_web.ino` (principal) + `Tela.cpp/Tela.h` (páginas) + `LedRGB.cpp/LedRGB.h` (componente)
- Patrón: Separación de responsabilidades (MVC simplificado)

### Dónde integrar: **Práctica 2B — Arquitectura Modular (complemento)**

**Justificación:** Introduce buenas prácticas de programación (clases, separación de archivos) que no se ven en las prácticas actuales donde todo está en un solo `.ino`. Puede ser un complemento de P2 para enseñar organización de código.

**Objetivos de la práctica:**
1. Organizar código Arduino en múltiples archivos (.cpp/.h)
2. Crear clases para componentes (LED RGB)
3. Separar lógica de presentación (HTML) de lógica de hardware
4. Controlar LED RGB con PWM desde interfaz web

**Consideraciones:**
- Requiere entender clases C++ básicas
- Los archivos `.cpp` y `.h` deben estar en la misma carpeta del `.ino`
- El LED RGB necesita 3 pines PWM
- La arquitectura es escalable: agregar nuevas "pantallas" es crear un método en `Tela.cpp`

---

## 10. simpleweatherstation-master — Estación Meteorológica

**Directorio:** [`simpleweatherstation-master/`](../simpleweatherstation-master/)

### Qué hace
Estación meteorológica que lee temperatura/humedad con DHT11 y envía datos a **SparkFun Data** (data.sparkfun.com) usando Phant. También incluye servidor Telnet para acceso local.

### Tecnología
- Arduino Mega 2560 + Ethernet Shield W5100 + DHT11
- Librerías: `SPI.h`, `Ethernet.h`, `DHT.h`, `stdlib.h`
- Nube: SparkFun Phant (data.sparkfun.com)
- Protocolos: HTTP GET (Phant) + Telnet
- Intervalo: 60 segundos

### Dónde integrar: **Práctica 6B — Plataformas IoT Alternativas**

**Justificación:** Complementa ThingSpeak (P6) mostrando otra plataforma IoT (SparkFun). Introduce Telnet como protocolo alternativo a HTTP para acceso local.

**Objetivos de la práctica:**
1. Comparar plataformas IoT (ThingSpeak vs SparkFun)
2. Implementar servidor Telnet para diagnóstico
3. Usar claves API (pública/privada) para autenticación

**Consideraciones:**
- SparkFun Phant fue **descontinuado** — usar alternativa como Blynk, Adafruit IO o MQTT
- Usa Arduino **Mega 2560** (no UNO)
- Requiere claves PUBLIC_KEY y PRIVATE_KEY
- El servidor Telnet permite acceso por terminal (PuTTY, etc.)

---

## 11. WebestufaServer — Servidor con Mega + DHT11 + Relés + LCD + RTC

**Directorio:** [`WebestufaServer_ArduinoMega2560_EthernetW5100-master/`](../WebestufaServer_ArduinoMega2560_EthernetW5100-master/)

### Qué hace
Servidor web completo para control de **invernadero** (estufa) con Arduino Mega 2560. Incluye sensor DHT11, módulo de relés (hasta 8), LCD I2C 16x2 y reloj RTC DS1307.

### Tecnología
- Arduino Mega 2560 + Ethernet Shield W5100
- Sensores: DHT11 (pin A0)
- Actuadores: Módulo de 8 relés
- Display: LCD 16x2 I2C (dirección 0x27)
- Reloj: RTC DS1307
- Librerías: `DHT.h`, `SPI.h`, `Ethernet.h`, `LiquidCrystal_I2C.h`, `Wire.h`, `RTClib.h`

### Dónde integrar: **Práctica 11 — Sistema de Automatización Completo**

**Justificación:** Es el proyecto más completo del workspace. Combina todos los conceptos aprendidos: servidor web, sensores, actuadores (relés), display LCD y reloj en tiempo real. Es la práctica "capstone" ideal.

**Objetivos de la práctica:**
1. Integrar múltiples periféricos I2C (LCD + RTC)
2. Controlar relés desde interfaz web
3. Mostrar información en LCD y web simultáneamente
4. Registrar eventos con timestamp (RTC)
5. Crear sistema de automatización basado en reglas

**Consideraciones:**
- Requiere **Arduino Mega 2560** (muchos pines y memoria)
- Pin 53 como OUTPUT (requerido para SPI en Mega)
- LCD I2C en dirección 0x27 — verificar con scanner I2C
- DHT11 en pin **A0** (inusual, normalmente se usa pin digital)
- Los relés pueden controlar cargas AC — **precaución con alto voltaje**

---

## 12. conectrar_stellarium-main — Conexión con Stellarium

**Directorio:** [`conectrar_stellarium-main/`](../conectrar_stellarium-main/)

### Qué hace
Recibe coordenadas celestes desde **Stellarium** (software de planetario) por TCP y las transforma en coordenadas altacimutales. Proyecto del Instituto Argentino de Radioastronomía.

### Tecnología
- Arduino + Ethernet Shield W5100
- Librería: `Ethernet.h`
- Protocolo: TCP en puerto 10000 (protocolo Stellarium)
- DHCP para obtener IP

### Dónde integrar: **Práctica Especial — Comunicación TCP Personalizada**

**Justificación:** Es el único proyecto que usa un **protocolo TCP personalizado** (no HTTP). Introduce conceptos de comunicación de bajo nivel y protocolos propietarios. Es muy especializado (astronomía) pero el concepto de servidor TCP es valioso.

**Objetivos de la práctica:**
1. Crear servidor TCP en puerto personalizado (no 80)
2. Recibir y parsear datos binarios/estructurados
3. Transformar coordenadas (ecuatoriales → altacimutales)
4. Integrar con software de escritorio (Stellarium)

**Consideraciones:**
- El código está **incompleto** (loop vacío) — necesita desarrollo
- Requiere Stellarium instalado y configurado para telescopio remoto
- Usa LCD (referencia a `lcd.print()`) pero no incluye la librería
- Puerto 10000 debe estar accesible en la red local

---

## Resumen: Mapa de Prácticas Propuesto

```
NIVEL BÁSICO (Fundamentos)
├── P1: Cliente HTTP + Servidor básico (LED/Switch)          [EXISTENTE]
├── P2: Control de I/O + AJAX + Voltímetro                   [EXISTENTE]
├── P2B: Arquitectura modular (LED RGB con clases C++)       [NUEVO - ServidorWebArduino-controlar_LedRGB]
│
NIVEL INTERMEDIO (Sensores y Datos)
├── P3: Sensores DHT + HTTP POST + MySQL                     [NUEVO - arduino-dht-ethernet + W5100-mysql + W5100-Eventos]
├── P4: SD card + AJAX + XML + Gauges                        [EXISTENTE]
│
NIVEL AVANZADO (Sistemas Completos)
├── P5: Múltiples I/O + Panel Solar                           [EXISTENTE]
├── P5B: Termostato dual con DHT                              [NUEVO - DualDHTthermostat]
├── P6: IoT - ThingSpeak                                      [EXISTENTE]
├── P6B: IoT - Plataformas alternativas + Telnet              [NUEVO - simpleweatherstation]
│
NIVEL EXPERTO (Integración Avanzada)
├── P7: Interfaz Web Moderna (Bootstrap + Animaciones)        [NUEVO - AlarmasUT]
├── P8: Comunicación GSM/SMS                                  [NUEVO - arduino-sms-web]
├── P9: Control por Voz vía Web                               [NUEVO - Arduino-Voice-control]
├── P10: Control de Acceso RFID                               [NUEVO - rfid]
├── P11: Sistema de Automatización Completo (Mega+DHT+Relés+LCD+RTC) [NUEVO - WebestufaServer]
│
PRÁCTICA ESPECIAL
└── PE: Comunicación TCP personalizada (Stellarium)           [NUEVO - conectrar_stellarium]
```

## Orden Recomendado de Implementación

| Prioridad | Práctica | Proyecto Base | Dificultad | Hardware Extra |
|---|---|---|---|---|
| 🟢 Alta | P3 | arduino-dht-ethernet + W5100-mysql | Media | DHT11 |
| 🟢 Alta | P2B | ServidorWebArduino-controlar_LedRGB | Baja | LED RGB |
| 🟡 Media | P5B | DualDHTthermostat | Media | 2x DHT + Relés |
| 🟡 Media | P7 | AlarmasUT | Media | Ninguno extra |
| 🟡 Media | P10 | rfid | Alta | MFRC522 + Tags |
| 🔴 Baja | P6B | simpleweatherstation | Media | DHT11 (Mega) |
| 🔴 Baja | P8 | arduino-sms-web | Alta | SIM900 + SIM |
| 🔴 Baja | P9 | Arduino-Voice-control | Alta | Servidor PHP |
| 🔴 Baja | P11 | WebestufaServer | Muy Alta | Mega + LCD + RTC + Relés |
| ⚪ Opcional | PE | conectrar_stellarium | Alta | Stellarium |
